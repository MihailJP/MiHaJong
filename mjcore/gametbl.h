#ifndef GAMETBL_H
#define GAMETBL_H

#include <cstdint>
#include <exception>
#include <stdexcept>
#include <algorithm>
#include <cassert>
#include <type_traits>
#include "mjexport.h"
#include "tilecode.h"
#include "ruletbl.h"
#include "largenum.h"

// -------------------------------------------------------------------------

enum gameTypeID { // ��̎��(�l���A�O��)�w��p
	Yonma = 0x01, Sanma = 0x02, Sanma4 = 0x04, SanmaS = 0x08,
	AllSanma = 0x0e, SanmaT = 0x0a, SanmaX = 0x06
};

// -------------------------------------------------------------------------

EXPORT_STRUCT TILE { // �ԃh���f�[�^���܂߂��v�̃f�[�^
	tileCode tile;
	uint8_t red;
};
#ifndef IMPORT_MJCORE
static_assert(std::is_pod<TILE>::value, "Non-POD data type detected");
#endif

// -------------------------------------------------------------------------

#define PLAYERS 4
#ifndef IMPORT_MJCORE
#define ACTUAL_PLAYERS (chkGameType(&GameStat, SanmaT) ? 3 : 4)
#endif
#define NUM_OF_TILES_IN_HAND 14
#define SIZE_OF_DISCARD_BUFFER 33

typedef int8_t PLAYER_ID; // �v���C���[�ԍ�

// -------------------------------------------------------------------------

typedef TILE HAND_TILES[NUM_OF_TILES_IN_HAND];

// -------------------------------------------------------------------------

#define SUTEHAI_TYPE_STEP 200
enum discardStat {
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
#ifndef IMPORT_MJCORE
static_assert(std::is_pod<discardTile>::value, "Non-POD data type detected");
#endif

// -------------------------------------------------------------------------

#define SIZE_OF_MELD_BUFFER 5
#define MELD_TYPE_STEP 1000
enum meldStat {
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
	meldQuadAddedRight          // ���Ƃ���|���̌�J��
};
EXPORT_STRUCT meldCode {
	TILE tcode;
	meldStat mstat;
};
typedef meldCode MELD_BUF[SIZE_OF_MELD_BUFFER];
#ifndef IMPORT_MJCORE
static_assert(std::is_pod<discardTile>::value, "Non-POD data type detected");
#endif

// -------------------------------------------------------------------------

EXPORT_STRUCT RichiStat {bool RichiFlag, IppatsuFlag, DoubleFlag, OpenFlag;};
#ifndef IMPORT_MJCORE
static_assert(std::is_pod<RichiStat>::value, "Non-POD data type detected");
#endif

// -------------------------------------------------------------------------

EXPORT_STRUCT KANGSTAT { uint8_t kangFlag, chainFlag, topFlag, chankanFlag; };
#ifndef IMPORT_MJCORE
static_assert(std::is_pod<KANGSTAT>::value, "Non-POD data type detected");
#endif

// -------------------------------------------------------------------------

#define PAO_YAKU_PAGES 4
enum paoYakuPage {pyDaisangen, pyDaisixi, pySikang, pyMinkan};

EXPORT_STRUCT PAOSTAT { PLAYER_ID paoPlayer, agariPlayer; };
typedef PAOSTAT paoStatBook[PAO_YAKU_PAGES];
#ifndef IMPORT_MJCORE
static_assert(std::is_pod<PAOSTAT>::value, "Non-POD data type detected");
#endif


// -------------------------------------------------------------------------

#define SIZE_OF_DECKBUF 144
typedef TILE DeckBuf[SIZE_OF_DECKBUF]; // �ŏ���union�ł�낤�Ǝv�������ǂ��������Ȃ�̂ł�߂�

// -------------------------------------------------------------------------

EXPORT_STRUCT prevMeldBook { tileCode Discard, Stepped; };
#ifndef IMPORT_MJCORE
static_assert(std::is_pod<prevMeldBook>::value, "Non-POD data type detected");
#endif

// -------------------------------------------------------------------------

typedef uint8_t DORASTAT[TILE_NONFLOWER_MAX];
EXPORT_STRUCT doraStatBook { DORASTAT Omote, Ura; };
#ifndef IMPORT_MJCORE
static_assert(std::is_pod<doraStatBook>::value, "Non-POD data type detected");
#endif

// -------------------------------------------------------------------------

EXPORT_STRUCT DECLFLAG {
	bool Ron, Kan, Pon;
	int8_t Chi;
};
#ifndef IMPORT_MJCORE
static_assert(std::is_pod<DECLFLAG>::value, "Non-POD data type detected");
#endif

// -------------------------------------------------------------------------

EXPORT_STRUCT CURRPLAYER { PLAYER_ID Active, Passive, Agari, Furikomi; };
#ifndef IMPORT_MJCORE
static_assert(std::is_pod<CURRPLAYER>::value, "Non-POD data type detected");
#endif

// -------------------------------------------------------------------------

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
	int8_t HandStat; // ��v�̏�ԁi���Ă�E������E������j
	int8_t NumberOfQuads; // �Ȏq�̐��i�l�ȗ��ǁA�O�Ȏq�A�l�Ȏq�Ȃǂ̔���Ɏg���j
	RichiStat RichiFlag; // ���[�`���Ă��邩�ǂ���
	bool FirstDrawFlag; // �P���ڂł���i�n�a�A�_�u�������̔���Ɏg���j
	bool DoujunFuriten; // �����U���ł���
	bool AgariHouki; // �a��������̔��������ǂ���
	uint8_t FlowerFlag; // �N���Ă���Ԕv���i�[����t���O
	uint8_t NorthFlag; // �N���Ă���k���v���i�[����t���O
	bool ConnectionLost;
	DECLFLAG DeclarationFlag; // ���̐錾
};
#ifndef IMPORT_MJCORE
static_assert(std::is_pod<PlayerTable>::value, "Non-POD data type detected");
#endif

