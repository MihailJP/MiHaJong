#include "aiscript.h"

lua_State* aiscript::lsMJCore = NULL;
bool aiscript::scriptLoaded = false;
const DiscardTileNum aiscript::DiscardThrough = {DiscardTileNum::Normal, NUM_OF_TILES_IN_HAND - 1};

const char* const aiscript::fncname_discard = "determine_discard"; // �̔v����p�֐��̖��O

void aiscript::initscript() {
	// Lua������ (��)
	lsMJCore = luaL_newstate();
	luaopen_base(lsMJCore); // base���C�u���������͊J���Ă����܂��傤
	const char* filename = ".\\ai\\ai.lua"; /* �t�@�C�����͉� */
	if (int errcode = luaL_loadfile(lsMJCore, filename)) { /* �t�@�C����ǂݍ��݁B����������0��Ԃ� */
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
		scriptLoaded = true;
	}
}

void aiscript::GameStatToLuaTable(lua_State *L, const GameTable* const gameStat) {
	debug("C++�\���́�Lua�e�[�u���ϊ��J�n");
	lua_newtable(L); // main table

	lua_newtable(L); // table of player table
	for (int p = 0; p < PLAYERS; p++)
		table::PlayerTbl(L, &gameStat->Player[p], (PLAYER_ID)p);
	lua_setfield(L, -2, "Player");

	debug("C++�\���́�Lua�e�[�u���ϊ��I��");
}

DiscardTileNum aiscript::compdahai(const GameTable* const gameStat) {
	std::ostringstream o;
	o << "AI�̑Ŕv�����ɓ���܂��B�v���C���[ [" << gameStat->CurrentPlayer.Active << "]";
	info(o.str().c_str());
	if (scriptLoaded) { /* �������ǂݍ��܂�Ă���Ȃ� */
		try { /* determine_discard ������΂悵�A�Ȃ��������O���� */
			lua_getglobal(lsMJCore, fncname_discard);
		} catch (...) { /* determine_discard ���Ȃ�������G���[�ɂȂ�̂ŗ�O���������� */
			std::ostringstream o;
			o << "�O���[�o���V���{�� [" << fncname_discard << "] �̎擾�Ɏ��s���܂���"; error(o.str().c_str());
			info("���̃X�N���v�g�͎g�p�ł��܂���B�f�t�H���gAI(�c���؂�)�ɐ؂�ւ��܂��B");
			scriptLoaded = false; return DiscardThrough;
		}
		GameStatToLuaTable(lsMJCore, gameStat);
		if (int errcode = lua_pcall(lsMJCore, 1, 2, 0)) {
			/* ���s���s�I */
			std::ostringstream o;
			switch (errcode) {
			case LUA_ERRRUN:
				o << "�X�N���v�g�̎��s���G���[ [";
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
			discard.type = (DiscardTileNum::discardType)lua_tointegerx(lsMJCore, -2, &flag);
			if (!flag) {
				warn("1�Ԗڂ̕Ԃ�l�����l�ł͂���܂���B�ʏ�̑Ŕv�Ƃ݂Ȃ��܂��B");
				discard.type = DiscardTileNum::Normal; // fallback
			}
			discard.id = lua_tointegerx(lsMJCore, -1, &flag);
			if (!flag) {
				warn("2�Ԗڂ̕Ԃ�l�����l�ł͂���܂���B�c���؂�Ƃ݂Ȃ��܂��B");
				discard.id = NUM_OF_TILES_IN_HAND - 1; // fallback
			}
			lua_pop(lsMJCore, 2);
			return discard;
		}
	} else {
		warn("�X�N���v�g�����[�h����Ă��Ȃ����A�g�p�ł��Ȃ����߁A�c���؂�Ƃ݂Ȃ��܂�");
		return DiscardThrough; // �X�N���v�g�͎g�p�s�\
	}
}

inline void aiscript::table::TableAdd(lua_State *L, std::string key, lua_Integer val) {
	lua_pushnumber(L, val);
	lua_setfield(L, -2, key.c_str());
}
inline void aiscript::table::TableAdd(lua_State *L, std::string key, lua_Number val) {
	lua_pushnumber(L, val);
	lua_setfield(L, -2, key.c_str());
}
inline void aiscript::table::TableAdd(lua_State *L, std::string key, bool val) {
	lua_pushboolean(L, val);
	lua_setfield(L, -2, key.c_str());
}
inline void aiscript::table::TableAdd(lua_State *L, std::string key, const TILE val) {
	lua_newtable(L);
	TableAdd(L, "tile", (lua_Integer)val.tile);
	TableAdd(L, "red", (lua_Integer)val.red);
	lua_setfield(L, -2, key.c_str());
}
inline void aiscript::table::TableAdd(lua_State *L, std::string key, const meldCode val) {
	lua_newtable(L);
	TableAdd(L, "tile", (lua_Integer)val.tile);
	{
		lua_newtable(L);
		for (int i = 0; i < 4; i++) {
			std::ostringstream o; o << i;
			TableAdd(L, o.str(), (lua_Integer)val.red[i]);
		}
		lua_setfield(L, -2, "red");
	}
	TableAdd(L, "mstat", (lua_Integer)val.mstat);
	lua_setfield(L, -2, key.c_str());
}

inline void aiscript::table::PlayerTbl(lua_State *L, const PlayerTable* const plStat, PLAYER_ID player) {
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

inline void aiscript::table::pltable::PlayerHand(lua_State *L, const HAND_TILES* const plHand) {
	lua_newtable(L);
	for (int i = 0; i < NUM_OF_TILES_IN_HAND; i++) {
		std::ostringstream o; o << (i + 1);
		TableAdd(L, o.str(), plHand[i]);
	}
	lua_setfield(L, -2, "Hand");
}
inline void aiscript::table::pltable::PlayerDiscard(lua_State *L, const DISCARD_BUF* const plDiscard, uint8_t DiscardPointer) {
	lua_newtable(L);
	for (uint8_t i = 1; i <= DiscardPointer; i++) {
		std::ostringstream o; o << (int)i;
		TableAdd(L, o.str(), plDiscard[i]);
	}
	lua_setfield(L, -2, "Discard");
}
inline void aiscript::table::pltable::PlayerMeld(lua_State *L, const MELD_BUF* const plMeld, uint8_t MeldPointer) {
	lua_newtable(L);
	for (uint8_t i = 1; i <= MeldPointer; i++) {
		std::ostringstream o; o << (int)i;
		TableAdd(L, o.str(), plMeld[i]);
	}
	lua_setfield(L, -2, "Meld");
}
inline void aiscript::table::pltable::PlayerRichiStat(lua_State *L, const RichiStat* const plstat) {
	lua_newtable(L);
	TableAdd(L, "RichiFlag", plstat->RichiFlag);
	TableAdd(L, "IppatsuFlag", plstat->IppatsuFlag);
	TableAdd(L, "DoubleFlag", plstat->DoubleFlag);
	TableAdd(L, "OpenFlag", plstat->OpenFlag);
	lua_setfield(L, -2, "RichiFlag");
}
inline void aiscript::table::pltable::PlayerFlower(lua_State *L, const FLOWERS* const plstat) {
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
inline void aiscript::table::pltable::PlayerDeclFlag(lua_State *L, const DECLFLAG* const plstat) {
	lua_newtable(L);
	TableAdd(L, "Ron", plstat->Ron);
	TableAdd(L, "Kan", plstat->Kan);
	TableAdd(L, "Pon", plstat->Pon);
	TableAdd(L, "Chi", plstat->Chi);
	lua_setfield(L, -2, "DeclarationFlag");
}
