#include "mnzdat.h"

using namespace std;
using std::min;

/* �ʎq�f�[�^������ */

static uint8_t* mentsuAnalysisDat = NULL;

void LoadFileInResource(int name, int type, DWORD& size, const char*& data) {
	HMODULE handle = ::GetModuleHandleA(NULL);
	HRSRC rc = ::FindResourceA(handle, MAKEINTRESOURCE(name), MAKEINTRESOURCE(type));
	HGLOBAL rcData = ::LoadResource(handle, rc);
	size = ::SizeofResource(handle, rc);
	data = static_cast<const char*>(::LockResource(rcData));
}

size_t decompressMentsuAnalysisDat() {
	DWORD size = 0; size_t uncompressedSize;
	const char* compressedMentsuDat = NULL;
	int result;
	LoadFileInResource(IDR_LZMA_STREA1, LZMA_STREAM, size, compressedMentsuDat);
	uncompressedSize = *((size_t *)(compressedMentsuDat+5));
	mentsuAnalysisDat = (uint8_t *)malloc(uncompressedSize);
	result = LzmaUncompress(mentsuAnalysisDat, &uncompressedSize,
		(const uint8_t *)(compressedMentsuDat+13),
		(SizeT *)&size, (const uint8_t *)compressedMentsuDat, 5);
	if (result != SZ_OK) {
		ostringstream o;
		o << "LZMA�X�g���[���̃f�R�[�h�Ɏ��s���܂����B�t�@�C�������Ă���񂪂���܂��B�G���[�R�[�h: " << result;
		fatal(o.str());
		throw decompress_failure(o.str(), result);
	}
	else {
		info("LZMA�X�g���[�����f�R�[�h���܂����B");
	}
	return uncompressedSize;
}

void calcSHA256(uint8_t* digest, const uint8_t* buf, size_t bufSize) {
	CSha256 p;
	Sha256_Init(&p);
	Sha256_Update(&p, buf, bufSize);
	Sha256_Final(&p, digest);
}

string bytesToHexString(vector<uint8_t> byteStr) {
	string hx = string();
	ostringstream o;
	o.setf(ios::right); o.fill('0'); o.width(2);
	for (unsigned int i = 0; i < byteStr.size(); i++) o << byteStr[i];
	return o.str();
}

void verifyMentsuAnalysisDat(size_t bufSize) {
	uint8_t expectedDigest[] = {
		0x2d, 0x10, 0x7e, 0x88, 0x85, 0xad, 0xd7, 0xe0,
		0x1f, 0xec, 0x65, 0xfa, 0x69, 0x06, 0x33, 0x7a,
		0xba, 0xe9, 0xf7, 0x6c, 0xfb, 0x6f, 0xc2, 0xe8,
		0x98, 0xca, 0xfe, 0x17, 0xaa, 0x7b, 0x51, 0xc1
	};
	uint8_t actualDigest[32]; bool mdUnmatch = false;
	calcSHA256(actualDigest, mentsuAnalysisDat, bufSize);
	for (int i = 0; i < 32; i++) {
		if (expectedDigest[i] != actualDigest[i]) mdUnmatch = true;
	}
	if (mdUnmatch) {
		ostringstream o;
		o << "�ʎq�\���f�[�^�x�[�X��SHA256�n�b�V���l����v���܂���ł����B";
		o << "�t�@�C�������Ă���񂪂���܂��B";
		o << "���҂����n�b�V���l: " << bytesToHexString(MDVEC(expectedDigest[0], expectedDigest[31])) << " ";
		o << "���ۂ̃n�b�V���l: " << bytesToHexString(MDVEC(actualDigest[0], actualDigest[31]));
		fatal(o.str());
		throw hash_mismatch(o.str(),
			MDVEC(expectedDigest[0], expectedDigest[31]),
			MDVEC(actualDigest[0], actualDigest[31]));
	}
	else {
		info("�ʎq�\���f�[�^�x�[�X��SHA256�n�b�V���l�̏ƍ��ɐ������܂����B");
	}
}

__declspec(dllexport) void initMentsuAnalysisDat() { // �ʎq�f�[�^������
	verifyMentsuAnalysisDat(decompressMentsuAnalysisDat());
}

/* ���������v�Z���� */

