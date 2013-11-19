#include "aiscript.h"

#include <lua.hpp>
#include <sstream>
#include "functbl.h"
#include "filesel.h"
#include "../gametbl.h"
#include "../logging.h"
#include "../chat.h"
#include "../../common/strcode.h"

const DiscardTileNum aiscript::DiscardThrough = {DiscardTileNum::Normal, NumOfTilesInHand - 1};

aiscript::ScriptStates aiscript::status[Players] = {{nullptr, false}};
const char aiscript::fncname_discard[8] = "ontsumo"; // 捨牌決定用関数の名前
const char aiscript::fncname_call[3][12] = {"ondiscard", "onkakan", "onankan",}; // 鳴き決定用関数の名前

bool aiscript::callFunc(const GameTable* const gameStat, PlayerID PlayerID, const char* const function_name, bool is_mandatory) {
	if (status[PlayerID].scriptLoaded) { /* 正しく読み込まれているなら */
		try { /* シンボルがあればよし、なかったら例外処理 */
			lua_getglobal(status[PlayerID].state, function_name);
		} catch (...) { /* シンボルがなかったらエラーになるので例外処理をする */
			CodeConv::tostringstream o;
			if (is_mandatory) {
				o << _T("グローバルシンボル [") << CodeConv::EnsureTStr(function_name) << _T("] の取得に失敗しました"); error(o.str().c_str());
				info(_T("このスクリプトは使用できません。デフォルトAI(ツモ切り)に切り替えます。"));
				status[PlayerID].scriptLoaded = false;
				chat::chatobj->sysmsg(CodeConv::tstring(_T("*** ")) + o.str());
				return true;
			} else {
				o << _T("グローバルシンボル [") << CodeConv::EnsureTStr(function_name) << _T("] の取得に失敗しました。無視します。");
				warn(o.str().c_str());
				return true;
			}
		}
		GameStatToLuaTable(status[PlayerID].state, gameStat);
		if (int errcode = lua_pcall(status[PlayerID].state, 1, 2, 0)) {
			/* 実行失敗！ */
			CodeConv::tostringstream o;
			switch (errcode) {
			case LUA_ERRRUN:
				o << _T("スクリプトの実行時エラー [") <<
					CodeConv::DecodeStr(lua_tostring(status[PlayerID].state, -1)) /* エラーメッセージ */ <<
					_T("]");
				lua_pop(status[PlayerID].state, 1);
				break;
			case LUA_ERRMEM: o << _T("メモリの割当に失敗しました。"); break;
			case LUA_ERRERR: o << _T("メッセージハンドラ実行中のエラーです。"); break;
			case LUA_ERRGCMM: o << _T("ガーベジコレクション実行中のエラーです。"); break;
			}
			error(o.str().c_str());
			if (is_mandatory)
				chat::chatobj->sysmsg(CodeConv::tstring(_T("*** ")) + o.str());
			if (std::string(function_name) == std::string(fncname_discard))
				warn(_T("関数呼び出しに失敗したため、ツモ切りとみなします"));
			else
				warn(_T("関数呼び出しに失敗したため、無視します"));
			return true;
		} else {
			/* 実行完了 */
			return false;
		}
	} else {
		if (std::string(function_name) == std::string(fncname_discard))
			warn(_T("スクリプトがロードされていないか、使用できないため、ツモ切りとみなします"));
		else
			warn(_T("スクリプトがロードされていないか、使用できないため、無視します"));
		return true; // スクリプトは使用不能
	}
}

void aiscript::initscript() {
	info(_T("AI用のスクリプトを初期化します"));
	FileSelector::filelist();
	// Lua初期化
	for (int i = 0; i < Players; i++) {
		status[i].state = luaL_newstate();
		luaopen_base(status[i].state); // baseライブラリだけは開いておきましょう
		table::functable::inittable(status[i].state, i);
		readfile(&status[i], FileSelector::randomfile().c_str()); /* ファイルを読み込み */
	}
	info(_T("スクリプトの初期化が完了しました"));
}

