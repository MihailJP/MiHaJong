#include "gametbl.h"

#ifdef MJCORE_EXPORTS
#include <algorithm>
#include <cassert>
#endif
#include "regex.h"
#include "except.h"
#include "func.h"
#include "tileutil.h"
#include "ruletbl.h"
#include "largenum.h"

using namespace mihajong_structs;

GameTable GameStat, StatSandBox;

void calcWareme(GameTable* const gameStat) {
#ifndef GUOBIAO
	assert((gameStat == &GameStat)||(gameStat == &StatSandBox));
	if (gameStat->chkGameType(AllSanma)) {
		if (RuleData::chkRuleApplied("wareme") || RuleData::chkRuleApplied("kaimenkaze")) {
			gameStat->WaremePlayer = ((gameStat->GameRound-(gameStat->GameRound/4))+24
				+gameStat->diceSum()-1) % 3;
			if (gameStat->chkGameType(Sanma4)) {
				gameStat->WaremePlayer =
					tobePlayed(gameStat, (24+gameStat->diceSum()-1) % 3);
			}
		}
	} else {
		if (RuleData::chkRuleApplied("wareme") || RuleData::chkRuleApplied("kaimenkaze"))
			gameStat->WaremePlayer = ((gameStat->GameRound % 4)+32+gameStat->diceSum()-1) % 4;
	}
#endif /* GUOBIAO */
	return;
}


/* ---------------------------------------------------------------------
**  ����������
** --------------------------------------------------------------------- */

void resetDeclarationFlag(GameTable* const gameStat) {
	assert((gameStat == &GameStat)||(gameStat == &StatSandBox));
	for (int pl = 0; pl < Players; pl++) {
		gameStat->Player[pl].DeclarationFlag.Ron =
			gameStat->Player[pl].DeclarationFlag.Pon =
			gameStat->Player[pl].DeclarationFlag.Kan = false;
		gameStat->Player[pl].DeclarationFlag.Chi = chiiNone;
	}
	return;
}

