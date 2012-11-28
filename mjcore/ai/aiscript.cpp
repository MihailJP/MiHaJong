#include "aiscript.h"

const DiscardTileNum aiscript::DiscardThrough = {DiscardTileNum::Normal, NUM_OF_TILES_IN_HAND - 1};

aiscript::ScriptStates aiscript::status[PLAYERS] = {{nullptr, false}};
const char aiscript::fncname_discard[8] = "ontsumo"; // �̔v����p�֐��̖��O
const char aiscript::fncname_call[3][12] = {"ondiscard", "onkakan", "onankan",}; // ������p�֐��̖��O

bool aiscript::callFunc(const GameTable* const gameStat, PLAYER_ID player_id, const char* const function_name, bool is_mandatory) {
	if (status[player_id].scriptLoaded) { /* �������ǂݍ��܂�Ă���Ȃ� */
		try { /* �V���{��������΂悵�A�Ȃ��������O���� */
			lua_getglobal(status[player_id].state, function_name);
		} catch (...) { /* �V���{�����Ȃ�������G���[�ɂȂ�̂ŗ�O���������� */
			CodeConv::tostringstream o;
			if (is_mandatory) {
				o << _T("�O���[�o���V���{�� [") << CodeConv::EnsureTStr(function_name) << _T("] �̎擾�Ɏ��s���܂���"); error(o.str().c_str());
				info(_T("���̃X�N���v�g�͎g�p�ł��܂���B�f�t�H���gAI(�c���؂�)�ɐ؂�ւ��܂��B"));
				status[player_id].scriptLoaded = false;
				chat::chatobj->sysmsg(CodeConv::tstring(_T("*** ")) + o.str());
				return true;
			} else {
				o << _T("�O���[�o���V���{�� [") << CodeConv::EnsureTStr(function_name) << _T("] �̎擾�Ɏ��s���܂����B�������܂��B");
				warn(o.str().c_str());
				return true;
			}
		}
		GameStatToLuaTable(status[player_id].state, gameStat);
		if (int errcode = lua_pcall(status[player_id].state, 1, 2, 0)) {
			/* ���s���s�I */
			CodeConv::tostringstream o;
			switch (errcode) {
			case LUA_ERRRUN:
				o << _T("�X�N���v�g�̎��s���G���[ [") <<
					CodeConv::DecodeStr(lua_tostring(status[player_id].state, -1)) /* �G���[���b�Z�[�W */ <<
					_T("]");
				lua_pop(status[player_id].state, 1);
				break;
			case LUA_ERRMEM: o << _T("�������̊����Ɏ��s���܂����B"); break;
			case LUA_ERRERR: o << _T("���b�Z�[�W�n���h�����s���̃G���[�ł��B"); break;
			case LUA_ERRGCMM: o << _T("�K�[�x�W�R���N�V�������s���̃G���[�ł��B"); break;
			}
			error(o.str().c_str());
			if (is_mandatory)
				chat::chatobj->sysmsg(CodeConv::tstring(_T("*** ")) + o.str());
			if (std::string(function_name) == std::string(fncname_discard))
				warn(_T("�֐��Ăяo���Ɏ��s�������߁A�c���؂�Ƃ݂Ȃ��܂�"));
			else
				warn(_T("�֐��Ăяo���Ɏ��s�������߁A�������܂�"));
			return true;
		} else {
			/* ���s���� */
			return false;
		}
	} else {
		if (std::string(function_name) == std::string(fncname_discard))
			warn(_T("�X�N���v�g�����[�h����Ă��Ȃ����A�g�p�ł��Ȃ����߁A�c���؂�Ƃ݂Ȃ��܂�"));
		else
			warn(_T("�X�N���v�g�����[�h����Ă��Ȃ����A�g�p�ł��Ȃ����߁A�������܂�"));
		return true; // �X�N���v�g�͎g�p�s�\
	}
}

__declspec(dllexport) void aiscript::initscript() {
	info(_T("AI�p�̃X�N���v�g�����������܂�"));
	FileSelector::filelist();
	// Lua������
	for (int i = 0; i < PLAYERS; i++) {
		status[i].state = luaL_newstate();
		luaopen_base(status[i].state); // base���C�u���������͊J���Ă����܂��傤
		table::functable::inittable(status[i].state, i);
		readfile(&status[i], FileSelector::randomfile().c_str()); /* �t�@�C����ǂݍ��� */
	}
	info(_T("�X�N���v�g�̏��������������܂���"));
}