void aiscript::initephemeral() {
	// ephemeralテーブル初期化
	for (int i = 0; i < Players; i++) {
		lua_newtable(status[i].state);
		lua_setglobal(status[i].state, "ephemeral");
	}
	debug(_T("ephemeral テーブルを初期化しました"));
}

void aiscript::initcall(const GameTable* const gameStat, PlayerID player) {
	callFunc(gameStat, player, "init", false);
}

void aiscript::closescript() {
	// Luaクリンナップ
	for (int i = 0; i < Players; i++) {
		lua_close(status[i].state); // Luaステートをクローズする
		status[i].scriptLoaded = false;
	}
	info(_T("スクリプトを解放しました"));
}

void aiscript::readfile(aiscript::ScriptStates* const L, const char* const filename) {
	if (int errcode = luaL_loadfile(L->state, filename)) { /* ファイルを読み込み。成功したら0を返す */
		/* 読み込み失敗した時の処理 */
		CodeConv::tostringstream o;
		o << _T("スクリプトファイル [") << CodeConv::EnsureTStr(filename) << _T("] の読み込みに失敗しました。");
		switch (errcode) {
			case LUA_ERRFILE: o << _T("ファイルが開けません。"); break;
			case LUA_ERRSYNTAX: o << _T("構文が正しくありません。"); break;
			case LUA_ERRMEM: o << _T("メモリの割当に失敗しました。"); break;
			case LUA_ERRGCMM: o << _T("ガーベジコレクション実行中のエラーです。"); break;
		}
		error(o.str().c_str());
		chat::chatobj->sysmsg(CodeConv::tstring(_T("*** ")) + o.str());
	} else {
		CodeConv::tostringstream o;
		o << _T("スクリプトファイル [") << CodeConv::EnsureTStr(filename) << _T("] を読み込みました。");
		info(o.str().c_str());
		if (int errcode = lua_pcall(L->state, 0, LUA_MULTRET, 0)) {
			/* 実行失敗！ */
			CodeConv::tostringstream o;
			switch (errcode) {
			case LUA_ERRRUN:
				o << _T("スクリプトの実行時エラー [") <<
					lua_tostring(L->state, -1) /* エラーメッセージ */ <<
					_T("]");
				lua_pop(L->state, 1);
				break;
			case LUA_ERRMEM: o << _T("メモリの割当に失敗しました。"); break;
			case LUA_ERRERR: o << _T("メッセージハンドラ実行中のエラーです。"); break;
			case LUA_ERRGCMM: o << _T("ガーベジコレクション実行中のエラーです。"); break;
			}
			error(o.str().c_str());
			chat::chatobj->sysmsg(CodeConv::tstring(_T("*** ")) + o.str());
		} else {
			/* 実行完了 */
			info(_T("スクリプトの実行に成功しました"));
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

// -------------------------------------------------------------------------

DiscardTileNum aiscript::compdahai(const GameTable* const gameStat) {
	return determine_discard(gameStat);
}
DiscardTileNum aiscript::determine_discard(const GameTable* const gameStat) {
	DiscardTileNum discard = {DiscardTileNum::Normal, NumOfTilesInHand - 1};
	THREADLIB::thread myThread(calcDiscard_threaded, std::ref(discard), gameStat);
	myThread.join();
	return discard;
}
void aiscript::calcDiscard_threaded(DiscardTileNum& answer, const GameTable* gameStat) {
	CodeConv::tostringstream o;
	o << _T("AIの打牌処理に入ります。プレイヤー [") << (int)gameStat->CurrentPlayer.Active << _T("]");
	info(o.str().c_str());
	if (callFunc(gameStat, gameStat->CurrentPlayer.Active, fncname_discard, true)) {
		answer = DiscardThrough;
		return;
	} else {
		/* 実行完了 */
		int flag;
		answer.type = (DiscardTileNum::discardType)lua_tointegerx(status[gameStat->CurrentPlayer.Active].state, -2, &flag);
		if (!flag) {
			warn(_T("1番目の返り値が数値ではありません。通常の打牌とみなします。"));
			answer.type = DiscardTileNum::Normal; // fallback
		} else if ((answer.type < DiscardTileNum::Normal) || (answer.type > DiscardTileNum::Disconnect)) {
			warn(_T("1番目の返り値が正しくありません。通常の打牌とみなします。"));
			answer.type = DiscardTileNum::Normal; // fallback
		}
		if ((answer.type == DiscardTileNum::Agari) || (answer.type == DiscardTileNum::Kyuushu) ||
			(answer.type == DiscardTileNum::Disconnect)) { // 番号指定が不要な場合
				answer.id = NumOfTilesInHand - 1; // 2番めの返り値は無視
		} else {
			int i = lua_tointegerx(status[gameStat->CurrentPlayer.Active].state, -1, &flag);
			if (!flag) {
				warn(_T("2番目の返り値が数値ではありません。ツモ切りとみなします。"));
				answer.id = NumOfTilesInHand - 1; // fallback
			} else if ((i >= 1)&&(i <= NumOfTilesInHand)) {
				answer.id = i - 1; // オリジンを1にする仕様……
			} else if ((i <= -1)&&(i >= -((int)NumOfTilesInHand))) { // マイナスを指定した場合の処理
				answer.id = NumOfTilesInHand + i;
			} else {
				warn(_T("2番目の返り値が範囲外です。ツモ切りとみなします。"));
				answer.id = NumOfTilesInHand - 1; // fallback
			}
		}
		lua_pop(status[gameStat->CurrentPlayer.Active].state, 2);
		return;
	}
}

// -------------------------------------------------------------------------

void aiscript::compfuuro(GameTable* const gameStat) {
	determine_meld(gameStat);
}
void aiscript::determine_meld(GameTable* const gameStat) {
	THREADLIB::thread myThread(calcCall_threaded, gameStat);
	myThread.join();
}

void aiscript::calcCall_threaded(GameTable* gameStat) {
	CodeConv::tostringstream o;
	o << _T("AIの副露判定に入ります。プレイヤー [") << (int)gameStat->CurrentPlayer.Passive << _T("]");
	info(o.str().c_str());
	gameStat->statOfPassive().DeclarationFlag.Chi = chiiNone; // リセット
	gameStat->statOfPassive().DeclarationFlag.Pon =
		gameStat->statOfPassive().DeclarationFlag.Kan =
		gameStat->statOfPassive().DeclarationFlag.Ron = false;
	if (callFunc(gameStat, gameStat->CurrentPlayer.Passive, fncname_call[gameStat->KangFlag.chankanFlag], (gameStat->KangFlag.chankanFlag == 0))) {
		return;
	} else {
		/* 実行完了 */
		int flag = 0;
		MeldCallID meldtype = (MeldCallID)lua_tointegerx(status[gameStat->CurrentPlayer.Passive].state, -2, &flag);
		if (!flag) {
			warn(_T("1番目の返り値が数値ではありません。無視します。"));
		} else {
			switch (meldtype) {
				case meldNone: break;
				case meldRon: gameStat->statOfPassive().DeclarationFlag.Ron = true; break;
				case meldKan: gameStat->statOfPassive().DeclarationFlag.Kan = true; break;
				case meldPon: gameStat->statOfPassive().DeclarationFlag.Pon = true; break;
				case meldChiiLower: gameStat->statOfPassive().DeclarationFlag.Chi = chiiLower; break;
				case meldChiiMiddle: gameStat->statOfPassive().DeclarationFlag.Chi = chiiMiddle; break;
				case meldChiiUpper: gameStat->statOfPassive().DeclarationFlag.Chi = chiiUpper; break;
				default: warn(_T("1番目の返り値が正しくありません。無視します。")); break;
			}
		}
		lua_pop(status[gameStat->CurrentPlayer.Passive].state, 1);
		return;
	}
}
