#include "aiscript.h"

const DiscardTileNum aiscript::DiscardThrough = {DiscardTileNum::Normal, NUM_OF_TILES_IN_HAND - 1};

aiscript::ScriptStates aiscript::status[PLAYERS] = {{NULL, false}};
const char aiscript::fncname_discard[8] = "ontsumo"; // 捨牌決定用関数の名前
const char aiscript::fncname_call[3][12] = {"ondiscard", "onkakan", "onankan",}; // 鳴き決定用関数の名前

__declspec(dllexport) void aiscript::initscript() {
	// Lua初期化 (仮)
	for (int i = 0; i < PLAYERS; i++) {
		status[i].state = luaL_newstate();
		luaopen_base(status[i].state); // baseライブラリだけは開いておきましょう
		table::functable::inittable(status[i].state, i);
		const char* filename = ".\\ai\\ai.lua"; /* ファイル名は仮 */
		readfile(&status[i], filename); /* ファイルを読み込み */
	}
}

__declspec(dllexport) void aiscript::initephemeral() {
	// ephemeralテーブル初期化
	for (int i = 0; i < PLAYERS; i++) {
		lua_newtable(status[i].state);
		lua_setglobal(status[i].state, "ephemeral");
	}
}

void aiscript::readfile(aiscript::ScriptStates* const L, const char* const filename) {
	if (int errcode = luaL_loadfile(L->state, filename)) { /* ファイルを読み込み。成功したら0を返す */
		/* 読み込み失敗した時の処理 */
		std::ostringstream o;
		o << "スクリプトファイル [" << filename << "] の読み込みに失敗しました。";
		switch (errcode) {
			case LUA_ERRFILE: o << "ファイルが開けません。"; break;
			case LUA_ERRSYNTAX: o << "構文が正しくありません。"; break;
			case LUA_ERRMEM: o << "メモリの割当に失敗しました。"; break;
			case LUA_ERRGCMM: o << "ガーベジコレクション実行中のエラーです。"; break;
		}
		error(o.str().c_str());
	} else {
		std::ostringstream o;
		o << "スクリプトファイル [" << filename << "] を読み込みました。";
		info(o.str().c_str());
		if (int errcode = lua_pcall(L->state, 0, LUA_MULTRET, 0)) {
			/* 実行失敗！ */
			std::ostringstream o;
			switch (errcode) {
			case LUA_ERRRUN:
				o << "スクリプトの実行時エラー [" <<
					lua_tostring(L->state, -1) /* エラーメッセージ */ <<
					"]";
				lua_pop(L->state, 1);
				break;
			case LUA_ERRMEM: o << "メモリの割当に失敗しました。"; break;
			case LUA_ERRERR: o << "メッセージハンドラ実行中のエラーです。"; break;
			case LUA_ERRGCMM: o << "ガーベジコレクション実行中のエラーです。"; break;
			}
			error(o.str().c_str());
		} else {
			/* 実行完了 */
			info("スクリプトの実行に成功しました");
			L->scriptLoaded = true;
		}
	}
}

void aiscript::GameStatToLuaTable(lua_State* const L, const GameTable* const gameStat) {
	lua_getglobal(L, table::functable::getTblName()); // get 'mihajong' table
	lua_getfield(L, -1, table::functable::getGTblName()); // get 'gametbl' table
	lua_remove(L, -2); // content of 'mihajong' table is no longer needed
	lua_getmetatable(L, -1); // get metatable
	lua_getfield(L, -1, "__index"); // get '__index' metamethod (table)
	lua_pushlightuserdata(L, (void *)gameStat); // gameStat pointer
	lua_setfield(L, -2, "addr"); // set 'addr' field
	lua_setfield(L, -2, "__index"); // update '__index' metamethod (table)
	lua_setmetatable(L, -2); // update metatable
}