// -------------------------------------------------------------------------

EXPORT_STRUCT GameTable { // ��̏����i�[����
	gameTypeID gameType;
	PlayerTable Player[PLAYERS];
	PLAYER_ID PlayerID;
	int GameLength;
	int GameRound;
	int LoopRound;
	int Honba;
	int Deposit;
	int AgariChain;
	int LastAgariPlayer;
	bool OpenRichiWait[TILE_NONFLOWER_MAX]; // �v�����[�̑҂��v(�b�n�l�ɈӐ}�I�ȕ��e���N�������Ȃ����߂Ɏg�p)
	KANGSTAT KangFlag; // ���J�ԁG�A�J�ԂƞȐU��G���Șa�G���Ȃ̔���Ɏg��
	uint8_t TurnRound; // ���݂̏���
	uint8_t KangNum; // �l�ȗ��ǁA�l�Ȏq�Ȃǂ̔���Ɏg��
	bool RichiCounter; // ���[�`���J�E���^�[(�錾�v������)
	PLAYER_ID WaremePlayer; // ����ڂ̈ʒu(-1�Ŋ���ڂȂ�)
	PLAYER_ID DoukasenPlayer; // ���ΐ��̈ʒu(-1�œ��ΐ��Ȃ�)
	uint8_t Dice1;
	uint8_t Dice2;
	bool Dice1Direction;
	bool Dice2Direction;
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
#ifndef IMPORT_MJCORE
static_assert(std::is_pod<GameTable>::value, "Non-POD data type detected");
#endif

// -------------------------------------------------------------------------

#ifndef IMPORT_MJCORE
extern GameTable GameStat;
inline bool chkGameType(GameTable* gameStat, gameTypeID gameType);
#endif

// -------------------------------------------------------------------------

#endif
