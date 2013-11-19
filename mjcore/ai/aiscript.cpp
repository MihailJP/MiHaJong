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
const char aiscript::fncname_discard[8] = "ontsumo"; // �̔v����p�֐��̖��O
const char aiscript::fncname_call[3][12] = {"ondiscard", "onkakan", "onankan",}; // ������p�֐��̖��O

bool aiscript::callFunc(const GameTable* const gameStat, PlayerID PlayerID, const char* const function_name, bool is_mandatory) {
	if (status[PlayerID].scriptLoaded) { /* �������ǂݍ��܂�Ă���Ȃ� */
		try { /* �V���{��������΂悵�A�Ȃ��������O���� */
			lua_getglobal(status[PlayerID].state, function_name);
		} catch (...) { /* �V���{�����Ȃ�������G���[�ɂȂ�̂ŗ�O���������� */
			CodeConv::tostringstream o;
			if (is_mandatory) {
				o << _T("�O���[�o���V���{�� [") << CodeConv::EnsureTStr(function_name) << _T("] �̎擾�Ɏ��s���܂���"); error(o.str().c_str());
				info(_T("���̃X�N���v�g�͎g�p�ł��܂���B�f�t�H���gAI(�c���؂�)�ɐ؂�ւ��܂��B"));
				status[PlayerID].scriptLoaded = false;
				chat::chatobj->sysmsg(CodeConv::tstring(_T("*** ")) + o.str());
				return true;
			} else {
				o << _T("�O���[�o���V���{�� [") << CodeConv::EnsureTStr(function_name) << _T("] �̎擾�Ɏ��s���܂����B�������܂��B");
				warn(o.str().c_str());
				return true;
			}
		}
		GameStatToLuaTable(status[PlayerID].state, gameStat);
		if (int errcode = lua_pcall(status[PlayerID].state, 1, 2, 0)) {
			/* ���s���s�I */
			CodeConv::tostringstream o;
			switch (errcode) {
			case LUA_ERRRUN:
				o << _T("�X�N���v�g�̎��s���G���[ [") <<
					CodeConv::DecodeStr(lua_tostring(status[PlayerID].state, -1)) /* �G���[���b�Z�[�W */ <<
					_T("]");
				lua_pop(status[PlayerID].state, 1);
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

void aiscript::initscript() {
	info(_T("AI�p�̃X�N���v�g�����������܂�"));
	FileSelector::filelist();
	// Lua������
	for (int i = 0; i < Players; i++) {
		status[i].state = luaL_newstate();
		luaopen_base(status[i].state); // base���C�u���������͊J���Ă����܂��傤
		table::functable::inittable(status[i].state, i);
		readfile(&status[i], FileSelector::randomfile().c_str()); /* �t�@�C����ǂݍ��� */
	}
	info(_T("�X�N���v�g�̏��������������܂���"));
}

void aiscript::initephemeral() {
	// ephemeral�e�[�u��������
	for (int i = 0; i < Players; i++) {
		lua_newtable(status[i].state);
		lua_setglobal(status[i].state, "ephemeral");
	}
	debug(_T("ephemeral �e�[�u�������������܂���"));
}

void aiscript::initcall(const GameTable* const gameStat, PlayerID player) {
	callFunc(gameStat, player, "init", false);
}

void aiscript::closescript() {
	// Lua�N�����i�b�v
	for (int i = 0; i < Players; i++) {
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
	o << _T("AI�̑Ŕv�����ɓ���܂��B�v���C���[ [") << (int)gameStat->CurrentPlayer.Active << _T("]");
	info(o.str().c_str());
	if (callFunc(gameStat, gameStat->CurrentPlayer.Active, fncname_discard, true)) {
		answer = DiscardThrough;
		return;
	} else {
		/* ���s���� */
		int flag;
		answer.type = (DiscardTileNum::discardType)lua_tointegerx(status[gameStat->CurrentPlayer.Active].state, -2, &flag);
		if (!flag) {
			warn(_T("1�Ԗڂ̕Ԃ�l�����l�ł͂���܂���B�ʏ�̑Ŕv�Ƃ݂Ȃ��܂��B"));
			answer.type = DiscardTileNum::Normal; // fallback
		} else if ((answer.type < DiscardTileNum::Normal) || (answer.type > DiscardTileNum::Disconnect)) {
			warn(_T("1�Ԗڂ̕Ԃ�l������������܂���B�ʏ�̑Ŕv�Ƃ݂Ȃ��܂��B"));
			answer.type = DiscardTileNum::Normal; // fallback
		}
		if ((answer.type == DiscardTileNum::Agari) || (answer.type == DiscardTileNum::Kyuushu) ||
			(answer.type == DiscardTileNum::Disconnect)) { // �ԍ��w�肪�s�v�ȏꍇ
				answer.id = NumOfTilesInHand - 1; // 2�Ԃ߂̕Ԃ�l�͖���
		} else {
			int i = lua_tointegerx(status[gameStat->CurrentPlayer.Active].state, -1, &flag);
			if (!flag) {
				warn(_T("2�Ԗڂ̕Ԃ�l�����l�ł͂���܂���B�c���؂�Ƃ݂Ȃ��܂��B"));
				answer.id = NumOfTilesInHand - 1; // fallback
			} else if ((i >= 1)&&(i <= NumOfTilesInHand)) {
				answer.id = i - 1; // �I���W����1�ɂ���d�l�c�c
			} else if ((i <= -1)&&(i >= -((int)NumOfTilesInHand))) { // �}�C�i�X���w�肵���ꍇ�̏���
				answer.id = NumOfTilesInHand + i;
			} else {
				warn(_T("2�Ԗڂ̕Ԃ�l���͈͊O�ł��B�c���؂�Ƃ݂Ȃ��܂��B"));
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
	o << _T("AI�̕��I����ɓ���܂��B�v���C���[ [") << (int)gameStat->CurrentPlayer.Passive << _T("]");
	info(o.str().c_str());
	gameStat->statOfPassive().DeclarationFlag.Chi = chiiNone; // ���Z�b�g
	gameStat->statOfPassive().DeclarationFlag.Pon =
		gameStat->statOfPassive().DeclarationFlag.Kan =
		gameStat->statOfPassive().DeclarationFlag.Ron = false;
	if (callFunc(gameStat, gameStat->CurrentPlayer.Passive, fncname_call[gameStat->KangFlag.chankanFlag], (gameStat->KangFlag.chankanFlag == 0))) {
		return;
	} else {
		/* ���s���� */
		int flag = 0;
		MeldCallID meldtype = (MeldCallID)lua_tointegerx(status[gameStat->CurrentPlayer.Passive].state, -2, &flag);
		if (!flag) {
			warn(_T("1�Ԗڂ̕Ԃ�l�����l�ł͂���܂���B�������܂��B"));
		} else {
			switch (meldtype) {
				case meldNone: break;
				case meldRon: gameStat->statOfPassive().DeclarationFlag.Ron = true; break;
				case meldKan: gameStat->statOfPassive().DeclarationFlag.Kan = true; break;
				case meldPon: gameStat->statOfPassive().DeclarationFlag.Pon = true; break;
				case meldChiiLower: gameStat->statOfPassive().DeclarationFlag.Chi = chiiLower; break;
				case meldChiiMiddle: gameStat->statOfPassive().DeclarationFlag.Chi = chiiMiddle; break;
				case meldChiiUpper: gameStat->statOfPassive().DeclarationFlag.Chi = chiiUpper; break;
				default: warn(_T("1�Ԗڂ̕Ԃ�l������������܂���B�������܂��B")); break;
			}
		}
		lua_pop(status[gameStat->CurrentPlayer.Passive].state, 1);
		return;
	}
}
