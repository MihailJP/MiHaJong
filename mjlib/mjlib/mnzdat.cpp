#include "mnzdat.h"

using namespace std;
using std::min;

/* 面子データ初期化 */

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
		o << "LZMAストリームのデコードに失敗しました。エラーコード: " << result;
		throw decompress_failure(o.str(), result);
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
		o << "面子構成データベースのSHA256ハッシュ値が一致しませんでした。";
		o << "期待されるハッシュ値: " << bytesToHexString(MDVEC(expectedDigest[0], expectedDigest[31])) << " ";
		o << "実際のハッシュ値: " << bytesToHexString(MDVEC(actualDigest[0], actualDigest[31]));
		throw hash_mismatch(o.str(),
			MDVEC(expectedDigest[0], expectedDigest[31]),
			MDVEC(actualDigest[0], actualDigest[31]));
	}
}

__declspec(dllexport) void initMentsuAnalysisDat() { // 面子データ初期化
	verifyMentsuAnalysisDat(decompressMentsuAnalysisDat());
}

/* 向聴数を計算する */

SHANTEN calcShanten(GameTable gameStat, PLAYER_ID playerID, shantenType mode)
{ // 向聴数を計算する
	/* 数牌それぞれの面子の数を数える */
	TileCount tileCount = countTilesInHand(gameStat, playerID);

	/* mode別分岐 */
	switch (mode) {
	case shantenRegular:
		/* あとで書く */
		break;
	case shantenPairs:
		/* あとで書く */
		break;
	case shantenOrphans:
		/* あとで書く */
		break;
	case shantenStellar:
		/* あとで書く */
		break;
	case shantenCivilWar:
		/* あとで書く */
		break;
	case shantenSyzygy:
		/* あとで書く */
		break;
	case shantenQuanbukao:
		/* あとで書く */
		break;
	default:
		/* 全部求めて一番和了に近いやつを返す */
		/* あとで書く */
		break;
	}
	return 0; //未完成
}

SHANTEN calcShantenRegular(GameTable gameStat, PLAYER_ID playerID, TileCount tileCount)
{ // 面子手の向聴数を求める
	SHANTEN shanten = 8; // 全く揃ってないてんでバラバラだったら面子手に対して8向聴（七対子に対してなら6向聴になる）

	// 数牌
	int mianzi = 0; int tarzi = 0; int atama = 0; // 面子塔子雀頭の数
	for (int suit = 0; suit < TILE_NUMERAL_COLORS; suit++) {
		unsigned int statcode = 0; unsigned int qDigit = 1;
		for (int i = 1; i <= 9; i++) {
			statcode += min(tileCount.count[suit * TILE_SUIT_STEP + i], 4) * qDigit;
			qDigit *= 5;
		}
		uint8_t tmpdat = mentsuAnalysisDat[statcode];
		mianzi += (tmpdat & 0x70) >> 4;
		tarzi += tmpdat & 0x0f;
		if (tmpdat & 0x80) atama = 1;
	}

	// 字牌
	for (int i = 1; i <= 7; i++) {
		if (tileCount.count[TILE_SUIT_HONORS + i] == 2) {tarzi++; atama = 1;}
		if (tileCount.count[TILE_SUIT_HONORS + i] >= 3) {mianzi++;}
	}

	// 未完成
	return shanten;
}
