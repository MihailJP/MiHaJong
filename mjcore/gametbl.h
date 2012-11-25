#ifndef GAMETBL_H
#define GAMETBL_H

#include <cstdint>
#ifdef MJCORE_EXPORTS
#include <algorithm>
#include <cassert>
#include <type_traits>
#endif
#include "mjexport.h"
#include "tilecode.h"
#include "ruletbl.h"
#include "largenum.h"
#include "except.h"

// -------------------------------------------------------------------------

enum doraCol : uint8_t { Normal, AkaDora, AoDora };
EXPORT_STRUCT TILE { // �ԃh���f�[�^���܂߂��v�̃f�[�^
	tileCode tile;
	doraCol red;
};
#ifdef MJCORE_EXPORTS
static_assert(std::is_pod<TILE>::value, "TILE is not POD");
#endif

// -------------------------------------------------------------------------

#define PLAYERS 4
#ifdef MJCORE_EXPORTS
#define ACTUAL_PLAYERS (chkGameType(&GameStat, SanmaT) ? 3 : 4)
#endif
#define NUM_OF_TILES_IN_HAND 14
#define SIZE_OF_DISCARD_BUFFER 33

typedef int8_t PLAYER_ID; // �v���C���[�ԍ�

template <class T> struct InfoByPlayer { // �v���C���[���ƂɎw�肵���^�ɂ����(�e���v���[�g)
	T val[PLAYERS];
	const T& operator[](const PLAYER_ID playerID) const {
		if ((playerID >= 0)&&(playerID < PLAYERS)) {
			return val[playerID];
		}
		else {
#ifdef MJCORE_EXPORTS
			CodeConv::tostringstream o;
			o << _T("InfoByPlayer:�Y�����͈͊O�ł� (") << (int)playerID << _T(")");
			Raise(EXCEPTION_MJCORE_SUBSCRIPT_OUT_OF_RANGE, o.str().c_str());
#endif
			terminate();
		}
	}
	const T& operator[](const int playerID) const {
		return InfoByPlayer::operator[]((PLAYER_ID)playerID);
	}
	T& operator[](const PLAYER_ID playerID) {
		if ((playerID >= 0)&&(playerID < PLAYERS)) {
			return val[playerID];
		}
		else {
#ifdef MJCORE_EXPORTS
			CodeConv::tostringstream o;
			o << _T("InfoByPlayer:�Y�����͈͊O�ł� (") << (int)playerID << _T(")");
			Raise(EXCEPTION_MJCORE_SUBSCRIPT_OUT_OF_RANGE, o.str().c_str());
#endif
			terminate();
		}
	}
	T& operator[](const int playerID) {
		return InfoByPlayer::operator[]((PLAYER_ID)playerID);
	}
};

// -------------------------------------------------------------------------

typedef TILE HAND_TILES[NUM_OF_TILES_IN_HAND];

// -------------------------------------------------------------------------

#define SUTEHAI_TYPE_STEP 200
enum discardStat : uint8_t {
	discardNormal,
	discardTaken,
	discardRiichi,
	discardRiichiTaken,
};
EXPORT_STRUCT discardTile {
	TILE tcode;
	discardStat dstat;
	bool isDiscardThrough; // �c���؂�t���O
};
typedef discardTile DISCARD_BUF[SIZE_OF_DISCARD_BUFFER];
#ifdef MJCORE_EXPORTS
static_assert(std::is_pod<discardTile>::value, "discardTile is not POD");
#endif

// -------------------------------------------------------------------------

