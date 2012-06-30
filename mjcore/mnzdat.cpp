#include "mnzdat.h"

using namespace std;
using std::min;

/* 面子データ初期化 */

static uint8_t* mentsuAnalysisDat = NULL;

void LoadFileInResource(int name, int type, DWORD& size, const uint8_t*& data) {
	HRSRC rc = ::FindResource(dllInst, MAKEINTRESOURCE(name), MAKEINTRESOURCE(type));
	assert(rc != NULL);
	HGLOBAL rcData = ::LoadResource(dllInst, rc);
	assert(rcData != NULL);
	size = ::SizeofResource(dllInst, rc);
	assert(size != 0);
	data = static_cast<const uint8_t*>(::LockResource(rcData));
}

size_t decompressMentsuAnalysisDat() {
	DWORD size = 0; size_t uncompressedSize;
	const uint8_t* compressedBuf = NULL;
	int result;
	LoadFileInResource(IDR_LZMA_STREA1, LZMA_STREAM, size, compressedBuf);
	assert(size > 13);
	uint8_t* compressedMentsuDat = (uint8_t *)malloc(size+1);
	memcpy(compressedMentsuDat, compressedBuf, size);
	compressedMentsuDat[size] = 0;
	uncompressedSize = *((size_t *)(compressedMentsuDat+5));
	mentsuAnalysisDat = (uint8_t *)malloc(uncompressedSize);
	result = LzmaUncompress(mentsuAnalysisDat, &uncompressedSize,
		(const uint8_t *)(compressedMentsuDat+13),
		(SizeT *)&size, (const uint8_t *)compressedMentsuDat, 5);
	if (result != SZ_OK) {
		ostringstream o, os;
		o << "LZMAストリームのデコードに失敗しました。ファイルが壊れている虞があります。";
		fatal(o.str());
		os << "エラーコード: " << result;
		info(os.str()); o << endl << os; os.str("");
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
		ostringstream o, os;
		o << "面子構成データベースのSHA256ハッシュ値が一致しませんでした。";
		o << "ファイルが壊れている虞があります。";
		fatal(o.str());
		os << "期待されるハッシュ値: " << bytesToHexString(MDVEC(expectedDigest[0], expectedDigest[31]));
		info(os.str()); o << endl << os; os.str("");
		os << "実際のハッシュ値: " << bytesToHexString(MDVEC(actualDigest[0], actualDigest[31]));
		info(os.str()); o << endl << os; os.str("");
		throw hash_mismatch(o.str(),
			MDVEC(expectedDigest[0], expectedDigest[31]),
			MDVEC(actualDigest[0], actualDigest[31]));
	}
	else {
		info("面子構成データベースのSHA256ハッシュ値の照合に成功しました。");
	}
}

__declspec(dllexport) void initMentsuAnalysisDat() { // 面子データ初期化
	try {
		verifyMentsuAnalysisDat(decompressMentsuAnalysisDat());
	}
	catch (decompress_failure& e) {
		MessageBox(NULL, (LPCSTR)e.what(), (LPCSTR)"LZMA decompression error",
			MB_OK | MB_ICONERROR | MB_SETFOREGROUND);
		terminate();
	}
	catch (hash_mismatch& e) {
		MessageBox(NULL, (LPCSTR)e.what(), (LPCSTR)"SHA256 verification error",
			MB_OK | MB_ICONERROR | MB_SETFOREGROUND);
		terminate();
	}
}

/* 向聴数を計算する */

