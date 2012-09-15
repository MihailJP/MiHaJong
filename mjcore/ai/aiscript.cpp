#include "aiscript.h"

const DiscardTileNum aiscript::DiscardThrough = {DiscardTileNum::Normal, NUM_OF_TILES_IN_HAND - 1};

aiscript::ScriptStates aiscript::status[PLAYERS] = {{NULL, false}};
const char aiscript::fncname_discard[8] = "ontsumo"; // 捨牌決定用関数の名前
const char aiscript::fncname_call[3][12] = {"ondiscard", "onkakan", "onankan",}; // 鳴き決定用関数の名前

__declspec(dllexport) void aiscript::initscript() {
	info("AI用のスクリプトを初期化します");
	FileSelector::filelist();
	// Lua初期化
	for (int i = 0; i < PLAYERS; i++) {
		status[i].state = luaL_newstate();
		luaopen_base(status[i].state); // baseライブラリだけは開いておきましょう
		table::functable::inittable(status[i].state, i);
		readfile(&status[i], FileSelector::randomfile().c_str()); /* ファイルを読み込み */
	}
	info("スクリプトの初期化が完了しました");
}

__declspec(dllexport) void aiscript::initephemeral() {
	// ephemeralテーブル初期化
	for (int i = 0; i < PLAYERS; i++) {
		lua_newtable(status[i].state);
		lua_setglobal(status[i].state, "ephemeral");
	}
	debug("ephemeral テーブルを初期化しました");
}

__declspec(dllexport) void aiscript::closescript() {
	// Luaクリンナップ
	for (int i = 0; i < PLAYERS; i++) {
		lua_close(status[i].state); // Luaステートをクローズする
		status[i].scriptLoaded = false;
	}
	info("スクリプトを解放しました");
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

// -------------------------------------------------------------------------

aiscript::detDiscardThread* aiscript::discard_worker = NULL;
DiscardTileNum aiscript::discard;
bool aiscript::finished = false;
__declspec(dllexport) void aiscript::compdahai_begin(const GameTable* const gameStat) {
	discard = DiscardTileNum(); finished = false;
	discard_worker = new detDiscardThread();
	discard_worker->setprm(gameStat, &discard, &finished);
	DWORD threadID;
	HANDLE hThread = CreateThread(NULL, 0, detDiscardThread::execute, (LPVOID)discard_worker, 0, &threadID);
}
__declspec(dllexport) int aiscript::compdahai_check() {
	return finished ? 1 : 0;
}
__declspec(dllexport) int aiscript::compdahai() {
	delete discard_worker; discard_worker = NULL;
	return discard.toSingleInt();
}
DiscardTileNum aiscript::determine_discard(const GameTable* const gameStat) {
	discard = DiscardTileNum(); finished = false;
	discard_worker = new detDiscardThread();
	discard_worker->setprm(gameStat, &discard, &finished);
	DWORD threadID;
	HANDLE hThread = CreateThread(NULL, 0, detDiscardThread::execute, (LPVOID)discard_worker, 0, &threadID);
	while (!finished) Sleep(0);
	delete discard_worker; discard_worker = NULL;
	return discard;
}
aiscript::detDiscardThread::detDiscardThread() {
	i_gameStat = NULL; i_discard = NULL; i_finished = NULL;
}
aiscript::detDiscardThread::~detDiscardThread() {
}
void aiscript::detDiscardThread::setprm(const GameTable* const gameStat, DiscardTileNum* const discard, bool* const finished) {
	i_gameStat = gameStat; i_discard = discard; i_finished = finished;
}
DWORD WINAPI aiscript::detDiscardThread::execute(LPVOID param) {
	return ((detDiscardThread *)param)->calculate(
		((detDiscardThread *)param)->i_gameStat,
		((detDiscardThread *)param)->i_discard,
		((detDiscardThread *)param)->i_finished);
}
DWORD WINAPI aiscript::detDiscardThread::calculate(const GameTable* const gameStat, DiscardTileNum* const discard, bool* const finished) {
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
			status[gameStat->CurrentPlayer.Active].scriptLoaded = false;
			*discard = DiscardThrough; *finished = true; return S_OK;
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
			*discard = DiscardThrough; *finished = true; return S_OK;
		} else {
			/* 実行完了 */
			int flag;
			discard->type = (DiscardTileNum::discardType)lua_tointegerx(status[gameStat->CurrentPlayer.Active].state, -2, &flag);
			if (!flag) {
				warn("1番目の返り値が数値ではありません。通常の打牌とみなします。");
				discard->type = DiscardTileNum::Normal; // fallback
			} else if ((discard->type < DiscardTileNum::Normal) || (discard->type > DiscardTileNum::Disconnect)) {
				warn("1番目の返り値が正しくありません。通常の打牌とみなします。");
				discard->type = DiscardTileNum::Normal; // fallback
			}
			if ((discard->type == DiscardTileNum::Agari) || (discard->type == DiscardTileNum::Kyuushu) ||
				(discard->type == DiscardTileNum::Disconnect)) { // 番号指定が不要な場合
					discard->id = NUM_OF_TILES_IN_HAND - 1; // 2番めの返り値は無視
			} else {
				int i = lua_tointegerx(status[gameStat->CurrentPlayer.Active].state, -1, &flag);
				if (!flag) {
					warn("2番目の返り値が数値ではありません。ツモ切りとみなします。");
					discard->id = NUM_OF_TILES_IN_HAND - 1; // fallback
				} else if ((i >= 1)&&(i <= NUM_OF_TILES_IN_HAND)) {
					discard->id = i - 1; // オリジンを1にする仕様……
				} else if ((i <= -1)&&(i >= -NUM_OF_TILES_IN_HAND)) { // マイナスを指定した場合の処理
					discard->id = NUM_OF_TILES_IN_HAND + i;
				} else {
					warn("2番目の返り値が範囲外です。ツモ切りとみなします。");
					discard->id = NUM_OF_TILES_IN_HAND - 1; // fallback
				}
			}
			lua_pop(status[gameStat->CurrentPlayer.Active].state, 2);
			*finished = true; return S_OK;
		}
	} else {
		warn("スクリプトがロードされていないか、使用できないため、ツモ切りとみなします");
		*discard = DiscardThrough; *finished = true; return S_OK; // スクリプトは使用不能
	}
}