__declspec(dllexport) int aiscript::compdahai(const GameTable* const gameStat) {
	return determine_discard(gameStat).toSingleInt();
}
DiscardTileNum aiscript::determine_discard(const GameTable* const gameStat) {
	std::ostringstream o;
	o << "AIの打牌処理に入ります。プレイヤー [" << (int)gameStat->CurrentPlayer.Active << "]";
	info(o.str().c_str());
	if (status[gameStat->CurrentPlayer.Active].scriptLoaded) { /* 正しく読み込まれているなら */
		try { /* determine_discard があればよし、なかったら例外処理 */
			lua_getglobal(status[gameStat->CurrentPlayer.Active].state, fncname_discard);
		} catch (...) { /* determine_discard がなかったらエラーになるので例外処理をする */
			std::ostringstream o;
			o << "グローバルシンボル [" << fncname_discard << "] の取得に失敗しました"; error(o.str().c_str());
			info("このスクリプトは使用できません。デフォルトAI(ツモ切り)に切り替えます。");
			status[gameStat->CurrentPlayer.Active].scriptLoaded = false; return DiscardThrough;
		}
		GameStatToLuaTable(status[gameStat->CurrentPlayer.Active].state, gameStat);
		if (int errcode = lua_pcall(status[gameStat->CurrentPlayer.Active].state, 1, 2, 0)) {
			/* 実行失敗！ */
			std::ostringstream o;
			switch (errcode) {
			case LUA_ERRRUN:
				o << "スクリプトの実行時エラー [" <<
					lua_tostring(status[gameStat->CurrentPlayer.Active].state, -1) /* エラーメッセージ */ <<
					"]";
				lua_pop(status[gameStat->CurrentPlayer.Active].state, 1);
				break;
			case LUA_ERRMEM: o << "メモリの割当に失敗しました。"; break;
			case LUA_ERRERR: o << "メッセージハンドラ実行中のエラーです。"; break;
			case LUA_ERRGCMM: o << "ガーベジコレクション実行中のエラーです。"; break;
			}
			error(o.str().c_str());
			warn("関数呼び出しに失敗したため、ツモ切りとみなします");
			return DiscardThrough;
		} else {
			/* 実行完了 */
			DiscardTileNum discard; int flag;
			discard.type = (DiscardTileNum::discardType)lua_tointegerx(status[gameStat->CurrentPlayer.Active].state, -2, &flag);
			if (!flag) {
				warn("1番目の返り値が数値ではありません。通常の打牌とみなします。");
				discard.type = DiscardTileNum::Normal; // fallback
			} else if ((discard.type < DiscardTileNum::Normal) || (discard.type > DiscardTileNum::Disconnect)) {
				warn("1番目の返り値が正しくありません。通常の打牌とみなします。");
				discard.type = DiscardTileNum::Normal; // fallback
			}
			if ((discard.type == DiscardTileNum::Agari) || (discard.type == DiscardTileNum::Kyuushu) ||
				(discard.type == DiscardTileNum::Disconnect)) { // 番号指定が不要な場合
					discard.id = NUM_OF_TILES_IN_HAND - 1; // 2番めの返り値は無視
			} else {
				int i = lua_tointegerx(status[gameStat->CurrentPlayer.Active].state, -1, &flag);
				if (!flag) {
					warn("2番目の返り値が数値ではありません。ツモ切りとみなします。");
					discard.id = NUM_OF_TILES_IN_HAND - 1; // fallback
				} else if ((i >= 1)&&(i <= NUM_OF_TILES_IN_HAND)) {
					discard.id = i - 1; // オリジンを1にする仕様……
				} else if ((i <= -1)&&(i >= -NUM_OF_TILES_IN_HAND)) { // マイナスを指定した場合の処理
					discard.id = NUM_OF_TILES_IN_HAND + i;
				} else {
					warn("2番目の返り値が範囲外です。ツモ切りとみなします。");
					discard.id = NUM_OF_TILES_IN_HAND - 1; // fallback
				}
			}
			lua_pop(status[gameStat->CurrentPlayer.Active].state, 2);
			return discard;
		}
	} else {
		warn("スクリプトがロードされていないか、使用できないため、ツモ切りとみなします");
		return DiscardThrough; // スクリプトは使用不能
	}
}


