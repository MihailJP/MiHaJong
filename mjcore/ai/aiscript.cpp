#include "aiscript.h"

const DiscardTileNum aiscript::DiscardThrough = {DiscardTileNum::Normal, NUM_OF_TILES_IN_HAND - 1};

aiscript::ScriptStates aiscript::status[PLAYERS] = {{NULL, false}};
const char* const aiscript::fncname_discard = "determine_discard"; // �̔v����p�֐��̖��O
const char* const aiscript::fncname_call = "determine_call"; // ������p�֐��̖��O

__declspec(dllexport) void aiscript::initscript() {
	// Lua������ (��)
	for (int i = 0; i < PLAYERS; i++) {
		status[i].state = luaL_newstate();
		luaopen_base(status[i].state); // base���C�u���������͊J���Ă����܂��傤
		table::functable::inittable(status[i].state, i);
		const char* filename = ".\\ai\\ai.lua"; /* �t�@�C�����͉� */
		readfile(&status[i], filename); /* �t�@�C����ǂݍ��� */
	}
}

__declspec(dllexport) void aiscript::initephemeral() {
	// ephemeral�e�[�u��������
	for (int i = 0; i < PLAYERS; i++) {
		lua_newtable(status[i].state);
		lua_setglobal(status[i].state, "ephemeral");
	}
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

__declspec(dllexport) int aiscript::compdahai(const GameTable* const gameStat) {
	return determine_discard(gameStat).toSingleInt();
}
DiscardTileNum aiscript::determine_discard(const GameTable* const gameStat) {
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
			status[gameStat->CurrentPlayer.Active].scriptLoaded = false; return DiscardThrough;
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
			return DiscardThrough;
		} else {
			/* ���s���� */
			DiscardTileNum discard; int flag;
			discard.type = (DiscardTileNum::discardType)lua_tointegerx(status[gameStat->CurrentPlayer.Active].state, -2, &flag);
			if (!flag) {
				warn("1�Ԗڂ̕Ԃ�l�����l�ł͂���܂���B�ʏ�̑Ŕv�Ƃ݂Ȃ��܂��B");
				discard.type = DiscardTileNum::Normal; // fallback
			} else if ((discard.type < DiscardTileNum::Normal) || (discard.type > DiscardTileNum::Disconnect)) {
				warn("1�Ԗڂ̕Ԃ�l������������܂���B�ʏ�̑Ŕv�Ƃ݂Ȃ��܂��B");
				discard.type = DiscardTileNum::Normal; // fallback
			}
			if ((discard.type == DiscardTileNum::Agari) || (discard.type == DiscardTileNum::Kyuushu) ||
				(discard.type == DiscardTileNum::Disconnect)) { // �ԍ��w�肪�s�v�ȏꍇ
					discard.id = NUM_OF_TILES_IN_HAND - 1; // 2�Ԃ߂̕Ԃ�l�͖���
			} else {
				int i = lua_tointegerx(status[gameStat->CurrentPlayer.Active].state, -1, &flag);
				if (!flag) {
					warn("2�Ԗڂ̕Ԃ�l�����l�ł͂���܂���B�c���؂�Ƃ݂Ȃ��܂��B");
					discard.id = NUM_OF_TILES_IN_HAND - 1; // fallback
				} else if ((i >= 1)&&(i <= NUM_OF_TILES_IN_HAND)) {
					discard.id = i - 1; // �I���W����1�ɂ���d�l�c�c
				} else if ((i <= -1)&&(i >= -NUM_OF_TILES_IN_HAND)) { // �}�C�i�X���w�肵���ꍇ�̏���
					discard.id = NUM_OF_TILES_IN_HAND + i;
				} else {
					warn("2�Ԗڂ̕Ԃ�l���͈͊O�ł��B�c���؂�Ƃ݂Ȃ��܂��B");
					discard.id = NUM_OF_TILES_IN_HAND - 1; // fallback
				}
			}
			lua_pop(status[gameStat->CurrentPlayer.Active].state, 2);
			return discard;
		}
	} else {
		warn("�X�N���v�g�����[�h����Ă��Ȃ����A�g�p�ł��Ȃ����߁A�c���؂�Ƃ݂Ȃ��܂�");
		return DiscardThrough; // �X�N���v�g�͎g�p�s�\
	}
}

#if 0
inline void aiscript::table::TableAdd(lua_State* const L, const char* const key, const TILE val) {
	lua_newtable(L);
	TableAdd(L, "tile", (lua_Integer)val.tile);
	TableAdd(L, "red", (lua_Integer)val.red);
	lua_setfield(L, -2, key);
}
inline void aiscript::table::TableAdd(lua_State* const L, const char* const key, const meldCode val) {
	lua_newtable(L);
	TableAdd(L, "tile", (lua_Integer)val.tile);
	{
		lua_newtable(L);
		for (int i = 0; i < 4; i++) {
			std::ostringstream o; o << i;
			TableAdd(L, o.str().c_str(), (lua_Integer)val.red[i]);
		}
		lua_setfield(L, -2, "red");
	}
	TableAdd(L, "mstat", (lua_Integer)val.mstat);
	lua_setfield(L, -2, key);
}