void inittable(GameTable* const gameStat) { /* �ǒP�ʂł̏����� */
	assert((gameStat == &GameStat)||(gameStat == &StatSandBox));
#ifdef GUOBIAO
	gameStat->ShibariFlag = false;
#else /* GUOBIAO */
	gameStat->ShibariFlag = //���ʔ���
		((gameStat->Honba >= 5)&&(RuleData::chkRule("ryanshiba", "from_5honba"))) ||
		((gameStat->Honba >= 4)&&(RuleData::chkRule("ryanshiba", "from_4honba")));
#endif /* GUOBIAO */

	for (int i = 0; i < PaoYakuPages; i++) // ��t���O�i-1�c�Ȃ��A0�`3�c�Y���v���C���[�j
		gameStat->PaoFlag[i].agariPlayer = gameStat->PaoFlag[i].paoPlayer = -1;

	for (int i = 0; i < SizeOfDeckBuf; i++) { // �����Ə��������Ă����܂��傤��
		gameStat->Deck[i].tile = NoTile;
		gameStat->Deck[i].red = Normal;
	}

#ifdef GUOBIAO
	gameStat->DeadTiles = gameStat->ExtraRinshan = 0; // ���v�Ȃ�Ă���Ǝv�����H�@�c�O�I
#else /* GUOBIAO */
	if (gameStat->chkGameType(AllSanma)) {
		gameStat->DeadTiles = 14; // ���v�̐�
		gameStat->ExtraRinshan = RuleData::chkRuleApplied("flower_tiles") ? 4 : 0;
	} else {
		if (RuleData::chkRule("flower_tiles", "no")) gameStat->DeadTiles = 14; // ���v�̐�
		else if (RuleData::chkRule("flower_tiles", "8tiles")) gameStat->DeadTiles = 22; // ���v�̐�(�Ԕv�����鎞�͓��ʂɂQ�Q���c���Ƃ���)
		else gameStat->DeadTiles = 18; // ���v�̐�
	}
#endif /* GUOBIAO */

	for (int i = 0; i < TileNonflowerMax; i++) // �v�����[�̑҂��v(�b�n�l�ɈӐ}�I�ȕ��e���N�������Ȃ����߂Ɏg�p)
		gameStat->OpenRichiWait[i] = false;
	gameStat->KangFlag.kangFlag = gameStat->KangFlag.topFlag = false; // ���J�ԁG���Șa�G�A�J�ԂƞȐU��G���Ȃ̔���Ɏg��
	gameStat->KangFlag.chainFlag = 0;
	gameStat->KangFlag.chankanFlag = chankanNone;
	gameStat->TurnRound =  // ���݂̏���
		gameStat->KangNum = 0; // �l�ȗ��ǁA�l�Ȏq�Ȃǂ̔���Ɏg��
	gameStat->RichiCounter =
		gameStat->DoubleRichiCounter = false; // ���[�`���J�E���^�[(�錾�v������)
	gameStat->WaremePlayer = // ����ڂ̈ʒu(-1�Ŋ���ڂȂ�)
		gameStat->DoukasenPlayer = -1; // ���ΐ��̈ʒu(-1�œ��ΐ��Ȃ�)
	gameStat->DoraPointer = 999;
	gameStat->Dice[0].Number = gameStat->Dice[1].Number = 0;
	gameStat->Dice[0].Direction = gameStat->Dice[1].Direction = 0;
	gameStat->TilePointer = 0;

#ifdef GUOBIAO
	gameStat->RinshanPointer = 143;
#else /* GUOBIAO */
	if (gameStat->chkGameType(AllSanma)) {
		gameStat->RinshanPointer = 107;
	} else {
		if (RuleData::chkRule("flower_tiles", "no")) gameStat->RinshanPointer = 135;
		else if ((RuleData::chkRule("flower_tiles", "seasons")) || (RuleData::chkRule("flower_tiles", "flowers")))
			gameStat->RinshanPointer = 139;
		else if (RuleData::chkRule("flower_tiles", "8tiles")) gameStat->RinshanPointer = 143;
		else {
			error(_T("flower_tiles�ُ�B�Ԕv�������[���Ƃ݂Ȃ��ď������܂��B"));
			gameStat->RinshanPointer = 135; // �ݒ�ُ펞�̃t�H�[���o�b�N
		}
	}
#endif /* GUOBIAO */

	gameStat->TianHuFlag = true; // �e�̑��Ŕv���܂��i�V�a�̔���ȂǂɎg���j
	gameStat->PreviousMeld.Discard = // ��قǖ����v�i�򂢑ւ��̔���Ɏg���j
		gameStat->PreviousMeld.Stepped = NoTile;
	for (int i = 0; i < TileNonflowerMax; i++) // �h������̔z��
		gameStat->DoraFlag.Omote[i] = gameStat->DoraFlag.Ura[i] = 0;
	gameStat->TsumoAgariFlag = false;
	gameStat->AgariSpecialStat = 0;
	resetDeclarationFlag(gameStat);
	gameStat->CurrentDiscard.tile = NoTile;
	gameStat->CurrentDiscard.red = Normal;
	gameStat->CurrentPlayer.Active = gameStat->CurrentPlayer.Passive =
		gameStat->CurrentPlayer.Agari = gameStat->CurrentPlayer.Furikomi = (PlayerID)-1;

	for (int pl = 0; pl < Players; pl++) {
		gameStat->Player[pl].ConnectionLost = false; // ����ؒf�ɂ��a�������
		for (int i = 0; i < NumOfTilesInHand; i++) { // ��v�̔z��(�S�l��)
			gameStat->Player[pl].Hand[i].tile = NoTile;
			gameStat->Player[pl].Hand[i].red = Normal;
		}
		gameStat->Player[pl].DiscardPointer = 0; // �����ƃ��Z�b�g���Ă����܂��傤��
		for (int i = 0; i < SizeOfDiscardBuffer; i++) {
			// �̔v�̔z��(�S�l��)
			gameStat->Player[pl].Discard[i].tcode.tile = NoTile;
			gameStat->Player[pl].Discard[i].tcode.red = Normal;
			gameStat->Player[pl].Discard[i].dstat = discardNormal;
			gameStat->Player[pl].Discard[i].isDiscardThrough = false;
		}
		gameStat->Player[pl].MenzenFlag = true; // ��O�t���O
		gameStat->Player[pl].HandStat = handUpright; // ��v�̏�ԁi���Ă�E������E������j
		gameStat->Player[pl].MeldPointer = 0; // �ŏ��ςȐ����������Ă��肷���Łc�c
		for (int i = 0; i < SizeOfMeldBuffer; i++) {
			// ���ʎq���i�[
			gameStat->Player[pl].Meld[i].tile = NoTile;
			for (int j = 0; j < 4; j++) gameStat->Player[pl].Meld[i].red[j] = Normal;
			gameStat->Player[pl].Meld[i].mstat = (MeldStat)0;
		}
		gameStat->Player[pl].NumberOfQuads = 0; // �Ȏq�̐��i�l�ȗ��ǁA�O�Ȏq�A�l�Ȏq�Ȃǂ̔���Ɏg���j
		gameStat->Player[pl].RichiFlag.RichiFlag = // ���[�`���Ă��邩�ǂ���
			gameStat->Player[pl].RichiFlag.IppatsuFlag =
			gameStat->Player[pl].RichiFlag.DoubleFlag =
			gameStat->Player[pl].RichiFlag.OpenFlag = false;
		gameStat->Player[pl].FirstDrawFlag = true; // �P���ڂł���i�n�a�A�_�u�������̔���Ɏg���j
		gameStat->Player[pl].DoujunFuriten = // �����U���ł���
			gameStat->Player[pl].AgariHouki = false; // �a��������̔��������ǂ���
		gameStat->Player[pl].FlowerFlag.Spring = // �N���Ă���Ԕv���i�[����t���O
			gameStat->Player[pl].FlowerFlag.Summer =
			gameStat->Player[pl].FlowerFlag.Autumn =
			gameStat->Player[pl].FlowerFlag.Winter =
			gameStat->Player[pl].FlowerFlag.Plum =
			gameStat->Player[pl].FlowerFlag.Orchid =
			gameStat->Player[pl].FlowerFlag.Chrys =
			gameStat->Player[pl].FlowerFlag.Bamboo = false;
		gameStat->Player[pl].NorthFlag = 0; // �N���Ă���k���v���i�[����t���O
		gameStat->Player[pl].shokanFlag = // ���ȃt���O
			gameStat->Player[pl].kansanjunFlag = false; // �ȎO���t���O
		gameStat->Player[pl].renpaiTenhohStat = 0; // �ߔv�V�a����p
	}
	assert(gameStat->Player[0].DiscardPointer == 0); // �������ł��Ă邩�`�F�b�N�i�f�o�b�O�p�j
}

