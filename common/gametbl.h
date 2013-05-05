#pragma once

#ifdef _MSC_VER
#include <eh.h>
#endif
#include <cstdint>
#include <climits>
#include <cstdlib>
#include <type_traits>
#include "tilecode.h"
#include "gametype.h"
#include "largenum.h"

namespace mihajong_structs {

// -------------------------------------------------------------------------

enum doraCol : uint8_t { Normal, AkaDora, AoDora };
struct Tile { // �ԃh���f�[�^���܂߂��v�̃f�[�^
	TileCode tile;
	doraCol red;
};
static_assert(std::is_pod<Tile>::value, "Tile is not POD");

// -------------------------------------------------------------------------

const unsigned int Players = 4;
#ifdef MJCORE_EXPORTS
#define ACTUAL_PLAYERS (GameStat.chkGameType(SanmaT) ? 3 : 4)
#endif
const unsigned int NumOfTilesInHand = 14;
const unsigned int TsumohaiIndex = NumOfTilesInHand - 1;
const unsigned int SizeOfDiscardBuffer = 33;

typedef int8_t PlayerID; // �v���C���[�ԍ�

template <class T> struct InfoByPlayer { // �v���C���[���ƂɎw�肵���^�ɂ����(�e���v���[�g)
	T val[Players];
	const T& operator[](const PlayerID playerID) const {
		if ((playerID >= 0)&&(playerID < Players)) {
			return val[playerID];
		}
		else {
#ifdef MJCORE_EXPORTS
			CodeConv::tostringstream o;
			o << _T("InfoByPlayer:�Y�����͈͊O�ł� (") << (int)playerID << _T(")");
			Raise(EXCEPTION_MJCORE_SUBSCRIPT_OUT_OF_RANGE, o.str().c_str());
#endif
#ifdef _MSC_VER
			terminate();
#else
			abort();
#endif
		}
	}
	const T& operator[](const int playerID) const {
		return InfoByPlayer::operator[]((PlayerID)playerID);
	}
	T& operator[](const PlayerID playerID) {
		if ((playerID >= 0)&&(playerID < Players)) {
			return val[playerID];
		}
		else {
#ifdef MJCORE_EXPORTS
			CodeConv::tostringstream o;
			o << _T("InfoByPlayer:�Y�����͈͊O�ł� (") << (int)playerID << _T(")");
			Raise(EXCEPTION_MJCORE_SUBSCRIPT_OUT_OF_RANGE, o.str().c_str());
#endif
#ifdef _MSC_VER
			terminate();
#else
			abort();
#endif
		}
	}
	T& operator[](const int playerID) {
		return InfoByPlayer::operator[]((PlayerID)playerID);
	}
};

// -------------------------------------------------------------------------

typedef Tile HandTiles[NumOfTilesInHand];

// -------------------------------------------------------------------------

const unsigned int SutehaiTypeStep = 200;
enum DiscardStat : uint8_t {
	discardNormal,
	discardTaken,
	discardRiichi,
	discardRiichiTaken,
};
struct DiscardTile {
	Tile tcode;
	DiscardStat dstat;
	bool isDiscardThrough; // �c���؂�t���O
};
typedef DiscardTile DiscardBuf[SizeOfDiscardBuffer];
static_assert(std::is_pod<DiscardTile>::value, "DiscardTile is not POD");

// -------------------------------------------------------------------------

const unsigned int SizeOfMeldBuffer = 5;
const unsigned int MeldTypeStep = 1000;
enum MeldStat : uint8_t {
	meldSequenceConcealed,      // ��̓��̏��q
	meldSequenceExposedLower,   // �����������`�[
	meldSequenceExposedMiddle,  // �ƒ����`�[
	meldSequenceExposedUpper,   // �傫�������`�[
	meldTripletConcealed,       // �Í�
	meldTripletExposedLeft,     // ��Ƃ���|��
	meldTripletExposedCenter,   // �Ζʂ���|��
	meldTripletExposedRight,    // ���Ƃ���|��
	meldQuadConcealed,          // �Þ�
	meldQuadExposedLeft,        // ��Ƃ��疾��
	meldQuadExposedCenter,      // �Ζʂ��疾��
	meldQuadExposedRight,       // ���Ƃ��疾��
	meldQuadAddedLeft,          // ��Ƃ���|���̌�J��
	meldQuadAddedCenter,        // �Ζʂ���|���̌�J��
	meldQuadAddedRight,         // ���Ƃ���|���̌�J��
};
struct MeldCode {
	TileCode tile;
	doraCol red[4];
	MeldStat mstat;
};
typedef MeldCode MeldBuf[SizeOfMeldBuffer];
static_assert(std::is_pod<MeldCode>::value, "MeldCode is not POD");

// -------------------------------------------------------------------------

struct RichiStat {bool RichiFlag, IppatsuFlag, DoubleFlag, OpenFlag;};
static_assert(std::is_pod<RichiStat>::value, "RichiStat is not POD");

// -------------------------------------------------------------------------

enum ChankanStat : uint8_t {
	chankanNone, chankanRegular, chankanOfAnkan, chankanOfNorth,
};
struct KangStat { bool kangFlag, topFlag; uint8_t chainFlag; ChankanStat chankanFlag; };
static_assert(std::is_pod<KangStat>::value, "KangStat is not POD");

// -------------------------------------------------------------------------

enum PaoYakuPage : uint8_t {pyDaisangen, pyDaisixi, pySikang, pyMinkan, PaoYakuPages};

struct PaoStat { PlayerID paoPlayer, agariPlayer; };
typedef PaoStat PaoStatBook[PaoYakuPages];
static_assert(std::is_pod<PaoStat>::value, "PaoStat is not POD");


// -------------------------------------------------------------------------

const unsigned SizeOfDeckBuf = 144;
typedef Tile DeckBuf[SizeOfDeckBuf]; // �ŏ���union�ł�낤�Ǝv�������ǂ��������Ȃ�̂ł�߂�

// -------------------------------------------------------------------------

struct PrevMeldBook { TileCode Discard, Stepped; };
static_assert(std::is_pod<PrevMeldBook>::value, "PrevMeldBook is not POD");

// -------------------------------------------------------------------------

struct DoraStatBook { UInt8ByTile Omote, Ura; };
static_assert(std::is_pod<DoraStatBook>::value, "DoraStatBook is not POD");

// -------------------------------------------------------------------------

enum ChiiType : int8_t {
	chiiNone, chiiLower, chiiMiddle, chiiUpper,
};
struct DeclFlag {
	bool Ron, Kan, Pon;
	ChiiType Chi;
};
static_assert(std::is_pod<DeclFlag>::value, "DeclFlag is not POD");

// -------------------------------------------------------------------------

struct CurrPlayer { PlayerID Active, Passive, Agari, Furikomi; };
static_assert(std::is_pod<CurrPlayer>::value, "CurrPlayer is not POD");

// -------------------------------------------------------------------------

struct Flowers { // �Ԕv�t���O�p
	bool Spring, Summer, Autumn, Winter,
		Plum, Orchid, Chrys, Bamboo;
	/* Chrysanthemum �͒Ԃ肪�����̂ŏȗ��c�c */
};
static_assert(std::is_pod<Flowers>::value, "Flowers is not POD");

// -------------------------------------------------------------------------

struct Dice { // �T�C�R��
	uint8_t Number;
	uint8_t Direction;
};
static_assert(std::is_pod<Dice>::value, "Dice is not POD");

// -------------------------------------------------------------------------

enum handStatCode : int8_t {
	handUpright, handExposed, handHidden, handOpenRiichi,
};
struct PlayerTable { // �v���C���[�̏�Ԃ��i�[
	LargeNum PlayerScore;
	int playerChip; // �`�b�v�̎��x
	HandTiles Hand; // ��v�̔z��
	DiscardBuf Discard; // �̔v�̔z��
	MeldBuf Meld; // ���ʎq���i�[
	uint8_t DiscardPointer;
	uint8_t MeldPointer;
	bool MenzenFlag; // ��O�t���O
	handStatCode HandStat; // ��v�̏�ԁi���Ă�E������E������j
	int8_t NumberOfQuads; // �Ȏq�̐��i�l�ȗ��ǁA�O�Ȏq�A�l�Ȏq�Ȃǂ̔���Ɏg���j
	bool FirstDrawFlag; // �P���ڂł���i�n�a�A�_�u�������̔���Ɏg���j
	bool DoujunFuriten; // �����U���ł���
	bool AgariHouki; // �a��������̔��������ǂ���
	RichiStat RichiFlag; // ���[�`���Ă��邩�ǂ���
	DeclFlag DeclarationFlag; // ���̐錾
	Flowers FlowerFlag; // �N���Ă���Ԕv���i�[����t���O
	uint8_t NorthFlag; // �N���Ă���k���v���i�[����t���O
	bool SumaroFlag; // �l�n�H���փt���O
	bool YakitoriFlag; // �Ă����t���O
	bool shokanFlag; // ���ȃt���O
	bool kansanjunFlag; // �ȎO���t���O
	bool ConnectionLost;