MJCORE SHANTEN calcShanten(GameTable* gameStat, PLAYER_ID playerID, shantenType mode)
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
		return calcShantenStellar(gameStat, playerID, tileCount, true);
	case shantenCivilWar:
		return calcShantenCivilWar(gameStat, playerID, tileCount);
	case shantenSyzygy:
		return calcShantenSyzygy(gameStat, playerID, tileCount);
	case shantenQuanbukao:
		return calcShantenStellar(gameStat, playerID, tileCount, false);
	default:
		/* 全部求めて一番和了に近いやつを返す */
		SHANTEN shanten, tmpShanten;
		shanten = calcShantenRegular(gameStat, playerID, tileCount);
		tmpShanten = calcShantenChiitoi(gameStat, playerID, tileCount); if (tmpShanten < shanten) shanten = tmpShanten;
		tmpShanten = calcShantenKokushi(gameStat, playerID, tileCount); if (tmpShanten < shanten) shanten = tmpShanten;
		tmpShanten = calcShantenStellar(gameStat, playerID, tileCount, true); if (tmpShanten < shanten) shanten = tmpShanten;
		tmpShanten = calcShantenCivilWar(gameStat, playerID, tileCount); if (tmpShanten < shanten) shanten = tmpShanten;
		tmpShanten = calcShantenSyzygy(gameStat, playerID, tileCount); if (tmpShanten < shanten) shanten = tmpShanten;
		tmpShanten = calcShantenStellar(gameStat, playerID, tileCount, false); if (tmpShanten < shanten) shanten = tmpShanten;
		return shanten;
	}
}
__declspec(dllexport) int calcShanten(GameTable* gameStat, int playerID, int mode)
{
	return (int)calcShanten(gameStat, (PLAYER_ID)playerID, (shantenType)mode);
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

SHANTEN calcShantenStellar(GameTable* gameStat, PLAYER_ID playerID, TileCount& tileCount, bool qixing)
{ // 特殊：七星不靠/全不靠の向聴数を求める
	if ((getRule(RULE_STELLAR_UUSHII) == 0)&&(qixing)) return SHANTEN_IMPOSSIBLE;
	else if ((getRule(RULE_QUANBUKAO) == 0)&&(!qixing)) return SHANTEN_IMPOSSIBLE;

	SHANTEN shanten = 999;
	tileCode QixingZiPai[7] = {
		EastWind, SouthWind, WestWind, NorthWind, WhiteDragon, GreenDragon, RedDragon
	};
	int qixingZiPaiCount = 0;
	for (int i = 0; i < 7; i++)
		if (tileCount[QixingZiPai[i]] >= 1)
			qixingZiPaiCount++;
	for (int i = 0; i < 6; i++) {
		tileCode QixingPai[9];
		switch (i) {
		case 0: case 1:
			QixingPai[0] = CharacterOne; QixingPai[1] = CharacterFour;
			QixingPai[2] = CharacterSeven; break;
		case 2: case 3:
			QixingPai[0] = CharacterTwo; QixingPai[1] = CharacterFive;
			QixingPai[2] = CharacterEight; break;
		case 4: case 5:
			QixingPai[0] = CharacterThree; QixingPai[1] = CharacterSix;
			QixingPai[2] = CharacterNine; break;
		}
		switch (i) {
		case 2: case 4:
			QixingPai[3] = CircleOne; QixingPai[4] = CircleFour;
			QixingPai[5] = CircleSeven; break;
		case 0: case 5:
			QixingPai[3] = CircleTwo; QixingPai[4] = CircleFive;
			QixingPai[5] = CircleEight; break;
		case 1: case 3:
			QixingPai[3] = CircleThree; QixingPai[4] = CircleSix;
			QixingPai[5] = CircleNine; break;
		}
		switch (i) {
		case 3: case 5:
			QixingPai[6] = BambooOne; QixingPai[7] = BambooFour;
			QixingPai[8] = BambooSeven; break;
		case 1: case 4:
			QixingPai[6] = BambooTwo; QixingPai[7] = BambooFive;
			QixingPai[8] = BambooEight; break;
		case 0: case 2:
			QixingPai[6] = BambooThree; QixingPai[7] = BambooSix;
			QixingPai[8] = BambooNine; break;
		}
		int qixingShuPaiCount = 0;
		for (int i = 0; i < 9; i++)
			if ((tileCount[QixingPai[i]] >= 1) &&
				((qixingShuPaiCount < 7)||(!qixing))) qixingShuPaiCount++;
		SHANTEN tmpShanten = 13 - qixingZiPaiCount - qixingShuPaiCount;
		// 鳴き面子や暗槓がある場合は不可能
		if (gameStat->Player[playerID].MeldPointer > 0) shanten = SHANTEN_IMPOSSIBLE;
		if (tmpShanten < shanten) shanten = tmpShanten;
	}

	return shanten;
}

SHANTEN calcShantenCivilWar(GameTable* gameStat, PLAYER_ID playerID, TileCount& tileCount)
{ // 特殊：南北戦争の向聴数を求める
	if (getRule(RULE_CIVIL_WAR) == 0) return SHANTEN_IMPOSSIBLE;

	SHANTEN shanten = 13;
	// 以下、一枚ずつ調べる
	for (int i = 0; i < 6; i++) {
		TileCount tileCountTmp;
		for (int j = 0; j < TILE_CODE_MAXIMUM; j++) tileCountTmp[j] = tileCount[j];
		tileCode CivilWarPai[NUM_OF_TILES_IN_HAND] = {
			NoTile, NoTile, NoTile, NoTile,
			NoTile, NoTile, NoTile, NoTile,
			SouthWind, SouthWind, SouthWind,
			NorthWind, NorthWind, NorthWind
		};
		switch (i) {
		case 0: case 1:
			CivilWarPai[0] = CharacterOne; CivilWarPai[1] = CivilWarPai[2] = CharacterEight;
			CivilWarPai[3] = CharacterOne; break;
		case 2: case 3:
			CivilWarPai[0] = CircleOne; CivilWarPai[1] = CivilWarPai[2] = CircleEight;
			CivilWarPai[3] = CircleOne; break;
		case 4: case 5:
			CivilWarPai[0] = BambooOne; CivilWarPai[1] = CivilWarPai[2] = BambooEight;
			CivilWarPai[3] = BambooOne; break;
		}
		switch (i) {
		case 2: case 4:
			CivilWarPai[4] = CharacterOne; CivilWarPai[5] = CivilWarPai[6] = CharacterEight;
			CivilWarPai[7] = CharacterFive; break;
		case 0: case 5:
			CivilWarPai[4] = CircleOne; CivilWarPai[5] = CivilWarPai[6] = CircleEight;
			CivilWarPai[7] = CircleFive; break;
		case 1: case 3:
			CivilWarPai[4] = BambooOne; CivilWarPai[5] = CivilWarPai[6] = BambooEight;
			CivilWarPai[7] = BambooFive; break;
		}
		int civilWarPaiCount = 0;
		for (int j = 0; j < NUM_OF_TILES_IN_HAND; j++) {
			if (tileCountTmp[CivilWarPai[j]] >= 1) {
				civilWarPaiCount++;
				tileCountTmp[CivilWarPai[j]]--;
			}
		}
		SHANTEN tmpShanten = 13 - civilWarPaiCount;
		// 鳴き面子や暗槓がある場合は考えない
		if (gameStat->Player[playerID].MeldPointer > 0) shanten = SHANTEN_IMPOSSIBLE;
		if (tmpShanten < shanten) shanten = tmpShanten;
	}

	return shanten;
}

SHANTEN calcShantenSyzygy(GameTable* gameStat, PLAYER_ID playerID, TileCount& tileCount)
{ // 特殊：惑星直列の向聴数を求める
	if (getRule(RULE_SYZYGY) == 0) return SHANTEN_IMPOSSIBLE;

	// 以下、一枚ずつ調べる
	TileCount tileCountTmp;
	for (int i = 0; i < TILE_CODE_MAXIMUM; i++) tileCountTmp[i] = tileCount[i];
	tileCode syzygyPai[NUM_OF_TILES_IN_HAND] = {
		CircleOne, CircleOne, CircleTwo,
		CircleThree, CircleThree, CircleFour,
		CircleFive, CircleFive, CircleSix,
		CircleSeven, CircleSeven, CircleEight,
		CircleNine, CircleNine
	};
	int syzygyPaiCount = 0;
	for (int i = 0; i < NUM_OF_TILES_IN_HAND; i++) {
		if (tileCountTmp[syzygyPai[i]] >= 1) {
			syzygyPaiCount++;
			tileCountTmp[syzygyPai[i]]--;
		}
	}
	// 鳴き面子や暗槓がある場合は考えない

	return (gameStat->Player[playerID].MeldPointer > 0) ? SHANTEN_IMPOSSIBLE : (13 - syzygyPaiCount);
}