__declspec(dllexport) void aiscript::initephemeral() {
	// ephemeral�e�[�u��������
	for (int i = 0; i < PLAYERS; i++) {
		lua_newtable(status[i].state);
		lua_setglobal(status[i].state, "ephemeral");
	}
	debug(_T("ephemeral �e�[�u�������������܂���"));
}

void aiscript::initcall(const GameTable* const gameStat, PLAYER_ID player) {
	callFunc(gameStat, player, "init", false);
}
__declspec(dllexport) void aiscript::initcall(const GameTable* const gameStat, int player) {
	initcall(gameStat, (PLAYER_ID)player);
}

__declspec(dllexport) void aiscript::closescript() {
	// Lua�N�����i�b�v
	for (int i = 0; i < PLAYERS; i++) {
		lua_close(status[i].state); // Lua�X�e�[�g���N���[�Y����
		status[i].scriptLoaded = false;
	}
	info(_T("�X�N���v�g��������܂���"));
}

void aiscript::readfile(aiscript::ScriptStates* const L, const char* const filename) {
	if (int errcode = luaL_loadfile(L->state, filename)) { /* �t�@�C����ǂݍ��݁B����������0��Ԃ� */
		/* �ǂݍ��ݎ��s�������̏��� */
		CodeConv::tostringstream o;
		o << _T("�X�N���v�g�t�@�C�� [") << CodeConv::EnsureTStr(filename) << _T("] �̓ǂݍ��݂Ɏ��s���܂����B");
		switch (errcode) {
			case LUA_ERRFILE: o << _T("�t�@�C�����J���܂���B"); break;
			case LUA_ERRSYNTAX: o << _T("�\��������������܂���B"); break;
			case LUA_ERRMEM: o << _T("�������̊����Ɏ��s���܂����B"); break;
			case LUA_ERRGCMM: o << _T("�K�[�x�W�R���N�V�������s���̃G���[�ł��B"); break;
		}
		error(o.str().c_str());
		chat::chatobj->sysmsg(CodeConv::tstring(_T("*** ")) + o.str());
	} else {
		CodeConv::tostringstream o;
		o << _T("�X�N���v�g�t�@�C�� [") << CodeConv::EnsureTStr(filename) << _T("] ��ǂݍ��݂܂����B");
		info(o.str().c_str());
		if (int errcode = lua_pcall(L->state, 0, LUA_MULTRET, 0)) {
			/* ���s���s�I */
			CodeConv::tostringstream o;
			switch (errcode) {
			case LUA_ERRRUN:
				o << _T("�X�N���v�g�̎��s���G���[ [") <<
					lua_tostring(L->state, -1) /* �G���[���b�Z�[�W */ <<
					_T("]");
				lua_pop(L->state, 1);
				break;
			case LUA_ERRMEM: o << _T("�������̊����Ɏ��s���܂����B"); break;
			case LUA_ERRERR: o << _T("���b�Z�[�W�n���h�����s���̃G���[�ł��B"); break;
			case LUA_ERRGCMM: o << _T("�K�[�x�W�R���N�V�������s���̃G���[�ł��B"); break;
			}
			error(o.str().c_str());
			chat::chatobj->sysmsg(CodeConv::tstring(_T("*** ")) + o.str());
		} else {
			/* ���s���� */
			info(_T("�X�N���v�g�̎��s�ɐ������܂���"));
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
	o << _T("AI�̑Ŕv�����ɓ���܂��B�v���C���[ [") << (int)gameStat->CurrentPlayer.Active << _T("]");
	info(o.str().c_str());
	if (callFunc(gameStat, gameStat->CurrentPlayer.Active, fncname_discard, true)) {
		*discard = DiscardThrough; *finished = true; return S_OK;
	} else {
		/* ���s���� */
		int flag;
		discard->type = (DiscardTileNum::discardType)lua_tointegerx(status[gameStat->CurrentPlayer.Active].state, -2, &flag);
		if (!flag) {
			warn(_T("1�Ԗڂ̕Ԃ�l�����l�ł͂���܂���B�ʏ�̑Ŕv�Ƃ݂Ȃ��܂��B"));
			discard->type = DiscardTileNum::Normal; // fallback
		} else if ((discard->type < DiscardTileNum::Normal) || (discard->type > DiscardTileNum::Disconnect)) {
			warn(_T("1�Ԗڂ̕Ԃ�l������������܂���B�ʏ�̑Ŕv�Ƃ݂Ȃ��܂��B"));
			discard->type = DiscardTileNum::Normal; // fallback
		}
		if ((discard->type == DiscardTileNum::Agari) || (discard->type == DiscardTileNum::Kyuushu) ||
			(discard->type == DiscardTileNum::Disconnect)) { // �ԍ��w�肪�s�v�ȏꍇ
				discard->id = NUM_OF_TILES_IN_HAND - 1; // 2�Ԃ߂̕Ԃ�l�͖���
		} else {
			int i = lua_tointegerx(status[gameStat->CurrentPlayer.Active].state, -1, &flag);
			if (!flag) {
				warn(_T("2�Ԗڂ̕Ԃ�l�����l�ł͂���܂���B�c���؂�Ƃ݂Ȃ��܂��B"));
				discard->id = NUM_OF_TILES_IN_HAND - 1; // fallback
			} else if ((i >= 1)&&(i <= NUM_OF_TILES_IN_HAND)) {
				discard->id = i - 1; // �I���W����1�ɂ���d�l�c�c
			} else if ((i <= -1)&&(i >= -NUM_OF_TILES_IN_HAND)) { // �}�C�i�X���w�肵���ꍇ�̏���
				discard->id = NUM_OF_TILES_IN_HAND + i;
			} else {
				warn(_T("2�Ԗڂ̕Ԃ�l���͈͊O�ł��B�c���؂�Ƃ݂Ȃ��܂��B"));
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
	o << "AI�̕��I����ɓ���܂��B�v���C���[ [" << (int)gameStat->CurrentPlayer.Passive << "]";
	info(o.str().c_str());
	gameStat->Player[gameStat->CurrentPlayer.Passive].DeclarationFlag.Chi = chiiNone; // ���Z�b�g
	gameStat->Player[gameStat->CurrentPlayer.Passive].DeclarationFlag.Pon =
		gameStat->Player[gameStat->CurrentPlayer.Passive].DeclarationFlag.Kan =
		gameStat->Player[gameStat->CurrentPlayer.Passive].DeclarationFlag.Ron = false;
	if (callFunc(gameStat, gameStat->CurrentPlayer.Passive, fncname_call[gameStat->KangFlag.chankanFlag], (gameStat->KangFlag.chankanFlag == 0))) {
		*finished = true; return S_OK;
	} else {
		/* ���s���� */
		int flag = 0;
		MeldCallID meldtype = (MeldCallID)lua_tointegerx(status[gameStat->CurrentPlayer.Passive].state, -2, &flag);
		if (!flag) {
			warn(_T("1�Ԗڂ̕Ԃ�l�����l�ł͂���܂���B�������܂��B"));
		} else {
			switch (meldtype) {
				case meldNone: break;
				case meldRon: gameStat->Player[gameStat->CurrentPlayer.Passive].DeclarationFlag.Ron = true; break;
				case meldKan: gameStat->Player[gameStat->CurrentPlayer.Passive].DeclarationFlag.Kan = true; break;
				case meldPon: gameStat->Player[gameStat->CurrentPlayer.Passive].DeclarationFlag.Pon = true; break;
				case meldChiiLower: gameStat->Player[gameStat->CurrentPlayer.Passive].DeclarationFlag.Chi = chiiLower; break;
				case meldChiiMiddle: gameStat->Player[gameStat->CurrentPlayer.Passive].DeclarationFlag.Chi = chiiMiddle; break;
				case meldChiiUpper: gameStat->Player[gameStat->CurrentPlayer.Passive].DeclarationFlag.Chi = chiiUpper; break;
				default: warn(_T("1�Ԗڂ̕Ԃ�l������������܂���B�������܂��B")); break;
			}
		}
		lua_pop(status[gameStat->CurrentPlayer.Passive].state, 1);
		*finished = true; return S_OK;
	}
}