SHANTEN calcShanten(GameTable* gameStat, PLAYER_ID playerID, shantenType mode)
{ // ���������v�Z����
	/* ���v���ꂼ��̖ʎq�̐��𐔂��� */
	TileCount tileCount = countTilesInHand(gameStat, playerID);

	/* mode�ʕ��� */
	switch (mode) {
	case shantenRegular:
		return calcShantenRegular(gameStat, playerID, tileCount);
	case shantenPairs:
		return calcShantenChiitoi(gameStat, playerID, tileCount);
	case shantenOrphans:
		return calcShantenKokushi(gameStat, playerID, tileCount);
	case shantenStellar:
		/* ���Ƃŏ��� */
		break;
	case shantenCivilWar:
		/* ���Ƃŏ��� */
		break;
	case shantenSyzygy:
		/* ���Ƃŏ��� */
		break;
	case shantenQuanbukao:
		/* ���Ƃŏ��� */
		break;
	default:
		/* �S�����߂Ĉ�Ԙa���ɋ߂����Ԃ� */
		/* ���Ƃŏ��� */
		break;
	}
	return 0; //������
}

SHANTEN calcShantenRegular(GameTable* gameStat, PLAYER_ID playerID, TileCount& tileCount)
{ // �ʎq��̌����������߂�
	SHANTEN shanten = 8; // �S�������ĂȂ��Ă�Ńo���o����������ʎq��ɑ΂���8�����i���Ύq�ɑ΂��ĂȂ�6�����ɂȂ�j

	// ���v
	int mianzi = 0; int tarzi = 0; bool atama = false; // �ʎq���q�����̐�
	for (int suit = 0; suit < TILE_NUMERAL_COLORS; suit++) {
		unsigned int statcode = 0; unsigned int qDigit = 1;
		for (int i = 1; i <= 9; i++) {
			statcode += min(tileCount[suit * TILE_SUIT_STEP + i], 4) * qDigit;
			qDigit *= 5;
		}
		uint8_t tmpdat = mentsuAnalysisDat[statcode];
		mianzi += (tmpdat & 0x70) >> 4;
		tarzi += tmpdat & 0x0f;
		if (tmpdat & 0x80) atama = true;
	}

	// ���v
	for (int i = 1; i <= 7; i++) {
		if (tileCount[TILE_SUIT_HONORS + i] == 2) {tarzi++; atama = 1;}
		if (tileCount[TILE_SUIT_HONORS + i] >= 3) {mianzi++;}
	}

	// ���ʎq��ÞȂ�����ꍇ
	mianzi += gameStat->Player[playerID].MeldPointer;
	
	int mianziCount = 0;
	if (mianzi + tarzi > 4) {
		// �ʎq�����̂Ƃ�
		shanten = 8 - (mianzi * 2) - (4 - mianzi);
		// �ʎq�����ł��A�������鎞�͓���������
		if (atama) shanten -= 1;
	} else {
		// �����łȂ��Ƃ�
		shanten = 8 - (mianzi * 2) - tarzi;
	}

	return shanten;
}

SHANTEN calcShantenChiitoi(GameTable* gameStat, PLAYER_ID playerID, TileCount& tileCount)
{ // ���Ύq�ɑ΂�������������߂�B
	SHANTEN shanten = 6;
	for (int i = 0; i < TILE_NONFLOWER_MAX; i++)
		// �P���ɑΎq�̐��𒲂ׂ�΂悢
		// �������A�����v�S����Ύq�Q�Ƃ��Ďg���Ă͂Ȃ�Ȃ�
		if (tileCount[i] >= 2) shanten--;
	// �Í�������ꍇ�ɒ��v�Ƃ݂Ȃ��Ȃ��悤�ɂ���
	for (int i = 0; i < TILE_NONFLOWER_MAX; i++)
		if ((tileCount[i] >= 3)&&(shanten < 1)) shanten++;
	// ���ʎq��ÞȂ�����ꍇ�͎��Ύq�͕s�\
	if (gameStat->Player[playerID].MeldPointer > 0) shanten = SHANTEN_IMPOSSIBLE;

	return shanten;
}

SHANTEN calcShantenKokushi(GameTable* gameStat, PLAYER_ID playerID, TileCount& tileCount)
{ // ���m���o�ɑ΂�������������߂�B
	if (chkGameType(gameStat, SanmaS)) return SHANTEN_IMPOSSIBLE; // ���v�O���ł͕s�\

	tileCode YaojiuPai[13] = {
		CharacterOne, CharacterNine, CircleOne, CircleNine, BambooOne, BambooNine,
		EastWind, SouthWind, WestWind, NorthWind, WhiteDragon, GreenDragon, RedDragon
	};
	SHANTEN shanten = 13; bool atama = false;
	for (int i = 0; i < 13; i++) {
		// ���I��v�P��ނɂ��A�P���J�E���g����B
		if (tileCount[YaojiuPai[i]] >= 2) atama = true; // �A�^�}���
		if (tileCount[YaojiuPai[i]] >= 1) shanten--;
	}
	/* ����������ꍇ */
	if (atama) shanten--;
	// ���ʎq��ÞȂ�����ꍇ�͍��m���o���s�\
	if (gameStat->Player[playerID].MeldPointer > 0) shanten = SHANTEN_IMPOSSIBLE;

	return shanten;
}