__declspec(dllexport) void aiscript::compfuuro(GameTable* const gameStat) {
	std::ostringstream o;
	o << "AIの副露判定に入ります。プレイヤー [" << (int)gameStat->CurrentPlayer.Passive << "]";
	info(o.str().c_str());
	gameStat->Player[gameStat->CurrentPlayer.Passive].DeclarationFlag.Chi = 0; // リセット
	gameStat->Player[gameStat->CurrentPlayer.Passive].DeclarationFlag.Pon =
		gameStat->Player[gameStat->CurrentPlayer.Passive].DeclarationFlag.Kan =
		gameStat->Player[gameStat->CurrentPlayer.Passive].DeclarationFlag.Ron = false;
	if (status[gameStat->CurrentPlayer.Active].scriptLoaded) { /* 正しく読み込まれているなら */
		try { /* determine_discard があればよし、なかったら例外処理 */
			lua_getglobal(status[gameStat->CurrentPlayer.Active].state, fncname_call[gameStat->KangFlag.chankanFlag]);
		} catch (...) { /* determine_discard がなかったらエラーになるので例外処理をする */
			if (gameStat->KangFlag.chankanFlag) {
				std::ostringstream o;
				o << "グローバルシンボル [" << fncname_call[0] << "] の取得に失敗しました"; error(o.str().c_str());
				info("このスクリプトは使用できません。デフォルトAI(ツモ切り)に切り替えます。");
				status[gameStat->CurrentPlayer.Active].scriptLoaded = false; return;
			} else {
				std::ostringstream o;
				o << "グローバルシンボル [" << fncname_call[gameStat->KangFlag.chankanFlag] <<
					"] の取得に失敗しました。無視します。";
				warn(o.str().c_str()); return;
			}
		}
		GameStatToLuaTable(status[gameStat->CurrentPlayer.Active].state, gameStat);
		if (int errcode = lua_pcall(status[gameStat->CurrentPlayer.Active].state, 1, 2, 0)) {
			/* 実行失敗！ */
			std::ostringstream o;
			switch (errcode) {
			case LUA_ERRRUN:
				o << "スクリプトの実行時エラー [" <<
					lua_tostring(status[gameStat->CurrentPlayer.Active].state, -1) /* エラーメッセージ */ <<
					"]";
				lua_pop(status[gameStat->CurrentPlayer.Active].state, 1);
				break;
			case LUA_ERRMEM: o << "メモリの割当に失敗しました。"; break;
			case LUA_ERRERR: o << "メッセージハンドラ実行中のエラーです。"; break;
			case LUA_ERRGCMM: o << "ガーベジコレクション実行中のエラーです。"; break;
			}
			error(o.str().c_str());
			warn("関数呼び出しに失敗したため、無視します");
			return;
		} else {
			/* 実行完了 */
			int flag = 0;
			MeldCallID meldtype = (MeldCallID)lua_tointegerx(status[gameStat->CurrentPlayer.Active].state, -2, &flag);
			if (!flag) {
				warn("1番目の返り値が数値ではありません。無視します。");
			} else {
				switch (meldtype) {
					case meldNone: break;
					case meldRon: gameStat->Player[gameStat->CurrentPlayer.Passive].DeclarationFlag.Ron = true; break;
					case meldKan: gameStat->Player[gameStat->CurrentPlayer.Passive].DeclarationFlag.Kan = true; break;
					case meldPon: gameStat->Player[gameStat->CurrentPlayer.Passive].DeclarationFlag.Pon = true; break;
					case meldChiiLower: gameStat->Player[gameStat->CurrentPlayer.Passive].DeclarationFlag.Chi = 1; break;
					case meldChiiMiddle: gameStat->Player[gameStat->CurrentPlayer.Passive].DeclarationFlag.Chi = 2; break;
					case meldChiiUpper: gameStat->Player[gameStat->CurrentPlayer.Passive].DeclarationFlag.Chi = 3; break;
					default: warn("1番目の返り値が正しくありません。無視します。"); break;
				}
			}
			lua_pop(status[gameStat->CurrentPlayer.Active].state, 1);
			return;
		}
	} else {
		warn("スクリプトがロードされていないか、使用できないため、無視します");
		return; // スクリプトは使用不能
	}
}
