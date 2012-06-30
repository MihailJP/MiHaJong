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
		o << "LZMAストリームのデコードに失敗しました。ファイルが壊れている虞があります。エラーコード: " << result;
		fatal(o.str());
		throw decompress_failure(o.str(), result);
	}
	else {
		info("LZMAストリームをデコードしました。");
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
		o << "ファイルが壊れている虞があります。";
		o << "期待されるハッシュ値: " << bytesToHexString(MDVEC(expectedDigest[0], expectedDigest[31])) << " ";
		o << "実際のハッシュ値: " << bytesToHexString(MDVEC(actualDigest[0], actualDigest[31]));
		fatal(o.str());
		throw hash_mismatch(o.str(),
			MDVEC(expectedDigest[0], expectedDigest[31]),
			MDVEC(actualDigest[0], actualDigest[31]));
	}
	else {
		info("面子構成データベースのSHA256ハッシュ値の照合に成功しました。");
	}
}

__declspec(dllexport) void initMentsuAnalysisDat() { // 面子データ初期化
	verifyMentsuAnalysisDat(decompressMentsuAnalysisDat());
}

/* 向聴数を計算する */

SHANTEN calcShanten(GameTable* gameStat, PLAYER_ID playerID, shantenType mode)
{ // 向聴数を計算する
	/* 数牌それぞれの面子の数を数える */
	TileCount tileCount = countTilesInHand(gameStat, playerID);

	/* mode別分岐 */
	switch (mode) {
	case shantenRegular:
		return calcShantenRegular(gameStat, playerID, tileCount);
	case shantenPairs:
		return calcShantenChiitoi(gameStat, playerID, tileCount);
	case shantenOrphans:
		return calcShantenKokushi(gameStat, playerID, tileCount);
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

SHANTEN calcShantenRegular(GameTable* gameStat, PLAYER_ID playerID, TileCount& tileCount)
{ // 面子手の向聴数を求める
	SHANTEN shanten = 8; // 全く揃ってないてんでバラバラだったら面子手に対して8向聴（七対子に対してなら6向聴になる）

	// 数牌
	int mianzi = 0; int tarzi = 0; bool atama = false; // 面子塔子雀頭の数
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

	// 字牌
	for (int i = 1; i <= 7; i++) {
		if (tileCount[TILE_SUIT_HONORS + i] == 2) {tarzi++; atama = 1;}
		if (tileCount[TILE_SUIT_HONORS + i] >= 3) {mianzi++;}
	}

	// 鳴き面子や暗槓がある場合
	mianzi += gameStat->Player[playerID].MeldPointer;
	
	int mianziCount = 0;
	if (mianzi + tarzi > 4) {
		// 面子多多のとき
		shanten = 8 - (mianzi * 2) - (4 - mianzi);
		// 面子多多でも、頭がある時は頭も数える
		if (atama) shanten -= 1;
	} else {
		// そうでないとき
		shanten = 8 - (mianzi * 2) - tarzi;
	}

	return shanten;
}

SHANTEN calcShantenChiitoi(GameTable* gameStat, PLAYER_ID playerID, TileCount& tileCount)
{ // 七対子に対する向聴数を求める。
	SHANTEN shanten = 6;
	for (int i = 0; i < TILE_NONFLOWER_MAX; i++)
		// 単純に対子の数を調べればよい
		// ただし、同じ牌４枚を対子２つとして使ってはならない
		if (tileCount[i] >= 2) shanten--;
	// 暗刻がある場合に聴牌とみなさないようにする
	for (int i = 0; i < TILE_NONFLOWER_MAX; i++)
		if ((tileCount[i] >= 3)&&(shanten < 1)) shanten++;
	// 鳴き面子や暗槓がある場合は七対子は不可能
	if (gameStat->Player[playerID].MeldPointer > 0) shanten = SHANTEN_IMPOSSIBLE;

	return shanten;
}

SHANTEN calcShantenKokushi(GameTable* gameStat, PLAYER_ID playerID, TileCount& tileCount)
{ // 国士無双に対する向聴数を求める。
	if (chkGameType(gameStat, SanmaS)) return SHANTEN_IMPOSSIBLE; // 数牌三麻では不可能

	tileCode YaojiuPai[13] = {
		CharacterOne, CharacterNine, CircleOne, CircleNine, BambooOne, BambooNine,
		EastWind, SouthWind, WestWind, NorthWind, WhiteDragon, GreenDragon, RedDragon
	};
	SHANTEN shanten = 13; bool atama = false;
	for (int i = 0; i < 13; i++) {
		// ヤオ九牌１種類につき、１をカウントする。
		if (tileCount[YaojiuPai[i]] >= 2) atama = true; // アタマ候補
		if (tileCount[YaojiuPai[i]] >= 1) shanten--;
	}
	/* 雀頭がある場合 */
	if (atama) shanten--;
	// 鳴き面子や暗槓がある場合は国士無双も不可能
	if (gameStat->Player[playerID].MeldPointer > 0) shanten = SHANTEN_IMPOSSIBLE;

	return shanten;
}

