#include "aiscript.h"

const DiscardTileNum aiscript::DiscardThrough = {DiscardTileNum::Normal, NUM_OF_TILES_IN_HAND - 1};

aiscript::ScriptStates aiscript::status[PLAYERS] = {{NULL, false}};
const char aiscript::fncname_discard[8] = "ontsumo"; // �̔v����p�֐��̖��O
const char aiscript::fncname_call[3][12] = {"ondiscard", "onkakan", "onankan",}; // ������p�֐��̖��O

__declspec(dllexport) void aiscript::initscript() {
	info("AI�p�̃X�N���v�g�����������܂�");
	FileSelector::filelist();
	// Lua������
	for (int i = 0; i < PLAYERS; i++) {
		status[i].state = luaL_newstate();
		luaopen_base(status[i].state); // base���C�u���������͊J���Ă����܂��傤
		table::functable::inittable(status[i].state, i);
		readfile(&status[i], FileSelector::randomfile().c_str()); /* �t�@�C����ǂݍ��� */
	}
	info("�X�N���v�g�̏��������������܂���");
}

__declspec(dllexport) void aiscript::initephemeral() {
	// ephemeral�e�[�u��������
	for (int i = 0; i < PLAYERS; i++) {
		lua_newtable(status[i].state);
		lua_setglobal(status[i].state, "ephemeral");
	}
	debug("ephemeral �e�[�u�������������܂���");
}

__declspec(dllexport) void aiscript::closescript() {
	// Lua�N�����i�b�v
	for (int i = 0; i < PLAYERS; i++) {
		lua_close(status[i].state); // Lua�X�e�[�g���N���[�Y����
		status[i].scriptLoaded = false;
	}
	info("�X�N���v�g��������܂���");
}