void doInitializeGameTable(GameTable* const gameStat, GameTypeID gameType) { // �����P�ʂ̏���������
	assert((gameStat == &GameStat)||(gameStat == &StatSandBox));
	/* ���������p�ŃG�N�X�|�[�g���Ȃ� */
	memset(gameStat, 0, sizeof(GameTable));
	gameStat->gameType = (GameTypeID)gameType;

#ifdef GUOBIAO
	for (int i = 0; i < Players; i++)
		gameStat->Player[i].PlayerScore = (LNum)500; // �����_500�_�Ƃ���
	gameStat->GameLength = 15; // �������[���͈ꑑ�s��
#else /* GUOBIAO */
	LNum initialPoints; /* �����_�� */
	if (RuleData::chkRule("starting_point", "custom")) {
		initialPoints = // ������
			std::atoi(RuleData::chkRule("starting_point_mantissa_tens")) * 10 +
			std::atoi(RuleData::chkRule("starting_point_mantissa_ones"));
		/* �w�����̏��� */
		REGEX::smatch matchDat; int exponent = 0;
		std::string expConf(RuleData::chkRule("starting_point_exponent"));
		if (REGEX::regex_match(expConf, matchDat, REGEX::regex("exp_(\\d+)")))
			exponent = atoi(matchDat[1].str().c_str()); // ���[���ݒ蕶���񂩂琮���𒊏o
		for (int j = 0; j < exponent; ++j)
			initialPoints *= 10;
	} else {
		initialPoints = (LNum)std::atoi(RuleData::chkRule("starting_point"));
	}

	for (int i = 0; i < Players; i++) {
		if (i < ACTUAL_PLAYERS)
			gameStat->Player[i].PlayerScore = initialPoints;
		else
			gameStat->Player[i].PlayerScore = (LNum)0;
	}

	if (RuleData::chkRule("game_length", "east_south_game"))
		gameStat->GameLength = GameStat.chkGameType(SanmaT) ? 6 : 7;
	else if (RuleData::chkRule("game_length", "east_wind_game") ||
		RuleData::chkRule("game_length", "east_only_game"))
		gameStat->GameLength = GameStat.chkGameType(SanmaT) ? 2 : 3;
	else if (RuleData::chkRule("game_length", "full_round_game") ||
		RuleData::chkRule("game_length", "east_north_game"))
		gameStat->GameLength = GameStat.chkGameType(SanmaT) ? 14 : 15;
	else if (RuleData::chkRule("game_length", "single_round_game"))
		gameStat->GameLength = 0;
	else if (RuleData::chkRule("game_length", "twice_east_game"))
		gameStat->GameLength = GameStat.chkGameType(SanmaT) ? 18 : 19;
	else if (RuleData::chkRule("game_length", "east_south_west_game") ||
		RuleData::chkRule("game_length", "east_west_game"))
		gameStat->GameLength = GameStat.chkGameType(SanmaT) ? 10 : 11;
	else {
		error(_T("game_length�ُ�l�B������Ƃ݂Ȃ��܂��B"));
		gameStat->GameLength = GameStat.chkGameType(SanmaT) ? 6 : 7;
	}
#endif /* GUOBIAO */
	gameStat->GameRound = gameStat->Honba = gameStat->PlayerID =
		gameStat->Deposit = gameStat->LoopRound = gameStat->AgariChain = 0;
	gameStat->LastAgariPlayer = -1;
	for (int i = 0; i < Players; i++) {
		gameStat->Player[i].SumaroFlag = false;
		gameStat->Player[i].YakitoriFlag = RuleData::chkRuleApplied("yakitori");
		gameStat->Player[i].playerChip = 0;
	}

	inittable(gameStat); // �ǂ��Ƃ̏��������s��
}