	const Tile& Tsumohai() const {return Hand[TsumohaiIndex];} /* ���̔v (immutable) */
	      Tile& Tsumohai()       {return Hand[TsumohaiIndex];} /* ���̔v (mutable) */
};
static_assert(std::is_pod<PlayerTable>::value, "PlayerTable is not POD");

// -------------------------------------------------------------------------

enum seatAbsolute : uint8_t { sEast, sSouth, sWest, sNorth };
enum seatRelative : uint8_t { sSelf, sRight, sOpposite, sLeft };

// -------------------------------------------------------------------------

template struct InfoByPlayer<PlayerTable>;
typedef InfoByPlayer<PlayerTable> StatusByPlayer;
typedef PlayerID Player_ID;
typedef Dice Dice_Struct;
struct GameTable { // ��̏����i�[����
	StatusByPlayer Player;
	DeckBuf Deck; // �ǔv�̔z��
	DoraStatBook DoraFlag; // �h������̔z��
	FlagByTile OpenRichiWait; // �v�����[�̑҂��v(�b�n�l�ɈӐ}�I�ȕ��e���N�������Ȃ����߂Ɏg�p)
	PaoStatBook PaoFlag; // ��t���O�i-1�c�Ȃ��A0�`3�c�Y���v���C���[�j
	int GameLength;
	int GameRound;
	int LoopRound;
	int Honba;
	int Deposit;
	int AgariChain;
	CurrPlayer CurrentPlayer;
	Player_ID PlayerID;
	Player_ID LastAgariPlayer;
	GameTypeID gameType;
	bool TsumoAgariFlag; // �c���A�K���H
	int16_t AgariSpecialStat; // ���̂Ƃ���H���ς��Ń`�����{�ɂȂ�ꍇ�����g���Ă�H
	PrevMeldBook PreviousMeld; // ��قǖ����v�i�򂢑ւ��̔���Ɏg���j
	KangStat KangFlag; // ���J�ԁG�A�J�ԂƞȐU��G���Șa�G���Ȃ̔���Ɏg��
	Dice_Struct Dice[2]; // �T�C�R��
	uint8_t TurnRound; // ���݂̏���
	uint8_t KangNum; // �l�ȗ��ǁA�l�Ȏq�Ȃǂ̔���Ɏg��
	bool RichiCounter; // ���[�`���J�E���^�[(�錾�v������)
	bool DoubleRichiCounter; // �_�u�����[�`���J�E���^�[(�錾�v������)
	Player_ID WaremePlayer; // ����ڂ̈ʒu(-1�Ŋ���ڂȂ�)
	Player_ID DoukasenPlayer; // ���ΐ��̈ʒu(-1�œ��ΐ��Ȃ�)
	uint8_t DeadTiles; // ���v�̐�
	uint8_t ExtraRinshan; // �ǉ��̗��v�̐�
	bool ShibariFlag; //���ʔ���
	uint8_t TilePointer; // �c���v�̃|�C���^
	uint16_t DoraPointer;
	uint8_t RinshanPointer; // ���v�̃|�C���^
	bool TianHuFlag; // �e�̑��Ŕv���܂��i�V�a�̔���ȂǂɎg���j
	Tile CurrentDiscard;