// -------------------------------------------------------------------------

aiscript::detCallThread* aiscript::meld_worker = NULL;
__declspec(dllexport) void aiscript::compfuuro_begin(GameTable* const gameStat) {
	finished = false;
	meld_worker = new detCallThread();
	meld_worker->setprm(gameStat, &finished);
	DWORD threadID;
	HANDLE hThread = CreateThread(NULL, 0, detCallThread::execute, (LPVOID)meld_worker, 0, &threadID);
}
__declspec(dllexport) int aiscript::compfuuro_check() {
	return finished ? 1 : 0;
}
__declspec(dllexport) void aiscript::compfuuro_end() {
	delete meld_worker; meld_worker = NULL;
}
void aiscript::determine_meld(GameTable* const gameStat) {
	finished = false;
	meld_worker = new detCallThread();
	meld_worker->setprm(gameStat, &finished);
	DWORD threadID;
	HANDLE hThread = CreateThread(NULL, 0, detCallThread::execute, (LPVOID)meld_worker, 0, &threadID);
	while (!finished) Sleep(0);
	delete meld_worker; meld_worker = NULL;
}
aiscript::detCallThread::detCallThread() {
	i_gameStat = NULL; i_finished = NULL;
}
aiscript::detCallThread::~detCallThread() {
}
void aiscript::detCallThread::setprm(GameTable* const gameStat, bool* const finished) {
	i_gameStat = gameStat; i_finished = finished;
}
DWORD WINAPI aiscript::detCallThread::execute(LPVOID param) {
	return ((detCallThread *)param)->calculate(
		((detCallThread *)param)->i_gameStat,
		((detCallThread *)param)->i_finished);
}

DWORD WINAPI aiscript::detCallThread::calculate(GameTable* const gameStat, bool* const finished) {
	std::ostringstream o;
	o << "AIの副露判定に入ります。プレイヤー [" << (int)gameStat->CurrentPlayer.Passive << "]";
	info(o.str().c_str());
	gameStat->Player[gameStat->CurrentPlayer.Passive].DeclarationFlag.Chi = 0; // リセット
	gameStat->Player[gameStat->CurrentPlayer.Passive].DeclarationFlag.Pon =
		gameStat->Player[gameStat->CurrentPlayer.Passive].DeclarationFlag.Kan =
		gameStat->Player[gameStat->CurrentPlayer.Passive].DeclarationFlag.Ron = false;
	if (status[gameStat->CurrentPlayer.Passive].scriptLoaded) { /* 正しく読み込まれているなら */
		try { /* determine_discard があればよし、なかったら例外処理 */
			lua_getglobal(status[gameStat->CurrentPlayer.Passive].state, fncname_call[gameStat->KangFlag.chankanFlag]);
		} catch (...) { /* determine_discard がなかったらエラーになるので例外処理をする */
			if (gameStat->KangFlag.chankanFlag) {
				std::ostringstream o;
				o << "グローバルシンボル [" << fncname_call[0] << "] の取得に失敗しました"; error(o.str().c_str());
				info("このスクリプトは使用できません。デフォルトAI(ツモ切り)に切り替えます。");
				status[gameStat->CurrentPlayer.Passive].scriptLoaded = false; return S_OK;
			} else {
				std::ostringstream o;
				o << "グローバルシンボル [" << fncname_call[gameStat->KangFlag.chankanFlag] <<
					"] の取得に失敗しました。無視します。";
				warn(o.str().c_str());
				*finished = true; return S_OK;
			}
		}
		GameStatToLuaTable(status[gameStat->CurrentPlayer.Passive].state, gameStat);
		if (int errcode = lua_pcall(status[gameStat->CurrentPlayer.Passive].state, 1, 2, 0)) {
			/* 実行失敗！ */
			std::ostringstream o;
			switch (errcode) {
			case LUA_ERRRUN:
				o << "スクリプトの実行時エラー [" <<
					lua_tostring(status[gameStat->CurrentPlayer.Passive].state, -1) /* エラーメッセージ */ <<
					"]";
				lua_pop(status[gameStat->CurrentPlayer.Passive].state, 1);
				break;
			case LUA_ERRMEM: o << "メモリの割当に失敗しました。"; break;
			case LUA_ERRERR: o << "メッセージハンドラ実行中のエラーです。"; break;
			case LUA_ERRGCMM: o << "ガーベジコレクション実行中のエラーです。"; break;
			}
			error(o.str().c_str());
			warn("関数呼び出しに失敗したため、無視します");
			*finished = true; return S_OK;
		} else {
			/* 実行完了 */
			int flag = 0;
			MeldCallID meldtype = (MeldCallID)lua_tointegerx(status[gameStat->CurrentPlayer.Passive].state, -2, &flag);
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
			lua_pop(status[gameStat->CurrentPlayer.Passive].state, 1);
			*finished = true; return S_OK;
		}
	} else {
		warn("スクリプトがロードされていないか、使用できないため、無視します");
		*finished = true; return S_OK; // スクリプトは使用不能
	}
}