GameTable* initializeGameTable(GameTypeID gameType) { // �����P�ʂ̏���������
	doInitializeGameTable(&GameStat, gameType);
	return &GameStat;
}

// ---------------------------------------------------------------------

GameTable* makesandBox(const GameTable* const gameStat, PlayerID targetPlayer) {
	/* ��̏�Ԃ̃T���h�{�b�N�X����� */
	GameTable* const sandbox = &StatSandBox;
	doInitializeGameTable(sandbox, gameStat->gameType);
	for (int p = 0; p < Players; p++) {
		sandbox->Player[p].PlayerScore = (LNum)gameStat->Player[p].PlayerScore;
		sandbox->Player[p].playerChip = gameStat->Player[p].playerChip;
		sandbox->Player[p].SumaroFlag = gameStat->Player[p].SumaroFlag;
		sandbox->Player[p].shokanFlag = gameStat->Player[p].shokanFlag;
		sandbox->Player[p].kansanjunFlag = gameStat->Player[p].kansanjunFlag;
		sandbox->Player[p].YakitoriFlag = gameStat->Player[p].YakitoriFlag;
		if ((gameStat->Player[p].RichiFlag.OpenFlag)||(p == targetPlayer)) {
			for (int i = 0; i < NumOfTilesInHand; i++) {
				sandbox->Player[p].Hand[i].tile = gameStat->Player[p].Hand[i].tile;
				sandbox->Player[p].Hand[i].red = gameStat->Player[p].Hand[i].red;
			}
			sandbox->Player[p].renpaiTenhohStat = gameStat->Player[p].renpaiTenhohStat;
		}
		sandbox->Player[p].RichiFlag = gameStat->Player[p].RichiFlag;
		for (int i = 0; i < SizeOfDiscardBuffer; i++) {
			sandbox->Player[p].Discard[i].isDiscardThrough = gameStat->Player[p].Discard[i].isDiscardThrough;
			sandbox->Player[p].Discard[i].dstat = gameStat->Player[p].Discard[i].dstat;
			sandbox->Player[p].Discard[i].tcode.tile = gameStat->Player[p].Discard[i].tcode.tile;
			sandbox->Player[p].Discard[i].tcode.red = gameStat->Player[p].Discard[i].tcode.red;
		}
		sandbox->Player[p].DiscardPointer = gameStat->Player[p].DiscardPointer;
		for (int i = 0; i < SizeOfMeldBuffer; i++) {
			sandbox->Player[p].Meld[i].mstat = gameStat->Player[p].Meld[i].mstat;
			sandbox->Player[p].Meld[i].tile = gameStat->Player[p].Meld[i].tile;
			for (int j = 0; j < 4; j++)
				sandbox->Player[p].Meld[i].red[j] = gameStat->Player[p].Meld[i].red[j];
		}
		sandbox->Player[p].MeldPointer = gameStat->Player[p].MeldPointer;
		sandbox->Player[p].MenzenFlag = gameStat->Player[p].MenzenFlag;
		sandbox->Player[p].NumberOfQuads = gameStat->Player[p].NumberOfQuads;
		sandbox->Player[p].RichiFlag.RichiFlag = gameStat->Player[p].RichiFlag.RichiFlag;
		sandbox->Player[p].RichiFlag.IppatsuFlag = gameStat->Player[p].RichiFlag.IppatsuFlag;
		sandbox->Player[p].RichiFlag.DoubleFlag = gameStat->Player[p].RichiFlag.DoubleFlag;
		sandbox->Player[p].RichiFlag.OpenFlag = gameStat->Player[p].RichiFlag.OpenFlag;
		sandbox->Player[p].FirstDrawFlag = gameStat->Player[p].FirstDrawFlag;
		sandbox->Player[p].DoujunFuriten = gameStat->Player[p].DoujunFuriten;
		sandbox->Player[p].AgariHouki = gameStat->Player[p].AgariHouki;
		sandbox->Player[p].FlowerFlag = gameStat->Player[p].FlowerFlag;
		sandbox->Player[p].NorthFlag = gameStat->Player[p].NorthFlag;
		sandbox->Player[p].ConnectionLost = gameStat->Player[p].ConnectionLost;
	}
	sandbox->PlayerID = gameStat->PlayerID;
	sandbox->GameRound = gameStat->GameRound;
	sandbox->LoopRound = gameStat->LoopRound;
	sandbox->Honba = gameStat->Honba;
	sandbox->TurnRound = gameStat->TurnRound;
	sandbox->Deposit = gameStat->Deposit;
	sandbox->AgariChain = gameStat->AgariChain;
	sandbox->LastAgariPlayer = gameStat->LastAgariPlayer;
	for (int i = 0; i < TileNonflowerMax; i++)
		sandbox->OpenRichiWait[i] = gameStat->OpenRichiWait[i];
	sandbox->KangFlag.kangFlag = gameStat->KangFlag.kangFlag;
	sandbox->KangFlag.chainFlag = gameStat->KangFlag.chainFlag;
	sandbox->KangFlag.topFlag = gameStat->KangFlag.topFlag;
	sandbox->KangFlag.chankanFlag = gameStat->KangFlag.chankanFlag;
	sandbox->KangNum = gameStat->KangNum;
	sandbox->RichiCounter = gameStat->RichiCounter;
	sandbox->DoubleRichiCounter = gameStat->DoubleRichiCounter;
	sandbox->DoukasenPlayer = gameStat->DoukasenPlayer;
	for (int i = 0; i < PaoYakuPages; i++) {
		sandbox->PaoFlag[i].paoPlayer = gameStat->PaoFlag[i].paoPlayer;
		sandbox->PaoFlag[i].agariPlayer = gameStat->PaoFlag[i].agariPlayer;
	}
	sandbox->Dice[0].Number = gameStat->Dice[0].Number;
	sandbox->Dice[1].Number = gameStat->Dice[1].Number;
	sandbox->Dice[0].Direction = gameStat->Dice[0].Direction;
	sandbox->Dice[1].Direction = gameStat->Dice[1].Direction;
	for (int i = 0; i < 6; i++) {
		if (gameStat->chkGameType(AllSanma)) {
			if (gameStat->DoraPointer <= (102 - gameStat->ExtraRinshan - i * 2))
				sandbox->Deck[102 - gameStat->ExtraRinshan - i * 2].tile =
				gameStat->Deck[102 - gameStat->ExtraRinshan - i * 2].tile;
		} else {
			if (gameStat->DoraPointer <= (130 - i * 2))
				sandbox->Deck[130 - i * 2].tile = gameStat->Deck[130 - i * 2].tile;
		}
	}
	sandbox->TilePointer = gameStat->TilePointer;
	sandbox->DoraPointer = gameStat->DoraPointer;
	sandbox->RinshanPointer = gameStat->RinshanPointer;
	sandbox->TianHuFlag = gameStat->TianHuFlag;
	sandbox->PreviousMeld.Discard = gameStat->PreviousMeld.Discard;
	sandbox->PreviousMeld.Stepped = gameStat->PreviousMeld.Stepped;
	sandbox->Deposit = gameStat->Deposit;
	sandbox->Deposit = gameStat->Deposit;
	sandbox->Deposit = gameStat->Deposit;
	sandbox->Deposit = gameStat->Deposit;
	for (int i = 0; i < TileNonflowerMax; i++) {
		sandbox->DoraFlag.Omote[i] = gameStat->DoraFlag.Omote[i];
		sandbox->DoraFlag.Ura[i] = gameStat->DoraFlag.Ura[i];
	}
	sandbox->CurrentDiscard.tile = gameStat->CurrentDiscard.tile;
	sandbox->CurrentDiscard.red = gameStat->CurrentDiscard.red;
	sandbox->CurrentPlayer.Active = gameStat->CurrentPlayer.Active;
	sandbox->CurrentPlayer.Passive = gameStat->CurrentPlayer.Passive;
	sandbox->CurrentPlayer.Agari = gameStat->CurrentPlayer.Agari;
	sandbox->CurrentPlayer.Furikomi = gameStat->CurrentPlayer.Furikomi;

	return sandbox;
}

// ---------------------------------------------------------------------

MJCORE GameTable* setGameType(GameTypeID gameType) {
	GameStat.gameType = gameType;
	return &GameStat;
}