	const PlayerTable& statOfActive () const {return Player[CurrentPlayer.Active ];} /* ���̔Ԃ̃v���C���[�̏�� (immutable) */
	      PlayerTable& statOfActive ()       {return Player[CurrentPlayer.Active ];} /* ���̔Ԃ̃v���C���[�̏�� (mutable) */
	const PlayerTable& statOfPassive() const {return Player[CurrentPlayer.Passive];} /* �����蒆�̃v���C���[�̏�� (immutable) */
	      PlayerTable& statOfPassive()       {return Player[CurrentPlayer.Passive];} /* �����蒆�̃v���C���[�̏�� (mutable) */
	const PlayerTable& statOfAgari  () const {return Player[CurrentPlayer.Agari  ];} /* �a�������v���C���[�̏�� (immutable) */
	      PlayerTable& statOfAgari  ()       {return Player[CurrentPlayer.Agari  ];} /* �a�������v���C���[�̏�� (mutable) */
	const PlayerTable& statOfMine   () const {return Player[PlayerID             ];} /* �����̃v���C���[�̏�� (immutable) */
	      PlayerTable& statOfMine   ()       {return Player[PlayerID             ];} /* �����̃v���C���[�̏�� (mutable) */

	bool chkGameType(GameTypeID gameType) const {return ((this->gameType) & gameType);}
	uint8_t diceSum() { // �T�C�R���̏o�ڂ��擾
		return Dice[0].Number + Dice[1].Number;
	}

	seatAbsolute playerwind(Player_ID player, int currentRound) const { // �v���C���[�̎������ǂꂩ���ׂ�
		if (chkGameType(SanmaT))
			return (seatAbsolute)((player + 24 - (currentRound - ( currentRound / 4))) % 3);
		else return (seatAbsolute)((player + 32 - currentRound) % 4);
	}
	seatAbsolute playerwind(Player_ID player) const { // �v���C���[�̎������ǂꂩ���ׂ�
		return playerwind(player, GameRound);
	}

	int tilesLeft() const { // ���v���������R�v�̎c�薇��
		return ((int)RinshanPointer - ((int)DeadTiles - 1) - (int)TilePointer);
	}

};
static_assert(std::is_pod<GameTable>::value, "GameTable is not POD");

// -------------------------------------------------------------------------

// �H���ς�����p�� gameStat->AgariSpecialStat �ԍ�
const unsigned int agariKuikae = 999;

// -------------------------------------------------------------------------

} /* namespace */
