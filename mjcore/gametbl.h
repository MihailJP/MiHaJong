#ifndef GAMETBL_H
#define GAMETBL_H

#include <array>
#include <bitset>
#include <cstdint>
#include <exception>
#include <stdexcept>
#include <algorithm>
#include <cassert>
#include "tilecode.h"
#include "ruletbl.h"
#include "largenum.h"

// -------------------------------------------------------------------------

enum gameTypeID { // ��̎��(�l���A�O��)�w��p
	Yonma = 0x01, Sanma = 0x02, Sanma4 = 0x04, SanmaS = 0x08,
	AllSanma = 0x0e, SanmaT = 0x0a, SanmaX = 0x06
};

// -------------------------------------------------------------------------

struct TILE { // �ԃh���f�[�^���܂߂��v�̃f�[�^
	tileCode tile;
	uint8_t red;
};

// -------------------------------------------------------------------------

#define PLAYERS 4
#define ACTUAL_PLAYERS (chkGameType(&GameStat, SanmaT) ? 3 : 4)
#define NUM_OF_TILES_IN_HAND 14
#define SIZE_OF_DISCARD_BUFFER 33

typedef int8_t PLAYER_ID; // �v���C���[�ԍ�

// -------------------------------------------------------------------------

typedef std::array<TILE, NUM_OF_TILES_IN_HAND> HAND_TILES;

// -------------------------------------------------------------------------

#define SUTEHAI_TYPE_STEP 200
enum discardStat {
	discardNormal,
	discardTaken,
	discardRiichi,
	discardRiichiTaken,
};
struct discardTile {
	TILE tcode;
	discardStat dstat;
	bool isDiscardThrough; // �c���؂�t���O
};
typedef std::array<discardTile, SIZE_OF_DISCARD_BUFFER> DISCARD_BUF;

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
struct meldCode {
	TILE tcode;
	meldStat mstat;
};
typedef std::array<meldCode, SIZE_OF_MELD_BUFFER> MELD_BUF;

// -------------------------------------------------------------------------

enum RICHI_STAT_BITS { // �����t���O���i�[
	RichiFlag, IppatsuFlag, DoubleFlag, OpenFlag,
	RichiStatBits
};
typedef std::bitset<RichiStatBits> RichiStat;

// -------------------------------------------------------------------------

struct KANGSTAT { uint8_t kangFlag, chainFlag, topFlag, chankanFlag; };

// -------------------------------------------------------------------------

#define PAO_YAKU_PAGES 4
enum paoYakuPage {pyDaisangen, pyDaisixi, pySikang, pyMinkan};

struct PAOSTAT { PLAYER_ID paoPlayer, agariPlayer; };
typedef std::array<PAOSTAT, PAO_YAKU_PAGES> paoStatBook;


// -------------------------------------------------------------------------

typedef std::array<TILE, 144> DeckBuf; // �ŏ���union�ł�낤�Ǝv�������ǂ��������Ȃ�̂ł�߂�

// -------------------------------------------------------------------------

struct prevMeldBook { tileCode Discard, Stepped; };

// -------------------------------------------------------------------------

typedef std::array<uint8_t, TILE_NONFLOWER_MAX> DORASTAT;
struct doraStatBook { DORASTAT Omote, Ura; };

// -------------------------------------------------------------------------

struct DECLFLAG {
	bool Ron, Kan, Pon;
	int8_t Chi;
};

// -------------------------------------------------------------------------

struct CURRPLAYER { PLAYER_ID Active, Passive, Agari, Furikomi; };

// -------------------------------------------------------------------------

struct PlayerTable { // �v���C���[�̏�Ԃ��i�[
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

// -------------------------------------------------------------------------

struct GameTable { // ��̏����i�[����
	gameTypeID gameType;
	std::array<PlayerTable, PLAYERS> Player;
	PLAYER_ID PlayerID;
	int GameLength;
	int GameRound;
	int LoopRound;
	int Honba;
	int Deposit;
	int AgariChain;
	int LastAgariPlayer;
	std::array<bool, TILE_NONFLOWER_MAX> OpenRichiWait; // �v�����[�̑҂��v(�b�n�l�ɈӐ}�I�ȕ��e���N�������Ȃ����߂Ɏg�p)
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

// -------------------------------------------------------------------------

extern GameTable GameStat;
inline bool chkGameType(GameTable* gameStat, gameTypeID gameType);

// -------------------------------------------------------------------------

#endif
