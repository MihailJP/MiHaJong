#include "aiscript.h"

const DiscardTileNum aiscript::DiscardThrough = {DiscardTileNum::Normal, NUM_OF_TILES_IN_HAND - 1};

aiscript::ScriptStates aiscript::status[PLAYERS] = {{nullptr, false}};
const char aiscript::fncname_discard[8] = "ontsumo"; // 捨牌決定用関数の名前
const char aiscript::fncname_call[3][12] = {"ondiscard", "onkakan", "onankan",}; // 鳴き決定用関数の名前

bool aiscript::callFunc(const GameTable* const gameStat, PLAYER_ID player_id, const char* const function_name, bool is_mandatory) {
	if (status[player_id].scriptLoaded) { /* 正しく読み込まれているなら */
		try { /* シンボルがあればよし、なかったら例外処理 */
			lua_getglobal(status[player_id].state, function_name);
		} catch (...) { /* シンボルがなかったらエラーになるので例外処理をする */
			CodeConv::tostringstream o;
			if (is_mandatory) {
				o << _T("グローバルシンボル [") << CodeConv::EnsureTStr(function_name) << _T("] の取得に失敗しました"); error(o.str().c_str());
				info(_T("このスクリプトは使用できません。デフォルトAI(ツモ切り)に切り替えます。"));
				status[player_id].scriptLoaded = false;
				chat::chatobj->sysmsg(CodeConv::tstring(_T("*** ")) + o.str());
				return true;
			} else {
				o << _T("グローバルシンボル [") << CodeConv::EnsureTStr(function_name) << _T("] の取得に失敗しました。無視します。");
				warn(o.str().c_str());
				return true;
			}
		}
		GameStatToLuaTable(status[player_id].state, gameStat);
		if (int errcode = lua_pcall(status[player_id].state, 1, 2, 0)) {
			/* 実行失敗！ */
			CodeConv::tostringstream o;
			switch (errcode) {
			case LUA_ERRRUN:
				o << _T("スクリプトの実行時エラー [") <<
					CodeConv::DecodeStr(lua_tostring(status[player_id].state, -1)) /* エラーメッセージ */ <<
					_T("]");
				lua_pop(status[player_id].state, 1);
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

__declspec(dllexport) void aiscript::initscript() {
	info(_T("AI用のスクリプトを初期化します"));
	FileSelector::filelist();
	// Lua初期化
	for (int i = 0; i < PLAYERS; i++) {
		status[i].state = luaL_newstate();
		luaopen_base(status[i].state); // baseライブラリだけは開いておきましょう
		table::functable::inittable(status[i].state, i);
		readfile(&status[i], FileSelector::randomfile().c_str()); /* ファイルを読み込み */
	}
	info(_T("スクリプトの初期化が完了しました"));
}

__declspec(dllexport) void aiscript::initephemeral() {
	// ephemeralテーブル初期化
	for (int i = 0; i < PLAYERS; i++) {
		lua_newtable(status[i].state);
		lua_setglobal(status[i].state, "ephemeral");
	}
	debug(_T("ephemeral テーブルを初期化しました"));
}

void aiscript::initcall(const GameTable* const gameStat, PLAYER_ID player) {
	callFunc(gameStat, player, "init", false);
}
__declspec(dllexport) void aiscript::initcall(const GameTable* const gameStat, int player) {
	initcall(gameStat, (PLAYER_ID)player);
}

__declspec(dllexport) void aiscript::closescript() {
	// Luaクリンナップ
	for (int i = 0; i < PLAYERS; i++) {
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

aiscript::detDiscardThread* aiscript::discard_worker = nullptr;
DiscardTileNum aiscript::discard;
bool aiscript::finished = false;
DiscardTileNum aiscript::compdahai(const GameTable* const gameStat) {
	discard = DiscardTileNum(); finished = false;
	discard_worker = new detDiscardThread();
	discard_worker->setprm(gameStat, &discard, &finished);
	DWORD threadID;
	HANDLE hThread = CreateThread(nullptr, 0, detDiscardThread::execute, (LPVOID)discard_worker, 0, &threadID);
	while (!finished) Sleep(1);
	delete discard_worker; discard_worker = nullptr;
	return discard;
}
DiscardTileNum aiscript::determine_discard(const GameTable* const gameStat) {
	discard = DiscardTileNum(); finished = false;
	discard_worker = new detDiscardThread();
	discard_worker->setprm(gameStat, &discard, &finished);
	DWORD threadID;
	HANDLE hThread = CreateThread(nullptr, 0, detDiscardThread::execute, (LPVOID)discard_worker, 0, &threadID);
	while (!finished) Sleep(0);
	delete discard_worker; discard_worker = nullptr;
	return discard;
}
aiscript::detDiscardThread::detDiscardThread() {
	i_gameStat = nullptr; i_discard = nullptr; i_finished = nullptr;
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
	CodeConv::tostringstream o;
	o << _T("AIの打牌処理に入ります。プレイヤー [") << (int)gameStat->CurrentPlayer.Active << _T("]");
	info(o.str().c_str());
	if (callFunc(gameStat, gameStat->CurrentPlayer.Active, fncname_discard, true)) {
		*discard = DiscardThrough; *finished = true; return S_OK;
	} else {
		/* 実行完了 */
		int flag;
		discard->type = (DiscardTileNum::discardType)lua_tointegerx(status[gameStat->CurrentPlayer.Active].state, -2, &flag);
		if (!flag) {
			warn(_T("1番目の返り値が数値ではありません。通常の打牌とみなします。"));
			discard->type = DiscardTileNum::Normal; // fallback
		} else if ((discard->type < DiscardTileNum::Normal) || (discard->type > DiscardTileNum::Disconnect)) {
			warn(_T("1番目の返り値が正しくありません。通常の打牌とみなします。"));
			discard->type = DiscardTileNum::Normal; // fallback
		}
		if ((discard->type == DiscardTileNum::Agari) || (discard->type == DiscardTileNum::Kyuushu) ||
			(discard->type == DiscardTileNum::Disconnect)) { // 番号指定が不要な場合
				discard->id = NUM_OF_TILES_IN_HAND - 1; // 2番めの返り値は無視
		} else {
			int i = lua_tointegerx(status[gameStat->CurrentPlayer.Active].state, -1, &flag);
			if (!flag) {
				warn(_T("2番目の返り値が数値ではありません。ツモ切りとみなします。"));
				discard->id = NUM_OF_TILES_IN_HAND - 1; // fallback
			} else if ((i >= 1)&&(i <= NUM_OF_TILES_IN_HAND)) {
				discard->id = i - 1; // オリジンを1にする仕様……
			} else if ((i <= -1)&&(i >= -NUM_OF_TILES_IN_HAND)) { // マイナスを指定した場合の処理
				discard->id = NUM_OF_TILES_IN_HAND + i;
			} else {
				warn(_T("2番目の返り値が範囲外です。ツモ切りとみなします。"));
				discard->id = NUM_OF_TILES_IN_HAND - 1; // fallback
			}
		}
		lua_pop(status[gameStat->CurrentPlayer.Active].state, 2);
		*finished = true; return S_OK;
	}
}

// -------------------------------------------------------------------------

aiscript::detCallThread* aiscript::meld_worker = nullptr;
void aiscript::compfuuro(GameTable* const gameStat) {
	finished = false;
	meld_worker = new detCallThread();
	meld_worker->setprm(gameStat, &finished);
	DWORD threadID;
	HANDLE hThread = CreateThread(nullptr, 0, detCallThread::execute, (LPVOID)meld_worker, 0, &threadID);
	while (!finished)
		Sleep(1);
	delete meld_worker; meld_worker = nullptr;
}
void aiscript::determine_meld(GameTable* const gameStat) {
	finished = false;
	meld_worker = new detCallThread();
	meld_worker->setprm(gameStat, &finished);
	DWORD threadID;
	HANDLE hThread = CreateThread(nullptr, 0, detCallThread::execute, (LPVOID)meld_worker, 0, &threadID);
	while (!finished) Sleep(0);
	delete meld_worker; meld_worker = nullptr;
}
aiscript::detCallThread::detCallThread() {
	i_gameStat = nullptr; i_finished = nullptr;
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
	CodeConv::tostringstream o;
	o << "AIの副露判定に入ります。プレイヤー [" << (int)gameStat->CurrentPlayer.Passive << "]";
	info(o.str().c_str());
	gameStat->Player[gameStat->CurrentPlayer.Passive].DeclarationFlag.Chi = chiiNone; // リセット
	gameStat->Player[gameStat->CurrentPlayer.Passive].DeclarationFlag.Pon =
		gameStat->Player[gameStat->CurrentPlayer.Passive].DeclarationFlag.Kan =
		gameStat->Player[gameStat->CurrentPlayer.Passive].DeclarationFlag.Ron = false;
	if (callFunc(gameStat, gameStat->CurrentPlayer.Passive, fncname_call[gameStat->KangFlag.chankanFlag], (gameStat->KangFlag.chankanFlag == 0))) {
		*finished = true; return S_OK;
	} else {
		/* 実行完了 */
		int flag = 0;
		MeldCallID meldtype = (MeldCallID)lua_tointegerx(status[gameStat->CurrentPlayer.Passive].state, -2, &flag);
		if (!flag) {
			warn(_T("1番目の返り値が数値ではありません。無視します。"));
		} else {
			switch (meldtype) {
				case meldNone: break;
				case meldRon: gameStat->Player[gameStat->CurrentPlayer.Passive].DeclarationFlag.Ron = true; break;
				case meldKan: gameStat->Player[gameStat->CurrentPlayer.Passive].DeclarationFlag.Kan = true; break;
				case meldPon: gameStat->Player[gameStat->CurrentPlayer.Passive].DeclarationFlag.Pon = true; break;
				case meldChiiLower: gameStat->Player[gameStat->CurrentPlayer.Passive].DeclarationFlag.Chi = chiiLower; break;
				case meldChiiMiddle: gameStat->Player[gameStat->CurrentPlayer.Passive].DeclarationFlag.Chi = chiiMiddle; break;
				case meldChiiUpper: gameStat->Player[gameStat->CurrentPlayer.Passive].DeclarationFlag.Chi = chiiUpper; break;
				default: warn(_T("1番目の返り値が正しくありません。無視します。")); break;
			}
		}
		lua_pop(status[gameStat->CurrentPlayer.Passive].state, 1);
		*finished = true; return S_OK;
	}
}