inline void aiscript::table::playertable::PlayerTbl(lua_State* const L, const PlayerTable* const plStat, PLAYER_ID player) {
	lua_newtable(L); // player table
	TableAdd(L, "PlayerScore", plStat->PlayerScore.bignumtodbl());
	TableAdd(L, "PlayerChip", plStat->playerChip);
	TableAdd(L, "SumaroFlag", plStat->SumaroFlag);
	TableAdd(L, "YakitoriFlag", plStat->YakitoriFlag);
	pltable::PlayerHand(L, &plStat->Hand);
	pltable::PlayerDiscard(L, &plStat->Discard, plStat->DiscardPointer);
	TableAdd(L, "DiscardPointer", plStat->DiscardPointer);
	pltable::PlayerMeld(L, &plStat->Meld, plStat->MeldPointer);
	TableAdd(L, "MeldPointer", plStat->MeldPointer);
	TableAdd(L, "MenzenFlag", plStat->MenzenFlag);
	TableAdd(L, "HandStat", plStat->HandStat);
	TableAdd(L, "NumberOfQuads", plStat->NumberOfQuads);
	TableAdd(L, "MenzenFlag", plStat->MenzenFlag);
	pltable::PlayerRichiStat(L, &plStat->RichiFlag);
	TableAdd(L, "FirstDrawFlag", plStat->FirstDrawFlag);
	TableAdd(L, "DoujunFuriten", plStat->DoujunFuriten);
	TableAdd(L, "AgariHouki", plStat->AgariHouki);
	pltable::PlayerFlower(L, &plStat->FlowerFlag);
	TableAdd(L, "NorthFlag", plStat->NorthFlag);
	TableAdd(L, "ConnectionLost", plStat->ConnectionLost);
	pltable::PlayerDeclFlag(L, &plStat->DeclarationFlag);
	std::ostringstream o; o << (int)(player + 1);
	lua_setfield(L, -2, o.str().c_str());
}

inline void aiscript::table::playertable::pltable::PlayerHand(lua_State* const L, const HAND_TILES* const plHand) {
	lua_newtable(L);
	for (int i = 0; i < NUM_OF_TILES_IN_HAND; i++) {
		std::ostringstream o; o << (i + 1);
		TableAdd(L, o.str().c_str(), plHand[i]);
	}
	lua_setfield(L, -2, "Hand");
}
inline void aiscript::table::playertable::pltable::PlayerDiscard(lua_State* const L, const DISCARD_BUF* const plDiscard, uint8_t DiscardPointer) {
	lua_newtable(L);
	for (uint8_t i = 1; i <= DiscardPointer; i++) {
		std::ostringstream o; o << (int)i;
		TableAdd(L, o.str().c_str(), plDiscard[i]);
	}
	lua_setfield(L, -2, "Discard");
}
inline void aiscript::table::playertable::pltable::PlayerMeld(lua_State* const L, const MELD_BUF* const plMeld, uint8_t MeldPointer) {
	lua_newtable(L);
	for (uint8_t i = 1; i <= MeldPointer; i++) {
		std::ostringstream o; o << (int)i;
		TableAdd(L, o.str().c_str(), plMeld[i]);
	}
	lua_setfield(L, -2, "Meld");
}
inline void aiscript::table::playertable::pltable::PlayerRichiStat(lua_State* const L, const RichiStat* const plstat) {
	lua_newtable(L);
	TableAdd(L, "RichiFlag", plstat->RichiFlag);
	TableAdd(L, "IppatsuFlag", plstat->IppatsuFlag);
	TableAdd(L, "DoubleFlag", plstat->DoubleFlag);
	TableAdd(L, "OpenFlag", plstat->OpenFlag);
	lua_setfield(L, -2, "RichiFlag");
}
inline void aiscript::table::playertable::pltable::PlayerFlower(lua_State* const L, const FLOWERS* const plstat) {
	lua_newtable(L);
	TableAdd(L, "Spring", plstat->Spring);
	TableAdd(L, "Summer", plstat->Summer);
	TableAdd(L, "Autumn", plstat->Autumn);
	TableAdd(L, "Winter", plstat->Winter);
	TableAdd(L, "Plum", plstat->Plum);
	TableAdd(L, "Orchid", plstat->Orchid);
	TableAdd(L, "Chrys", plstat->Chrys);
	TableAdd(L, "Bamboo", plstat->Bamboo);
	lua_setfield(L, -2, "FlowerFlag");
}
inline void aiscript::table::playertable::pltable::PlayerDeclFlag(lua_State* const L, const DECLFLAG* const plstat) {
	lua_newtable(L);
	TableAdd(L, "Ron", plstat->Ron);
	TableAdd(L, "Kan", plstat->Kan);
	TableAdd(L, "Pon", plstat->Pon);
	TableAdd(L, "Chi", plstat->Chi);
	lua_setfield(L, -2, "DeclarationFlag");
}
#endif