void aiscript::readfile(aiscript::ScriptStates* const L, const char* const filename) {
	if (int errcode = luaL_loadfile(L->state, filename)) { /* �t�@�C����ǂݍ��݁B����������0��Ԃ� */
		/* �ǂݍ��ݎ��s�������̏��� */
		std::ostringstream o;
		o << "�X�N���v�g�t�@�C�� [" << filename << "] �̓ǂݍ��݂Ɏ��s���܂����B";
		switch (errcode) {
			case LUA_ERRFILE: o << "�t�@�C�����J���܂���B"; break;
			case LUA_ERRSYNTAX: o << "�\��������������܂���B"; break;
			case LUA_ERRMEM: o << "�������̊����Ɏ��s���܂����B"; break;
			case LUA_ERRGCMM: o << "�K�[�x�W�R���N�V�������s���̃G���[�ł��B"; break;
		}
		error(o.str().c_str());
	} else {
		std::ostringstream o;
		o << "�X�N���v�g�t�@�C�� [" << filename << "] ��ǂݍ��݂܂����B";
		info(o.str().c_str());
		if (int errcode = lua_pcall(L->state, 0, LUA_MULTRET, 0)) {
			/* ���s���s�I */
			std::ostringstream o;
			switch (errcode) {
			case LUA_ERRRUN:
				o << "�X�N���v�g�̎��s���G���[ [" <<
					lua_tostring(L->state, -1) /* �G���[���b�Z�[�W */ <<
					"]";
				lua_pop(L->state, 1);
				break;
			case LUA_ERRMEM: o << "�������̊����Ɏ��s���܂����B"; break;
			case LUA_ERRERR: o << "���b�Z�[�W�n���h�����s���̃G���[�ł��B"; break;
			case LUA_ERRGCMM: o << "�K�[�x�W�R���N�V�������s���̃G���[�ł��B"; break;
			}
			error(o.str().c_str());
		} else {
			/* ���s���� */
			info("�X�N���v�g�̎��s�ɐ������܂���");
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
	o << "AI�̑Ŕv�����ɓ���܂��B�v���C���[ [" << (int)gameStat->CurrentPlayer.Active << "]";
	info(o.str().c_str());
	if (status[gameStat->CurrentPlayer.Active].scriptLoaded) { /* �������ǂݍ��܂�Ă���Ȃ� */
		try { /* determine_discard ������΂悵�A�Ȃ��������O���� */
			lua_getglobal(status[gameStat->CurrentPlayer.Active].state, fncname_discard);
		} catch (...) { /* determine_discard ���Ȃ�������G���[�ɂȂ�̂ŗ�O���������� */
			std::ostringstream o;
			o << "�O���[�o���V���{�� [" << fncname_discard << "] �̎擾�Ɏ��s���܂���"; error(o.str().c_str());
			info("���̃X�N���v�g�͎g�p�ł��܂���B�f�t�H���gAI(�c���؂�)�ɐ؂�ւ��܂��B");
			status[gameStat->CurrentPlayer.Active].scriptLoaded = false;
			*discard = DiscardThrough; *finished = true; return S_OK;
		}
		GameStatToLuaTable(status[gameStat->CurrentPlayer.Active].state, gameStat);
		if (int errcode = lua_pcall(status[gameStat->CurrentPlayer.Active].state, 1, 2, 0)) {
			/* ���s���s�I */
			std::ostringstream o;
			switch (errcode) {
			case LUA_ERRRUN:
				o << "�X�N���v�g�̎��s���G���[ [" <<
					lua_tostring(status[gameStat->CurrentPlayer.Active].state, -1) /* �G���[���b�Z�[�W */ <<
					"]";
				lua_pop(status[gameStat->CurrentPlayer.Active].state, 1);
				break;
			case LUA_ERRMEM: o << "�������̊����Ɏ��s���܂����B"; break;
			case LUA_ERRERR: o << "���b�Z�[�W�n���h�����s���̃G���[�ł��B"; break;
			case LUA_ERRGCMM: o << "�K�[�x�W�R���N�V�������s���̃G���[�ł��B"; break;
			}
			error(o.str().c_str());
			warn("�֐��Ăяo���Ɏ��s�������߁A�c���؂�Ƃ݂Ȃ��܂�");
			*discard = DiscardThrough; *finished = true; return S_OK;
		} else {
			/* ���s���� */
			int flag;
			discard->type = (DiscardTileNum::discardType)lua_tointegerx(status[gameStat->CurrentPlayer.Active].state, -2, &flag);
			if (!flag) {
				warn("1�Ԗڂ̕Ԃ�l�����l�ł͂���܂���B�ʏ�̑Ŕv�Ƃ݂Ȃ��܂��B");
				discard->type = DiscardTileNum::Normal; // fallback
			} else if ((discard->type < DiscardTileNum::Normal) || (discard->type > DiscardTileNum::Disconnect)) {
				warn("1�Ԗڂ̕Ԃ�l������������܂���B�ʏ�̑Ŕv�Ƃ݂Ȃ��܂��B");
				discard->type = DiscardTileNum::Normal; // fallback
			}
			if ((discard->type == DiscardTileNum::Agari) || (discard->type == DiscardTileNum::Kyuushu) ||
				(discard->type == DiscardTileNum::Disconnect)) { // �ԍ��w�肪�s�v�ȏꍇ
					discard->id = NUM_OF_TILES_IN_HAND - 1; // 2�Ԃ߂̕Ԃ�l�͖���
			} else {
				int i = lua_tointegerx(status[gameStat->CurrentPlayer.Active].state, -1, &flag);
				if (!flag) {
					warn("2�Ԗڂ̕Ԃ�l�����l�ł͂���܂���B�c���؂�Ƃ݂Ȃ��܂��B");
					discard->id = NUM_OF_TILES_IN_HAND - 1; // fallback
				} else if ((i >= 1)&&(i <= NUM_OF_TILES_IN_HAND)) {
					discard->id = i - 1; // �I���W����1�ɂ���d�l�c�c
				} else if ((i <= -1)&&(i >= -NUM_OF_TILES_IN_HAND)) { // �}�C�i�X���w�肵���ꍇ�̏���
					discard->id = NUM_OF_TILES_IN_HAND + i;
				} else {
					warn("2�Ԗڂ̕Ԃ�l���͈͊O�ł��B�c���؂�Ƃ݂Ȃ��܂��B");
					discard->id = NUM_OF_TILES_IN_HAND - 1; // fallback
				}
			}
			lua_pop(status[gameStat->CurrentPlayer.Active].state, 2);
			*finished = true; return S_OK;
		}
	} else {
		warn("�X�N���v�g�����[�h����Ă��Ȃ����A�g�p�ł��Ȃ����߁A�c���؂�Ƃ݂Ȃ��܂�");
		*discard = DiscardThrough; *finished = true; return S_OK; // �X�N���v�g�͎g�p�s�\
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
	o << "AI�̕��I����ɓ���܂��B�v���C���[ [" << (int)gameStat->CurrentPlayer.Passive << "]";
	info(o.str().c_str());
	gameStat->Player[gameStat->CurrentPlayer.Passive].DeclarationFlag.Chi = 0; // ���Z�b�g
	gameStat->Player[gameStat->CurrentPlayer.Passive].DeclarationFlag.Pon =
		gameStat->Player[gameStat->CurrentPlayer.Passive].DeclarationFlag.Kan =
		gameStat->Player[gameStat->CurrentPlayer.Passive].DeclarationFlag.Ron = false;
	if (status[gameStat->CurrentPlayer.Passive].scriptLoaded) { /* �������ǂݍ��܂�Ă���Ȃ� */
		try { /* determine_discard ������΂悵�A�Ȃ��������O���� */
			lua_getglobal(status[gameStat->CurrentPlayer.Passive].state, fncname_call[gameStat->KangFlag.chankanFlag]);
		} catch (...) { /* determine_discard ���Ȃ�������G���[�ɂȂ�̂ŗ�O���������� */
			if (gameStat->KangFlag.chankanFlag) {
				std::ostringstream o;
				o << "�O���[�o���V���{�� [" << fncname_call[0] << "] �̎擾�Ɏ��s���܂���"; error(o.str().c_str());
				info("���̃X�N���v�g�͎g�p�ł��܂���B�f�t�H���gAI(�c���؂�)�ɐ؂�ւ��܂��B");
				status[gameStat->CurrentPlayer.Passive].scriptLoaded = false; return S_OK;
			} else {
				std::ostringstream o;
				o << "�O���[�o���V���{�� [" << fncname_call[gameStat->KangFlag.chankanFlag] <<
					"] �̎擾�Ɏ��s���܂����B�������܂��B";
				warn(o.str().c_str());
				*finished = true; return S_OK;
			}
		}
		GameStatToLuaTable(status[gameStat->CurrentPlayer.Passive].state, gameStat);
		if (int errcode = lua_pcall(status[gameStat->CurrentPlayer.Passive].state, 1, 2, 0)) {
			/* ���s���s�I */
			std::ostringstream o;
			switch (errcode) {
			case LUA_ERRRUN:
				o << "�X�N���v�g�̎��s���G���[ [" <<
					lua_tostring(status[gameStat->CurrentPlayer.Passive].state, -1) /* �G���[���b�Z�[�W */ <<
					"]";
				lua_pop(status[gameStat->CurrentPlayer.Passive].state, 1);
				break;
			case LUA_ERRMEM: o << "�������̊����Ɏ��s���܂����B"; break;
			case LUA_ERRERR: o << "���b�Z�[�W�n���h�����s���̃G���[�ł��B"; break;
			case LUA_ERRGCMM: o << "�K�[�x�W�R���N�V�������s���̃G���[�ł��B"; break;
			}
			error(o.str().c_str());
			warn("�֐��Ăяo���Ɏ��s�������߁A�������܂�");
			*finished = true; return S_OK;
		} else {
			/* ���s���� */
			int flag = 0;
			MeldCallID meldtype = (MeldCallID)lua_tointegerx(status[gameStat->CurrentPlayer.Passive].state, -2, &flag);
			if (!flag) {
				warn("1�Ԗڂ̕Ԃ�l�����l�ł͂���܂���B�������܂��B");
			} else {
				switch (meldtype) {
					case meldNone: break;
					case meldRon: gameStat->Player[gameStat->CurrentPlayer.Passive].DeclarationFlag.Ron = true; break;
					case meldKan: gameStat->Player[gameStat->CurrentPlayer.Passive].DeclarationFlag.Kan = true; break;
					case meldPon: gameStat->Player[gameStat->CurrentPlayer.Passive].DeclarationFlag.Pon = true; break;
					case meldChiiLower: gameStat->Player[gameStat->CurrentPlayer.Passive].DeclarationFlag.Chi = 1; break;
					case meldChiiMiddle: gameStat->Player[gameStat->CurrentPlayer.Passive].DeclarationFlag.Chi = 2; break;
					case meldChiiUpper: gameStat->Player[gameStat->CurrentPlayer.Passive].DeclarationFlag.Chi = 3; break;
					default: warn("1�Ԗڂ̕Ԃ�l������������܂���B�������܂��B"); break;
				}
			}
			lua_pop(status[gameStat->CurrentPlayer.Passive].state, 1);
			*finished = true; return S_OK;
		}
	} else {
		warn("�X�N���v�g�����[�h����Ă��Ȃ����A�g�p�ł��Ȃ����߁A�������܂�");
		*finished = true; return S_OK; // �X�N���v�g�͎g�p�s�\
	}
}