#define SIZE_OF_MELD_BUFFER 5
#define MELD_TYPE_STEP 1000
enum meldStat : uint8_t {
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
EXPORT_STRUCT meldCode {
	tileCode tile;
	doraCol red[4];
	meldStat mstat;
};
typedef meldCode MELD_BUF[SIZE_OF_MELD_BUFFER];
#ifdef MJCORE_EXPORTS
static_assert(std::is_pod<meldCode>::value, "meldCode is not POD");
#endif

// -------------------------------------------------------------------------

EXPORT_STRUCT RichiStat {bool RichiFlag, IppatsuFlag, DoubleFlag, OpenFlag;};
#ifdef MJCORE_EXPORTS
static_assert(std::is_pod<RichiStat>::value, "RichiStat is not POD");
#endif

// -------------------------------------------------------------------------

enum ChankanStat : uint8_t {
	chankanNone, chankanRegular, chankanOfAnkan, chankanOfNorth,
};
EXPORT_STRUCT KANGSTAT { bool kangFlag, topFlag; uint8_t chainFlag; ChankanStat chankanFlag; };
#ifdef MJCORE_EXPORTS
static_assert(std::is_pod<KANGSTAT>::value, "KANGSTAT is not POD");
#endif

// -------------------------------------------------------------------------

#define PAO_YAKU_PAGES 4
enum paoYakuPage : uint8_t {pyDaisangen, pyDaisixi, pySikang, pyMinkan};

EXPORT_STRUCT PAOSTAT { PLAYER_ID paoPlayer, agariPlayer; };
typedef PAOSTAT paoStatBook[PAO_YAKU_PAGES];
#ifdef MJCORE_EXPORTS
static_assert(std::is_pod<PAOSTAT>::value, "PAOSTAT is not POD");
#endif


// -------------------------------------------------------------------------

#define SIZE_OF_DECKBUF 144
typedef TILE DeckBuf[SIZE_OF_DECKBUF]; // �ŏ���union�ł�낤�Ǝv�������ǂ��������Ȃ�̂ł�߂�

// -------------------------------------------------------------------------

EXPORT_STRUCT prevMeldBook { tileCode Discard, Stepped; };
#ifdef MJCORE_EXPORTS
static_assert(std::is_pod<prevMeldBook>::value, "prevMeldBook is not POD");
#endif

// -------------------------------------------------------------------------

EXPORT_STRUCT doraStatBook { UInt8ByTile Omote, Ura; };
#ifdef MJCORE_EXPORTS
static_assert(std::is_pod<doraStatBook>::value, "doraStatBook is not POD");
#endif

// -------------------------------------------------------------------------

enum ChiiType : int8_t {
	chiiNone, chiiLower, chiiMiddle, chiiUpper,
};
EXPORT_STRUCT DECLFLAG {
	bool Ron, Kan, Pon;
	ChiiType Chi;
};
#ifdef MJCORE_EXPORTS
static_assert(std::is_pod<DECLFLAG>::value, "DECLFLAG is not POD");
#endif

// -------------------------------------------------------------------------

EXPORT_STRUCT CURRPLAYER { PLAYER_ID Active, Passive, Agari, Furikomi; };
#ifdef MJCORE_EXPORTS
static_assert(std::is_pod<CURRPLAYER>::value, "CURRPLAYER is not POD");
#endif

// -------------------------------------------------------------------------

EXPORT_STRUCT FLOWERS { // �Ԕv�t���O�p
	bool Spring, Summer, Autumn, Winter,
		Plum, Orchid, Chrys, Bamboo;
	/* Chrysanthemum �͒Ԃ肪�����̂ŏȗ��c�c */
};
#ifdef MJCORE_EXPORTS
static_assert(std::is_pod<FLOWERS>::value, "FLOWERS is not POD");
#endif

// -------------------------------------------------------------------------

EXPORT_STRUCT DICE { // �T�C�R��
	uint8_t Number;
	bool Direction;
};
#ifdef MJCORE_EXPORTS
static_assert(std::is_pod<DICE>::value, "DICE is not POD");
#endif

// -------------------------------------------------------------------------

enum handStatCode : int8_t {
	handUpright, handExposed, handHidden, handOpenRiichi,
};
EXPORT_STRUCT PlayerTable { // �v���C���[�̏�Ԃ��i�[
	LargeNum PlayerScore;
	int playerChip; // �`�b�v�̎��x
	bool SumaroFlag; // �l�n�H���փt���O
	bool YakitoriFlag; // �Ă����t���O
	HAND_TILES Hand; // ��v�̔z��
	DISCARD_BUF Discard; // �̔v�̔z��
	uint8_t DiscardPointer;
	MELD_BUF Meld; // ���ʎq���i�[
	uint8_t MeldPointer;
	bool MenzenFlag; // ��O�t���O
	handStatCode HandStat; // ��v�̏�ԁi���Ă�E������E������j
	int8_t NumberOfQuads; // �Ȏq�̐��i�l�ȗ��ǁA�O�Ȏq�A�l�Ȏq�Ȃǂ̔���Ɏg���j
	RichiStat RichiFlag; // ���[�`���Ă��邩�ǂ���
	bool FirstDrawFlag; // �P���ڂł���i�n�a�A�_�u�������̔���Ɏg���j
	bool DoujunFuriten; // �����U���ł���
	bool AgariHouki; // �a��������̔��������ǂ���
	FLOWERS FlowerFlag; // �N���Ă���Ԕv���i�[����t���O
	uint8_t NorthFlag; // �N���Ă���k���v���i�[����t���O
	bool ConnectionLost;
	DECLFLAG DeclarationFlag; // ���̐錾
};
#ifdef MJCORE_EXPORTS
static_assert(std::is_pod<PlayerTable>::value, "PlayerTable is not POD");
#endif

// -------------------------------------------------------------------------

EXPORT_TEMPLATE_STRUCT InfoByPlayer<PlayerTable>;
typedef InfoByPlayer<PlayerTable> StatusByPlayer;
EXPORT_STRUCT GameTable { // ��̏����i�[����
	gameTypeID gameType;
	StatusByPlayer Player;
	PLAYER_ID PlayerID;
	int GameLength;
	int GameRound;
	int LoopRound;
	int Honba;
	int Deposit;
	int AgariChain;
	PLAYER_ID LastAgariPlayer;
	FlagByTile OpenRichiWait; // �v�����[�̑҂��v(�b�n�l�ɈӐ}�I�ȕ��e���N�������Ȃ����߂Ɏg�p)
	KANGSTAT KangFlag; // ���J�ԁG�A�J�ԂƞȐU��G���Șa�G���Ȃ̔���Ɏg��
	uint8_t TurnRound; // ���݂̏���
	uint8_t KangNum; // �l�ȗ��ǁA�l�Ȏq�Ȃǂ̔���Ɏg��
	bool RichiCounter; // ���[�`���J�E���^�[(�錾�v������)
	bool DoubleRichiCounter; // �_�u�����[�`���J�E���^�[(�錾�v������)
	PLAYER_ID WaremePlayer; // ����ڂ̈ʒu(-1�Ŋ���ڂȂ�)
	PLAYER_ID DoukasenPlayer; // ���ΐ��̈ʒu(-1�œ��ΐ��Ȃ�)
	DICE Dice[2]; // �T�C�R��
	paoStatBook PaoFlag; // ��t���O�i-1�c�Ȃ��A0�`3�c�Y���v���C���[�j
	DeckBuf Deck; // �ǔv�̔z��
	uint8_t DeadTiles; // ���v�̐�
	uint8_t ExtraRinshan; // �ǉ��̗��v�̐�
	bool ShibariFlag; //���ʔ���
	doraStatBook DoraFlag; // �h������̔z��
	uint8_t TilePointer; // �c���v�̃|�C���^
	uint16_t DoraPointer;
	uint8_t RinshanPointer; // ���v�̃|�C���^
	bool TianHuFlag; // �e�̑��Ŕv���܂��i�V�a�̔���ȂǂɎg���j
	prevMeldBook PreviousMeld; // ��قǖ����v�i�򂢑ւ��̔���Ɏg���j
	CURRPLAYER CurrentPlayer;
	bool TsumoAgariFlag; // �c���A�K���H
	int16_t AgariSpecialStat; // ���̂Ƃ���H���ς��Ń`�����{�ɂȂ�ꍇ�����g���Ă�H
	TILE CurrentDiscard;
};
#ifdef MJCORE_EXPORTS
static_assert(std::is_pod<GameTable>::value, "GameTable is not POD");
#endif

// -------------------------------------------------------------------------

#ifdef MJCORE_EXPORTS
/* ��s�錾 */
extern "C" inline uint8_t diceSum(const GameTable* const gameStat);
PLAYER_ID tobePlayed(const GameTable* const gameStat, int id);

/* gametbl.cpp */
extern GameTable GameStat, StatSandBox;
inline bool chkGameType(const GameTable* const gameStat, gameTypeID gameType);
extern "C" __declspec(dllexport) void calcWareme(GameTable* const gameStat);
extern "C" GameTable* makesandBox(const GameTable* const gameStat, PLAYER_ID targetPlayer);
extern "C" __declspec(dllexport) void inittable(GameTable* const gameStat);
extern "C" GameTable* initializeGameTable(gameTypeID gameType);
#endif

// -------------------------------------------------------------------------

#endif
