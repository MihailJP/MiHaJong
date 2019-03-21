#include "haifu.h"

#include <sstream>
#include <fstream>
#include <iomanip>
#include <string>
#include <cassert>
#ifdef _WIN32
#include <windows.h>
#endif /*_WIN32*/
#include "../common/largenum.h"
#include "envtbl.h"
#include "../common/version.h"
#include "ruletbl.h"
#include "yaku/yaku.h"

/* レガシー牌譜形式（プレーンテキスト、HTML）は廃止しました。XMLに統一します。 */

/* 雀牌の名前データ */
const std::array<CodeConv::tstring, TileFlowerMax> haifu::Xtilerefcode = {
	_T(""), _T("&m1;"),     _T("&m2;"),     _T("&m3;"),     _T("&m4;"),     _T("&m5;"),    _T("&m6;"),    _T("&m7;"),     _T("&m8;"),    _T("&m9;"),
	_T(""), _T("&p1;"),     _T("&p2;"),     _T("&p3;"),     _T("&p4;"),     _T("&p5;"),    _T("&p6;"),    _T("&p7;"),     _T("&p8;"),    _T("&p9;"),
	_T(""), _T("&s1;"),     _T("&s2;"),     _T("&s3;"),     _T("&s4;"),     _T("&s5;"),    _T("&s6;"),    _T("&s7;"),     _T("&s8;"),    _T("&s9;"),
	_T(""), _T("&east;"),   _T("&south;"),  _T("&west;"),   _T("&north;"),  _T("&white;"), _T("&green;"), _T("&red;"),    _T(""),        _T(""),
	_T(""), _T("&m1;"),     _T("&m2;"),     _T("&m3;"),     _T("&m4;"),     _T("&m5;"),    _T("&m6;"),    _T("&m7;"),     _T("&m8;"),    _T("&m9;"),
	_T(""), _T("&p1;"),     _T("&p2;"),     _T("&p3;"),     _T("&p4;"),     _T("&p5;"),    _T("&p6;"),    _T("&p7;"),     _T("&p8;"),    _T("&p9;"),
	_T(""), _T("&s1;"),     _T("&s2;"),     _T("&s3;"),     _T("&s4;"),     _T("&s5;"),    _T("&s6;"),    _T("&s7;"),     _T("&s8;"),    _T("&s9;"),
	_T(""), _T("&east;"),   _T("&south;"),  _T("&west;"),   _T("&north;"),  _T("&white;"), _T("&green;"), _T("&red;"),    _T(""),        _T(""),
	_T(""), _T("&m1;"),     _T("&m2;"),     _T("&m3;"),     _T("&m4;"),     _T("&m5;"),    _T("&m6;"),    _T("&m7;"),     _T("&m8;"),    _T("&m9;"),
	_T(""), _T("&p1;"),     _T("&p2;"),     _T("&p3;"),     _T("&p4;"),     _T("&p5;"),    _T("&p6;"),    _T("&p7;"),     _T("&p8;"),    _T("&p9;"),
	_T(""), _T("&s1;"),     _T("&s2;"),     _T("&s3;"),     _T("&s4;"),     _T("&s5;"),    _T("&s6;"),    _T("&s7;"),     _T("&s8;"),    _T("&s9;"),
	_T(""), _T("&east;"),   _T("&south;"),  _T("&west;"),   _T("&north;"),  _T("&white;"), _T("&green;"), _T("&red;"),    _T(""),        _T(""),
	_T(""), _T("&spring;"), _T("&summer;"), _T("&autumn;"), _T("&winter;"), _T("&joker;"), _T("&plum;"),  _T("&orchid;"), _T("&chrys;"), _T("&bamboo;"),
};

InfoByPlayer<LargeNum> haifu::origPoint;
CodeConv::tostringstream haifu::XMLhaifuBuffer, haifu::XhaifuBuffer, haifu::XhaifuBufferBody;
bool haifu::haifukanflag = false;

haifu::HaifuStreams haifu::XhaifuP;

#ifdef GUOBIAO
namespace { // 席替え後のプレイヤー番号対照
	unsigned currWindNum = 0;
	constexpr unsigned playerNumberList[4][4] = {
		{0, 1, 2, 3}, {1, 0, 3, 2}, {2, 3, 1, 0}, {3, 2, 0, 1}
	};
}
#endif /* GUOBIAO */

/* 牌譜記録用の補助ルーチン */
void haifu::tools::haifuskipX(PlayerID targetPlayer) {
	if (GameStat.chkGameType(SanmaT) && (targetPlayer == 3)) return; // 三麻で北家にあたる位置だったら帰る
	if (GameStat.chkGameType(Sanma4) && (GameStat.playerwind(targetPlayer) == sNorth)) return; // 四人三麻で北家だったら帰る
	checkCycle();
#ifdef GUOBIAO
	XhaifuBufferBody << _T("\t\t\t\t<turn player=\"player") << playerNumberList[currWindNum][static_cast<int>(targetPlayer)] << _T("\" />") << std::endl;
#else /* GUOBIAO */
	XhaifuBufferBody << _T("\t\t\t\t<turn player=\"player") << static_cast<int>(targetPlayer) << _T("\" />") << std::endl;
#endif /* GUOBIAO */
}
void haifu::tools::haifuskip(
	PlayerID PassivePlayer, PlayerID ActivePlayer) {
		if (playerRelative(ActivePlayer, PassivePlayer) == sOpposite) {
			// 対面からポンした場合
			haifuskipX(RelativePositionOf(ActivePlayer, sRight));
		}
		if (playerRelative(ActivePlayer, PassivePlayer) == sRight) {
			// 下家からポンした場合
			haifuskipX(RelativePositionOf(ActivePlayer, sRight));
			haifuskipX(RelativePositionOf(ActivePlayer, sOpposite));
		}
}

CodeConv::tstring haifu::tools::haifudoraClassX(doraCol Akadora) { // 赤牌黒牌の別(XML牌譜)
	switch (Akadora) {
	case AkaDora:
		return _T(" dora=\"red\"");
	case AoDora:
		return _T(" dora=\"blue\"");
	default:
		return _T("");
	}
}

void haifu::tools::recordDoraStream(CodeConv::tostringstream* const x, TileCode tmpDora) {
	if (tmpDora == Flower) {
		*x << _T("\t\t\t\t花") << std::endl;
	} else {
		*x << _T("\t\t\t\t<tile tile=\"") << Xtilerefcode[tmpDora] << _T("\" />") << std::endl;
	}
}

/* 牌を記録する */
void haifu::tools::recordTile_Inline(Tile tlCode, bool rotate) {
	XhaifuBufferBody << _T("<tile tile=\"") <<
		Xtilerefcode[static_cast<std::size_t>(tlCode.tile) + static_cast<std::size_t>(tlCode.red) * TileNonflowerMax] << _T('\"') <<
		haifudoraClassX(tlCode.red) << _T(" />");
}
void haifu::tools::recordTile_Inline(Tile tlCode, doraCol kakanCol) {
	XhaifuBufferBody << _T("<tile tile=\"") <<
		Xtilerefcode[static_cast<std::size_t>(tlCode.tile) + static_cast<std::size_t>(kakanCol) * TileNonflowerMax] << _T('\"') <<
		haifudoraClassX(kakanCol) << _T(" /><tile tile=\"") <<
		Xtilerefcode[static_cast<std::size_t>(tlCode.tile) + static_cast<std::size_t>(tlCode.red) * TileNonflowerMax] << _T('\"') <<
		haifudoraClassX(tlCode.red) << _T(" />");
}
void haifu::tools::recordTile_Table(Tile tlCode, CodeConv::tstring tagName, bool keepOpen) {
	XhaifuBufferBody << _T('<') << tagName << _T(" tile=\"") <<
		Xtilerefcode[static_cast<std::size_t>(tlCode.tile) + static_cast<std::size_t>(tlCode.red) * TileNonflowerMax] << _T('\"') <<
		haifudoraClassX(tlCode.red);
	if (!keepOpen) XhaifuBufferBody << _T(" />") << std::endl;
}

void haifu::tools::haifuwritetsumohai(
	PlayerID ActivePlayer, Tile tlCode,CodeConv::tstring XAttr) {
		checkCycle();
#ifdef GUOBIAO
		XhaifuBufferBody << _T("\t\t\t\t<turn player=\"player") << playerNumberList[currWindNum][static_cast<int>(ActivePlayer)] << _T("\">") << std::endl;
#else /* GUOBIAO */
		XhaifuBufferBody << _T("\t\t\t\t<turn player=\"player") << static_cast<int>(ActivePlayer) << _T("\">") << std::endl;
#endif /* GUOBIAO */
		XhaifuBufferBody << _T("\t\t\t\t\t");
		recordTile_Table(tlCode, _T("pick"), true);
		XhaifuBufferBody << XAttr << _T(" />") << std::endl;
}

void haifu::tools::haifuskipall(PlayerID PassivePlayer) {
	haifuskipX(RelativePositionOf(PassivePlayer, sRight));
	haifuskipX(RelativePositionOf(PassivePlayer, sOpposite));
	haifuskipX(RelativePositionOf(PassivePlayer, sLeft));
}

/* 一半荘分の牌譜バッファを初期化 */
void haifu::haifubufinit() {
#ifdef GUOBIAO
	currWindNum = 0;
#endif /* GUOBIAO */
	CodeConv::tostringstream headerTxt; headerTxt.str(_T(""));
	headerTxt << _T("MiHaJong ");
	switch (GameStat.gameType) {
		case SanmaSeto: headerTxt << _T("瀬戸内三麻"); break;
		case SanmaS:    headerTxt << _T("数牌三麻"); break;
		case Sanma4:    headerTxt << _T("四人三麻"); break;
		case Sanma:     headerTxt << _T("三人打ち"); break;
		case Yonma:     headerTxt << _T("四人打ち"); break;
		case GuobiaoMJ: headerTxt << _T("国標麻将"); break;
	}
	headerTxt << _T("牌譜データ Ver. ") << MIHAJONG_VER;

	/* XML牌譜 */
	XMLhaifuBuffer.str(_T("")); XhaifuBuffer.str(_T(""));
	XMLhaifuBuffer <<
#ifdef UNICODE
		_T("<?xml version=\"1.0\" encoding=\"UTF-8\"?>") <<
#else
		_T("<?xml version=\"1.0\" encoding=\"Shift_JIS\"?>") <<
#endif
		std::endl <<
		_T("<?xml-stylesheet href=\"haifu.xsl\" type=\"text/xsl\"?>") << std::endl <<
		_T("<!DOCTYPE haifu [") << std::endl <<
		_T("\t<!ENTITY east \"&#x1f000;\"><!ENTITY south \"&#x1f001;\"><!ENTITY west \"&#x1f002;\"><!ENTITY north \"&#x1f003;\">")
		_T("<!ENTITY red \"&#x1f004;\"><!ENTITY green \"&#x1f005;\"><!ENTITY white \"&#x1f006;\">") << std::endl <<
		_T("\t<!ENTITY m1 \"&#x1f007;\"><!ENTITY m2 \"&#x1f008;\"><!ENTITY m3 \"&#x1f009;\"><!ENTITY m4 \"&#x1f00a;\"><!ENTITY m5 \"&#x1f00b;\">")
		_T("<!ENTITY m6 \"&#x1f00c;\"><!ENTITY m7 \"&#x1f00d;\"><!ENTITY m8 \"&#x1f00e;\"><!ENTITY m9 \"&#x1f00f;\">") << std::endl <<
		_T("\t<!ENTITY s1 \"&#x1f010;\"><!ENTITY s2 \"&#x1f011;\"><!ENTITY s3 \"&#x1f012;\"><!ENTITY s4 \"&#x1f013;\"><!ENTITY s5 \"&#x1f014;\">")
		_T("<!ENTITY s6 \"&#x1f015;\"><!ENTITY s7 \"&#x1f016;\"><!ENTITY s8 \"&#x1f017;\"><!ENTITY s9 \"&#x1f018;\">") << std::endl <<
		_T("\t<!ENTITY p1 \"&#x1f019;\"><!ENTITY p2 \"&#x1f01a;\"><!ENTITY p3 \"&#x1f01b;\"><!ENTITY p4 \"&#x1f01c;\"><!ENTITY p5 \"&#x1f01d;\">")
		_T("<!ENTITY p6 \"&#x1f01e;\"><!ENTITY p7 \"&#x1f01f;\"><!ENTITY p8 \"&#x1f020;\"><!ENTITY p9 \"&#x1f021;\">") << std::endl <<
		_T("\t<!ENTITY plum \"&#x1f022;\"><!ENTITY orchid \"&#x1f023;\"><!ENTITY bamboo \"&#x1f024;\"><!ENTITY chrys \"&#x1f025;\"><!ENTITY chrysanthemum \"&#x1f025;\">")
		_T("<!ENTITY spring \"&#x1f026;\"><!ENTITY summer \"&#x1f027;\"><!ENTITY autumn \"&#x1f028;\"><!ENTITY winter \"&#x1f029;\">")
		_T("<!ENTITY joker \"&#x1f02a;\"><!ENTITY back \"&#x1f02b;\">") << std::endl <<
		_T("]>") << std::endl <<
		_T("<haifu xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance")
		_T("\" xsi:noNamespaceSchemaLocation=\"haifu.xsd\">") << std::endl <<
		_T("\t<match-description>") << std::endl <<
		_T("\t\t<title>") << headerTxt.str() << _T("</title>") << std::endl;
	tools::haifuRecTime(_T("time-began"));
	XhaifuBuffer << _T("\t\t<ruleset system=\"") <<
		(GameStat.chkGameType(Yonma) ? _T("richi") :
		GameStat.chkGameType(Sanma) ? _T("sanma") :
		GameStat.chkGameType(Sanma4) ? _T("sanma-4players") :
		GameStat.chkGameType(SanmaS) ? _T("sanma-numerals") :
		GameStat.chkGameType(SanmaSeto) ? _T("sanma-setouchi") :
		GameStat.chkGameType(GuobiaoMJ) ? _T("guobiao") : _T(""))
		<< _T("\">") << std::endl;
	RuleData::forEachRule([&](std::string key, std::string val) -> void {
		if (((val != "no") || (RuleData::getRuleItemTag(key, 0) != "no")) &&
			((key.substr(0, 12) != "point_basis_") || RuleData::chkRule("point_basis", "custom")) &&
			((key.substr(0, 15) != "starting_point_") || RuleData::chkRule("starting_point", "custom")) &&
			(val != "N/A"))
			XhaifuBuffer << _T("\t\t\t<rule key=\"") << CodeConv::EnsureTStr(key) << _T("\" val=\"") <<
			CodeConv::EnsureTStr(val) << _T("\" />") << std::endl;
	});
	XhaifuBuffer << _T("\t\t</ruleset>") << std::endl;
	XhaifuBuffer << _T("\t\t<player-description>") << std::endl;
	const CodeConv::tstring nomenVenti[4] = {_T("east"), _T("south"), _T("west"), _T("north")};
	for (int i = 0; i < Players; ++i)
#ifdef GUOBIAO
		XhaifuBuffer << _T("\t\t\t<player-data id=\"player") << playerNumberList[currWindNum][i] << _T("\" name=\"") <<
#else /* GUOBIAO */
		XhaifuBuffer << _T("\t\t\t<player-data id=\"player") << i << _T("\" name=\"") <<
#endif /* GUOBIAO */
		EnvTable::Instantiate()->PlayerDat[i].PlayerName << _T("\" starting-wind=\"") <<
		nomenVenti[i] << _T("\" />") << std::endl;
	XhaifuBuffer <<
		_T("\t\t</player-description>") << std::endl <<
		_T("\t</match-description>") << std::endl;
}

void haifu::tools::haifuRecTime(CodeConv::tstring tagName) { // 現在時刻タグ
#ifdef _WIN32
	SYSTEMTIME currTime; GetLocalTime(&currTime);
	TIME_ZONE_INFORMATION tz; GetTimeZoneInformation(&tz);
	XMLhaifuBuffer << _T("\t\t<") << tagName << _T(">") <<
		std::setw(4) << std::setfill(_T('0')) << currTime.wYear << _T("-") <<
		std::setw(2) << std::setfill(_T('0')) << currTime.wMonth << _T("-") <<
		std::setw(2) << std::setfill(_T('0')) << currTime.wDay << _T("T") <<
		std::setw(2) << std::setfill(_T('0')) << currTime.wHour << _T(":") <<
		std::setw(2) << std::setfill(_T('0')) << currTime.wMinute << _T(":") <<
		std::setw(2) << std::setfill(_T('0')) << currTime.wSecond << _T(".") <<
		std::setw(3) << std::setfill(_T('0')) << currTime.wMilliseconds <<
		std::showpos << std::setw(3) << std::setfill(_T('0')) << std::internal << ((-tz.Bias) / 60) << _T(":") <<
		std::noshowpos << std::setw(2) << std::setfill(_T('0')) << ((-tz.Bias) % 60) <<
		_T("</") << tagName << _T(">") << std::endl;
#else /*_WIN32*/
	timespec tempus; clock_gettime(CLOCK_REALTIME, &tempus);
	tm currTime;
	localtime_s(&currTime, &tempus.tv_sec);
	constexpr signed long tz = []() -> signed long {
		time_t t1 = 86400; // GNU Cはそうではないが、time_tがunsignedの処理系を見たことがあるので86400とする
		tm* tmDat = gmtime(&t1); // 協定世界時を算出
		time_t t2 = mktime(tmDat); // わざと地方時と解釈することで時差を求める
		return t1 - t2; // 秒単位で時差を返す。日本時間だったら32400となる
	}();
	XMLhaifuBuffer << _T("\t\t<") << tagName << _T(">") <<
		std::setw(4) << std::setfill(_T('0')) << (currTime.tm_year + 1900) << _T("-") <<
		std::setw(2) << std::setfill(_T('0')) << (currTime.tm_mon + 1) << _T("-") <<
		std::setw(2) << std::setfill(_T('0')) << currTime.tm_mday << _T("T") <<
		std::setw(2) << std::setfill(_T('0')) << currTime.tm_hour << _T(":") <<
		std::setw(2) << std::setfill(_T('0')) << currTime.tm_min << _T(":") <<
		std::setw(2) << std::setfill(_T('0')) << currTime.tm_sec << _T(".") <<
		std::setw(3) << std::setfill(_T('0')) << (tempus.tv_nsec / 1000000) <<
		std::showpos << std::setw(3) << std::setfill(_T('0')) << std::internal << ((tz + 86400) / 3600 - (86400 / 3600)) << _T(":") <<
		std::noshowpos << std::setw(2) << std::setfill(_T('0')) << ((tz + 86400) % 3600 / 60) <<
		_T("</") << tagName << _T(">") << std::endl;
#endif /*_WIN32*/
}

/* 一局分の牌譜バッファを初期化 */
void haifu::haifuinit() {
	for (int p = 0; p < Players; p++) {
		XhaifuP.streamDat[p].haipai.str(_T(""));
		XhaifuP.streamDat[p].tsumo.str(_T(""));
		XhaifuP.streamDat[p].tsumolabel.str(_T(""));
		XhaifuP.streamDat[p].sutehai.str(_T(""));
		XhaifuP.streamDat[p].sutehailabel.str(_T(""));
		XhaifuP.streamDat[p].final.str(_T(""));
	}
	XhaifuP.dora.str(_T(""));
	XhaifuP.uraDora.str(_T(""));
	XhaifuP.aliceDora.str(_T(""));
	XhaifuP.aliceDoraMax.str(_T(""));
	XhaifuP.resultDesc.str(_T(""));
	XhaifuBufferBody.str(_T(""));
}

/* 配牌を牌譜に記録 */
void haifu::haifurechaipai(const GameTable* const gameStat) {
#ifdef GUOBIAO
	currWindNum = gameStat->GameRound / Players;
#endif /* GUOBIAO */
	XhaifuBufferBody << _T("\t\t<distribution>") << std::endl;
	for (int p = 0; p < Players; p++) {
#ifdef GUOBIAO
		XhaifuBufferBody << _T("\t\t\t<initial-hand player=\"player") << playerNumberList[currWindNum][p] << _T("\">") << std::endl;
#else /* GUOBIAO */
		XhaifuBufferBody << _T("\t\t\t<initial-hand player=\"player") << p << _T("\">") << std::endl;
#endif /* GUOBIAO */
		for (int i = 0; i < NumOfTilesInHand; i++) {
			if (gameStat->Player[p].Hand[i].tile != NoTile) {
				XhaifuBufferBody << _T("\t\t\t\t");
				tools::recordTile_Inline(gameStat->Player[p].Hand[i], false);
				XhaifuBufferBody << std::endl;
			}
		}
		XhaifuBufferBody << _T("\t\t\t</initial-hand>") << std::endl;
		origPoint[p] = gameStat->Player[p].PlayerScore;
	}
	XhaifuBufferBody << _T("\t\t</distribution>") << std::endl;
	XhaifuBufferBody << _T("\t\t<progress>") << std::endl;
}

/* ドラを牌譜に記録 */
void haifu::haifurecdora(TileCode tmpDora) {
	tools::recordDoraStream(&XhaifuP.dora, tmpDora);
}

/* 裏ドラを牌譜に記録 */
void haifu::haifurecuradora(TileCode tmpDora) {
	tools::recordDoraStream(&XhaifuP.uraDora, tmpDora);
}

/* アリスドラを牌譜に記録 */
void haifu::haifurecalicedora(TileCode tmpDora) {
	tools::recordDoraStream(&XhaifuP.aliceDora, tmpDora);
}

/* アリスドラを更新 */
void haifu::haifualicedoraupd() {
	if (XhaifuP.aliceDora.str().length() > XhaifuP.aliceDoraMax.str().length()) {
		XhaifuP.aliceDoraMax.str(XhaifuP.aliceDora.str());
	}
}

/* アリスドラ初期化 */
void haifu::haifuresetalicedora() {
	XhaifuP.aliceDora.str(_T(""));
}

/* 摸打を牌譜に記録 */
void haifu::haifurecmota(const GameTable* const gameStat, const DiscardTileNum& DiscardTileIndex) {
	// ツモってきた牌を記録
	bool discard_through = false;
	if (gameStat->TianHuFlag) {
		// 親の１巡目
		tools::checkCycle(true);
#ifdef GUOBIAO
		XhaifuBufferBody << _T("\t\t\t\t<turn player=\"player") << playerNumberList[currWindNum][static_cast<int>(gameStat->CurrentPlayer.Active)] << _T("\">") << std::endl;
#else /* GUOBIAO */
		XhaifuBufferBody << _T("\t\t\t\t<turn player=\"player") << static_cast<int>(gameStat->CurrentPlayer.Active) << _T("\">") << std::endl;
#endif /* GUOBIAO */
	} else if (gameStat->statOfActive().Tsumohai().tile == NoTile) {
		// 鳴いた直後 (何もしない)
	} else if ((DiscardTileIndex.id) == (NumOfTilesInHand - 1)) {
		// ツモ切り
		discard_through = true;
		tools::checkCycle();
#ifdef GUOBIAO
		XhaifuBufferBody << _T("\t\t\t\t<turn player=\"player") << playerNumberList[currWindNum][static_cast<int>(gameStat->CurrentPlayer.Active)] << _T("\">") << std::endl;
#else /* GUOBIAO */
		XhaifuBufferBody << _T("\t\t\t\t<turn player=\"player") << static_cast<int>(gameStat->CurrentPlayer.Active) << _T("\">") << std::endl;
#endif /* GUOBIAO */
	} else {
		tools::haifuwritetsumohai(gameStat->CurrentPlayer.Active, gameStat->statOfActive().Tsumohai(), _T(""));
	}
	// 捨てた牌を記録
	XhaifuBufferBody << _T("\t\t\t\t\t");
	tools::recordTile_Table(gameStat->statOfActive().Hand[DiscardTileIndex.id], discard_through ? _T("discard-through") : _T("discard"), true);
}

/* 放銃したか否かを牌譜に記録 */
void haifu::haifurecfurikomi(const GameTable* const gameStat) {
	if (gameStat->statOfActive().RichiFlag.IppatsuFlag) {
		// 立直宣言牌の場合
		if (RonPlayers(gameStat) > 0) {
			// 立直宣言牌での振り込み
			XhaifuBufferBody << _T(" call=\"richi\" let-go=\"true\" />") << std::endl;
		} else {
			// 立直が通った場合
			XhaifuBufferBody << _T(" call=\"richi\" />") << std::endl;
		}
	} else {
		// それ以外の場合
		if (RonPlayers(gameStat) > 0) {
			// 通常の振り込み
			XhaifuBufferBody << _T(" let-go=\"true\" />") << std::endl;
		} else {
			// 何もなかった場合
			XhaifuBufferBody << _T(" />") << std::endl;
		}
	}
	XhaifuBufferBody << _T("\t\t\t\t</turn>") << std::endl;
}

/* チーしたことを牌譜に記録 */
void haifu::haifurecchi(const GameTable* const gameStat) {
	tools::haifuwritetsumohai(
		gameStat->CurrentPlayer.Passive,
		gameStat->CurrentDiscard, _T(" meld=\"sequence\""));
}
/* ポンしたことを牌譜に記録 */
void haifu::haifurecpon(const GameTable* const gameStat) {
	tools::haifuskip(
		gameStat->CurrentPlayer.Passive, gameStat->CurrentPlayer.Active);
	tools::haifuwritetsumohai(
		gameStat->CurrentPlayer.Passive,
		gameStat->CurrentDiscard, _T(" meld=\"triplet\""));
}
/* 自摸和したことを牌譜に記録 */
void haifu::haifurectsumo(const GameTable* const gameStat) {
	tools::haifuwritetsumohai(
		gameStat->CurrentPlayer.Active,
		gameStat->statOfActive().Tsumohai(),
		_T(" meld=\"mahjong\""));
		XhaifuBufferBody << _T("\t\t\t\t</turn>") << std::endl;
}
/* 大明槓したことを牌譜に記録 */
void haifu::haifurecminkan(const GameTable* const gameStat) {
	tools::haifuskip(gameStat->CurrentPlayer.Passive, gameStat->CurrentPlayer.Active);
	tools::haifuwritetsumohai(gameStat->CurrentPlayer.Passive,
		gameStat->CurrentDiscard, _T(" meld=\"quad\""));
	XhaifuBufferBody << _T("\t\t\t\t</turn>") << std::endl;
	// 牌譜の記述上は、嶺上牌の取得は次巡として扱う
	tools::haifuskipall(gameStat->CurrentPlayer.Passive);
}

/* 巡目区切りか */
void haifu::tools::checkCycle(bool reset) {
	static int cycle = 1, turn = 0;
	if (reset) {
		cycle = 1; turn = 0;
		XhaifuBufferBody << _T("\t\t\t<cycle ord=\"1\">") << std::endl;
	} else {
		if ((++turn) >= (GameStat.chkGameType(AllSanma) ? 3 : 4)) {
			++cycle; turn = 0;
			XhaifuBufferBody << _T("\t\t\t</cycle>") << std::endl <<
				_T("\t\t\t<cycle ord=\"") << cycle << _T("\">") << std::endl;
		}
	}
}

/* カンがあった時 */
void haifu::tools::kan_sub::recordKanOrFlower(
	const GameTable* const gameStat, const DiscardTileNum& DiscardTileIndex) {
		if ((gameStat->TianHuFlag)||((DiscardTileIndex.id) != (NumOfTilesInHand - 1))) {
			// 親の１巡目の場合か、ツモってきた牌以外をカンした場合
			if (gameStat->TianHuFlag) {
				checkCycle(true);
#ifdef GUOBIAO
				XhaifuBufferBody << _T("\t\t\t\t<turn player=\"player") << playerNumberList[currWindNum][static_cast<int>(gameStat->CurrentPlayer.Active)] << _T("\">") << std::endl;
#else /* GUOBIAO */
				XhaifuBufferBody << _T("\t\t\t\t<turn player=\"player") << static_cast<int>(gameStat->CurrentPlayer.Active) << _T("\">") << std::endl;
#endif /* GUOBIAO */
				XhaifuBufferBody << _T("\t\t\t\t\t");
				recordTile_Table(gameStat->statOfActive().Hand[DiscardTileIndex.id], _T("pick"), true);
				haifukanflag = true;
			} else if (gameStat->statOfActive().Tsumohai().tile ==
				gameStat->statOfActive().Hand[DiscardTileIndex.id].tile) {
					// ツモってきた牌と同じだった
					checkCycle();
#ifdef GUOBIAO
					XhaifuBufferBody << _T("\t\t\t\t<turn player=\"player") << playerNumberList[currWindNum][static_cast<int>(gameStat->CurrentPlayer.Active)] << _T("\">") << std::endl;
#else /* GUOBIAO */
					XhaifuBufferBody << _T("\t\t\t\t<turn player=\"player") << static_cast<int>(gameStat->CurrentPlayer.Active) << _T("\">") << std::endl;
#endif /* GUOBIAO */
					XhaifuBufferBody << _T("\t\t\t\t\t");
					recordTile_Table(gameStat->statOfActive().Hand[DiscardTileIndex.id], _T("pick"), true);
					haifukanflag = false;
			} else {
				haifuwritetsumohai(gameStat->CurrentPlayer.Active, gameStat->statOfActive().Tsumohai(), _T(""));
				XhaifuBufferBody << _T("\t\t\t\t\t");
				recordTile_Table(gameStat->statOfActive().Hand[DiscardTileIndex.id], _T("discard"), true);
				haifukanflag = true;
			}
		} else {
			checkCycle();
#ifdef GUOBIAO
			XhaifuBufferBody << _T("\t\t\t\t<turn player=\"player") << playerNumberList[currWindNum][static_cast<int>(gameStat->CurrentPlayer.Active)] << _T("\">") << std::endl;
#else /* GUOBIAO */
			XhaifuBufferBody << _T("\t\t\t\t<turn player=\"player") << static_cast<int>(gameStat->CurrentPlayer.Active) << _T("\">") << std::endl;
#endif /* GUOBIAO */
			XhaifuBufferBody << _T("\t\t\t\t\t");
			recordTile_Table(gameStat->statOfActive().Hand[DiscardTileIndex.id], _T("pick"), true);
			haifukanflag = false;
		}
}
/* 搶“北”を牌譜に記録 */
void inline haifu::tools::kan_sub::recordChanKan(const GameTable* const gameStat, CodeConv::tstring XAttrA, CodeConv::tstring XAttrB) {
	if (haifukanflag) {
		// 親の１巡目の場合か、ツモってきた牌以外をカンした場合
		XhaifuBufferBody << XAttrB << _T(" />") << std::endl;
	} else {
		XhaifuBufferBody << XAttrA << _T(" />") << std::endl;
	}
	XhaifuBufferBody << _T("\t\t\t\t</turn>") << std::endl;
}
/* 抜き北が成功した場合の牌譜処理 */
void inline haifu::tools::kan_sub::recordKan(const GameTable* const gameStat, CodeConv::tstring XAttrA, CodeConv::tstring XAttrB) {
	if (haifukanflag == 1) {
		// 親の１巡目の場合か、ツモってきた牌以外をカンした場合
		XhaifuBufferBody << XAttrB << _T(" />") << std::endl;
	} else {
		XhaifuBufferBody << XAttrA << _T(" />") << std::endl;
	}
	XhaifuBufferBody << _T("\t\t\t\t</turn>") << std::endl;
		
	// 牌譜の記述上は、嶺上牌の取得は次巡として扱う
	haifuskipall(gameStat->CurrentPlayer.Active);
}

/* 抜き北を牌譜に記録 */
void haifu::haifurecnorth(const GameTable* const gameStat, const DiscardTileNum& DiscardTileIndex) {
	tools::kan_sub::recordKanOrFlower(gameStat, DiscardTileIndex);
}
/* 搶“北”を牌譜に記録 */
void haifu::haifurecchanpei(const GameTable* const gameStat) {
	tools::kan_sub::recordChanKan(gameStat, _T(" meld=\"flower\" let-go=\"true\""), _T(" call=\"flower\" let-go=\"true\""));
}
/* 抜き北が成功した場合の牌譜処理 */
void haifu::haifurecnorthproc(const GameTable* const gameStat) {
	tools::kan_sub::recordKan(gameStat, _T(" meld=\"flower\""), _T(" call=\"flower\""));
}

/* 暗槓ないし加槓を牌譜に記録 */
void haifu::haifurecankan(const GameTable* const gameStat, const DiscardTileNum& DiscardTileIndex) {
	tools::kan_sub::recordKanOrFlower(gameStat, DiscardTileIndex);
}
/* 搶槓を牌譜に記録 */
void haifu::haifurecchankan(const GameTable* const gameStat) {
	tools::kan_sub::recordChanKan(gameStat, _T(" meld=\"quad\" let-go=\"true\""), _T(" call=\"quad\" let-go=\"true\""));
}
/* カンが成功した場合の牌譜処理 */
void haifu::haifureckanproc(const GameTable* const gameStat) {
	tools::kan_sub::recordKan(gameStat, _T(" meld=\"quad\""), _T(" call=\"quad\""));
}

/* 花牌を牌譜に記録 */
void haifu::haifurecflower(const GameTable* const gameStat, const DiscardTileNum& DiscardTileIndex) {
	tools::kan_sub::recordKanOrFlower(gameStat, DiscardTileIndex);
	tools::kan_sub::recordKan(gameStat, _T(" meld=\"flower\""), _T(" call=\"flower\""));
}

/* hfwriter */
void haifu::tools::hfwriter::hfWriteHead(const GameTable* const gameStat,
	int OrigTurn, int OrigHonba, bool tmpUraFlag, bool tmpAliceFlag,
	CodeConv::tstring ResultDesc, EndType RoundEndType) {
		XhaifuBuffer << _T("\t<round>") << std::endl << _T("\t\t<round-description>") << std::endl;
		const CodeConv::tstring windName[7] = {
			_T("east"), _T("south"), _T("west"), _T("north"), _T("white"), _T("green"), _T("red"),
		};
#ifdef GUOBIAO
		constexpr bool eastOnly = false;
#else /* GUOBIAO */
		const bool eastOnly = RuleData::chkRule("game_length", "twice_east_game") || RuleData::chkRule("game_length", "east_only_game");
#endif /* GUOBIAO */
		XhaifuBuffer << _T("\t\t\t<round-number prevailing-wind=\"") << windName[OrigTurn / Players] <<
			_T("\" rotation=\"") << ((eastOnly ? gameStat->LoopRound : 0) * Players + (OrigTurn % Players) + 1) <<
#ifndef GUOBIAO
			_T("\" counter=\"") << OrigHonba <<
#endif /* GUOBIAO */
			_T("\" />") << std::endl;
		XhaifuBuffer << _T("\t\t\t<result>") << ResultDesc << _T("</result>") << std::endl;
		XhaifuBuffer << _T("\t\t\t<dice face1=\"") << static_cast<int>(gameStat->Dice[0].Number) <<
			_T("\" face2=\"") << static_cast<int>(gameStat->Dice[1].Number) << _T("\" />") << std::endl;
#ifndef GUOBIAO
		if (RuleData::chkRule("dice_roll", "roll_twice"))
#endif /* GUOBIAO */
			XhaifuBuffer << _T("\t\t\t<dice face1=\"") << static_cast<int>(gameStat->Dice[2].Number) <<
			_T("\" face2=\"") << static_cast<int>(gameStat->Dice[3].Number) << _T("\" />") << std::endl;
#ifndef GUOBIAO
		XhaifuBuffer << _T("\t\t\t<dora>") << std::endl <<
			XhaifuP.dora.str() << _T("\t\t\t</dora>") << std::endl;
		if ((RoundEndType == Agari)&&(tmpUraFlag)&&(RuleData::chkRuleApplied("uradora")))
			XhaifuBuffer << _T("\t\t\t<uradora>") << std::endl <<
			XhaifuP.uraDora.str() << _T("\t\t\t</uradora>") << std::endl;
		if ((RoundEndType == Agari)&&(tmpAliceFlag)&&(RuleData::chkRuleApplied("alice")))
			XhaifuBuffer << _T("\t\t\t<alice>") << std::endl <<
			XhaifuP.aliceDoraMax.str() << _T("\t\t\t</alice>") << std::endl;
#endif /* GUOBIAO */
}

// 最終牌姿
void haifu::tools::hfwriter::finalformWriter::hfFinalForm(const GameTable* const gameStat, PlayerID player, EndType RoundEndType) {
	// 最終牌姿(純手牌のみ)
	bool agariFlag = false;
	XhaifuBufferBody << _T("\t\t\t\t<hand>") << std::endl;
	for (int i = 0; i < NumOfTilesInHand; i++) {
		if (gameStat->Player[player].Hand[i].tile != NoTile) {
			if (i == NumOfTilesInHand - 1) {
				if ((RoundEndType == Ryuukyoku)||(RoundEndType == Agari)||(RoundEndType == Chonbo)) {
					if (gameStat->TsumoAgariFlag) {
						XhaifuBufferBody << _T("\t\t\t\t</hand>") << std::endl;
						XhaifuBufferBody << _T("\t\t\t\t<finishing-tile finish-type=\"tsumo\">") << std::endl;
					} else {
						XhaifuBufferBody << _T("\t\t\t\t</hand>") << std::endl;
						XhaifuBufferBody << _T("\t\t\t\t<finishing-tile finish-type=\"ron\">") << std::endl;
					}
					agariFlag = true;
				}
			}
			XhaifuBufferBody << _T("\t\t\t\t\t");
			recordTile_Inline(gameStat->Player[player].Hand[i], false);
			XhaifuBufferBody << std::endl;
		}
	}
	if (agariFlag)
		XhaifuBufferBody << _T("\t\t\t\t</finishing-tile>") << std::endl;
	else
		XhaifuBufferBody << _T("\t\t\t\t</hand>") << std::endl;
}
void haifu::tools::hfwriter::finalformWriter::hfFlower(const GameTable* const gameStat, PlayerID player) {
	if (gameStat->Player[player].FlowerFlag.Spring || gameStat->Player[player].FlowerFlag.Summer ||
		gameStat->Player[player].FlowerFlag.Autumn || gameStat->Player[player].FlowerFlag.Winter ||
		gameStat->Player[player].FlowerFlag.Plum || gameStat->Player[player].FlowerFlag.Orchid ||
		gameStat->Player[player].FlowerFlag.Chrys || gameStat->Player[player].FlowerFlag.Bamboo)
		XhaifuBufferBody << _T("\t\t\t\t<flower>") << std::endl;
	if (gameStat->Player[player].FlowerFlag.Spring ||
		gameStat->Player[player].FlowerFlag.Summer ||
		gameStat->Player[player].FlowerFlag.Autumn ||
		gameStat->Player[player].FlowerFlag.Winter) {
			if (gameStat->Player[player].FlowerFlag.Spring)
				XhaifuBufferBody << _T("\t\t\t\t\t<tile tile=\"&spring;\" />") << std::endl;
			if (gameStat->Player[player].FlowerFlag.Summer)
				XhaifuBufferBody << _T("\t\t\t\t\t<tile tile=\"&summer;\" />") << std::endl;
			if (gameStat->Player[player].FlowerFlag.Autumn)
				XhaifuBufferBody << _T("\t\t\t\t\t<tile tile=\"&autumn;\" />") << std::endl;
			if (gameStat->Player[player].FlowerFlag.Winter)
				XhaifuBufferBody << _T("\t\t\t\t\t<tile tile=\"&winter;\" />") << std::endl;
	}
	if (gameStat->Player[player].FlowerFlag.Plum ||
		gameStat->Player[player].FlowerFlag.Orchid ||
		gameStat->Player[player].FlowerFlag.Chrys ||
		gameStat->Player[player].FlowerFlag.Bamboo) {
			if (gameStat->Player[player].FlowerFlag.Plum)
				XhaifuBufferBody << _T("\t\t\t\t\t<tile tile=\"&plum;\" />") << std::endl;
			if (gameStat->Player[player].FlowerFlag.Orchid)
				XhaifuBufferBody << _T("\t\t\t\t\t<tile tile=\"&orchid;\" />") << std::endl;
			if (gameStat->Player[player].FlowerFlag.Chrys)
				XhaifuBufferBody << _T("\t\t\t\t\t<tile tile=\"&chrys;\" />") << std::endl;
			if (gameStat->Player[player].FlowerFlag.Bamboo)
				XhaifuBufferBody << _T("\t\t\t\t\t<tile tile=\"&bamboo;\" />") << std::endl;
	}
	if (gameStat->Player[player].FlowerFlag.Spring || gameStat->Player[player].FlowerFlag.Summer ||
		gameStat->Player[player].FlowerFlag.Autumn || gameStat->Player[player].FlowerFlag.Winter ||
		gameStat->Player[player].FlowerFlag.Plum || gameStat->Player[player].FlowerFlag.Orchid ||
		gameStat->Player[player].FlowerFlag.Chrys || gameStat->Player[player].FlowerFlag.Bamboo)
		XhaifuBufferBody << _T("\t\t\t\t</flower>") << std::endl;
}

void haifu::tools::hfwriter::finalformWriter::MeldWriter::hfChii(PlayerID player, MeldCode meld) {
	// チー
	XhaifuBufferBody << _T("\t\t\t\t<sequence>") << std::endl;
	Tile meldTile[3];
	switch (meld.mstat) {
	case meldSequenceExposedLower:
		meldTile[0].tile = meld.tile; meldTile[0].red = meld.red[0];
		meldTile[1].tile = TileCode(meld.tile + 1); meldTile[1].red = meld.red[1];
		meldTile[2].tile = TileCode(meld.tile + 2); meldTile[2].red = meld.red[2];
		break;
	case meldSequenceExposedMiddle:
		meldTile[1].tile = meld.tile; meldTile[1].red = meld.red[0];
		meldTile[0].tile = TileCode(meld.tile + 1); meldTile[0].red = meld.red[1];
		meldTile[2].tile = TileCode(meld.tile + 2); meldTile[2].red = meld.red[2];
		break;
	case meldSequenceExposedUpper:
		meldTile[1].tile = meld.tile; meldTile[1].red = meld.red[0];
		meldTile[2].tile = TileCode(meld.tile + 1); meldTile[2].red = meld.red[1];
		meldTile[0].tile = TileCode(meld.tile + 2); meldTile[0].red = meld.red[2];
		break;
	default:
		// This must not occur...
		throw std::invalid_argument("Not a sequence meld!");
	}
	for (int i = 0; i < 3; i++) {
		XhaifuBufferBody << _T("\t\t\t\t\t");
		recordTile_Inline(meldTile[i], i == 0);
		XhaifuBufferBody << std::endl;
	}
	XhaifuBufferBody << _T("\t\t\t\t</sequence>") << std::endl;
}
inline void haifu::tools::hfwriter::finalformWriter::MeldWriter::hfPon1(PlayerID player, MeldCode meld) {
	const Tile meldTile(meld.tile, meld.red[0]);
	XhaifuBufferBody << _T("\t\t\t\t\t");
	if ((meld.mstat == meldQuadAddedLeft) ||
		(meld.mstat == meldQuadAddedCenter) ||
		(meld.mstat == meldQuadAddedRight))
			recordTile_Inline(meldTile, meld.red[3]);
	else recordTile_Inline(meldTile, true);
	XhaifuBufferBody << std::endl;
}
void haifu::tools::hfwriter::finalformWriter::MeldWriter::hfPon(PlayerID player, MeldCode meld) {
	int tiles = 0, interrupt = 0; CodeConv::tstring meldDirection;
	switch (meld.mstat) {
	case meldTripletExposedLeft: case meldQuadExposedLeft: case meldQuadAddedLeft:
		meldDirection = _T(" meld-direction=\"left\"");
		interrupt = 1; break;
	case meldTripletExposedCenter: case meldQuadExposedCenter: case meldQuadAddedCenter:
		meldDirection = _T(" meld-direction=\"opposite\"");
		interrupt = 2; break;
	case meldTripletExposedRight: case meldQuadExposedRight: case meldQuadAddedRight:
		meldDirection = _T(" meld-direction=\"right\"");
		interrupt = 8; break;
	case meldQuadConcealed:
		interrupt = 7; break;
	}
	switch (meld.mstat) {
	case meldTripletExposedLeft: case meldTripletExposedCenter:
	case meldTripletExposedRight:
		tiles = 3;
		XhaifuBufferBody << _T("\t\t\t\t<triplet") << meldDirection << _T('>') << std::endl;
		break;
	case meldQuadExposedLeft: case meldQuadExposedCenter:
	case meldQuadExposedRight:
		XhaifuBufferBody << _T("\t\t\t\t<quad-exposed") << meldDirection << _T(" added=\"false\">") << std::endl;
		goto quad_4tiles;
	case meldQuadConcealed:
		XhaifuBufferBody << _T("\t\t\t\t<quad-concealed>") << std::endl;
		goto quad_4tiles;
	quad_4tiles:
		tiles = 4;
		break;
	case meldQuadAddedLeft: case meldQuadAddedCenter:
	case meldQuadAddedRight:
		tiles = 3;
		XhaifuBufferBody << _T("\t\t\t\t<quad-exposed") << meldDirection << _T(" added=\"true\">") << std::endl;
		break;
	}
	for (int i = (meld.mstat == meldQuadConcealed ? 0 : 1); i < tiles; i++) {
		if (i == interrupt) hfPon1(player, meld);
		const Tile meldTile(meld.tile, meld.red[i]);
		XhaifuBufferBody << _T("\t\t\t\t\t");
		recordTile_Inline(meldTile, false);
		XhaifuBufferBody << std::endl;
	}
	if (interrupt == 8) hfPon1(player, meld);
	switch (meld.mstat) {
	case meldTripletExposedLeft: case meldTripletExposedCenter:
	case meldTripletExposedRight:
		XhaifuBufferBody << _T("\t\t\t\t</triplet>") << std::endl;
		break;
	case meldQuadExposedLeft: case meldQuadExposedCenter: case meldQuadExposedRight:
	case meldQuadAddedLeft: case meldQuadAddedCenter: case meldQuadAddedRight:
		XhaifuBufferBody << _T("\t\t\t\t</quad-exposed>") << std::endl;
		break;
	case meldQuadConcealed:
		XhaifuBufferBody << _T("\t\t\t\t</quad-concealed>") << std::endl;
		break;
	}
}

void haifu::tools::hfwriter::finalformWriter::hfExposedMeld(const GameTable* const gameStat, PlayerID player) {
	for (int i = 1; i <= gameStat->Player[player].MeldPointer; i++) {
		switch (gameStat->Player[player].Meld[i].mstat) {
		case meldSequenceExposedLower: case meldSequenceExposedMiddle:
		case meldSequenceExposedUpper:
			MeldWriter::hfChii(player, gameStat->Player[player].Meld[i]);
			break;
		case meldTripletExposedLeft: case meldQuadExposedLeft: case meldQuadAddedLeft:
		case meldTripletExposedCenter: case meldQuadExposedCenter: case meldQuadAddedCenter:
		case meldTripletExposedRight: case meldQuadExposedRight: case meldQuadAddedRight:
		case meldQuadConcealed:
			MeldWriter::hfPon(player, gameStat->Player[player].Meld[i]);
			break;
		}
	}
}

void haifu::tools::hfwriter::hfScoreWriteOut(const GameTable* const gameStat, PlayerID player, seatAbsolute wind, EndType RoundEndType) {
	// 点数の変動
	CodeConv::tostringstream o;
	o << _T(" ") << origPoint[player].to_str(_T(""), _T("△"));
	if (origPoint[player] != gameStat->Player[player].PlayerScore) // 点数が一致しないなら
		o << _T(" → ") <<
			gameStat->Player[player].PlayerScore.to_str(_T(""), _T("△")) << _T(" (") <<
			(gameStat->Player[player].PlayerScore -
			origPoint[player]).to_str(_T("+"), _T("-")) <<
			_T(")");
#ifndef GUOBIAO
	if (RuleData::chkRuleApplied("chip")) // チップありの時
		o << _T(" チップ: ") <<
			((gameStat->Player[player].playerChip >= 0) ? _T("+") : _T("")) <<
			gameStat->Player[player].playerChip;

#endif /* GUOBIAO */
					
	// XML用出力
	const CodeConv::tstring nomDeVent[4] = {_T("east"), _T("south"), _T("west"), _T("north")};
#ifdef GUOBIAO
	XhaifuBuffer << _T("\t\t\t\t<player ref=\"player") << playerNumberList[currWindNum][static_cast<int>(player)] << _T("\" wind=\"") <<
#else /* GUOBIAO */
	XhaifuBuffer << _T("\t\t\t\t<player ref=\"player") << static_cast<int>(player) << _T("\" wind=\"") <<
#endif /* GUOBIAO */
		nomDeVent[wind] << _T("\" score=\"")
		<< origPoint[player].to_str_plain() << _T('"');
	if (origPoint[player] != gameStat->Player[player].PlayerScore) // 点数が一致しないなら
		XhaifuBuffer << _T(" delta=\"") <<
			(gameStat->Player[player].PlayerScore -
			origPoint[player]).to_str_plain() << _T('"');
#ifndef GUOBIAO
	if (RuleData::chkRuleApplied("chip")) // チップありの時
		XhaifuBuffer << _T(" chip=\"") <<
			static_cast<int>(gameStat->Player[player].playerChip) << _T('"');
	{
		CodeConv::tstring tmpStr;
		if (gameStat->Player[player].AgariHouki) { // アガリ放棄
			tmpStr += _T("アガリ放棄");
		}
		if (RuleData::chkRuleApplied("wareme") && (gameStat->WaremePlayer == player)) { // 割れ目ルールのとき
			if (!tmpStr.empty()) tmpStr += _T("、");
			if ((RuleData::chkRule("wareme", "greater_wareme")) &&
				((gameStat->Dice[0].Number == gameStat->Dice[1].Number) ||
				((gameStat->Dice[2].Number != 0) && (gameStat->Dice[2].Number == gameStat->Dice[3].Number))))
				tmpStr += _T("大"); // サイコロがゾロ目の時はさらに倍
			tmpStr += _T("割れ目");
		}
		if (RuleData::chkRuleApplied("doukasen") && (gameStat->DoukasenPlayer == player)) { // 導火線ルールの時
			if (!tmpStr.empty()) tmpStr += _T("、");
			tmpStr += _T("導火線");
		}
		if (!tmpStr.empty())
			XhaifuBuffer << _T(" comment=\"") << tmpStr << _T("\"");
	}
#endif /* GUOBIAO */
	if (RoundEndType == Agari) { // 役リスト
		const yaku::YAKUSTAT& yakuData(yaku::yakuCalculator::countyaku(gameStat, player));
		mihajong_structs::YakuListType yakuList;
		yakuData.yakuList(&yakuList, true);
		if (yakuData.isValid) {
			XhaifuBuffer << _T(">") << std::endl;
			for (const auto& yaku : yakuList) {
				XhaifuBuffer << _T("\t\t\t\t\t<yaku name=\"") << yaku.first << _T("\"");
				if (!yaku.second.empty())
					XhaifuBuffer << _T(" value=\"") << yaku.second << _T("\"");
				XhaifuBuffer << _T(" />") << std::endl;
			}
			XhaifuBuffer << _T("\t\t\t\t</player>") << std::endl;
		} else {
			XhaifuBuffer << _T(" />") << std::endl;
		}
	} else {
		XhaifuBuffer << _T(" />") << std::endl;
	}
}

void haifu::tools::hfwriter::hfWriteFinalForms(const GameTable* const gameStat, int OrigTurn, EndType RoundEndType) {
	XhaifuBufferBody << _T("\t\t\t</cycle>") << std::endl;
	XhaifuBufferBody << _T("\t\t</progress>") << std::endl;
	XhaifuBufferBody << _T("\t\t<final-hands>") << std::endl;
	XhaifuBuffer << _T("\t\t\t<player-score>") << std::endl;
	for (int i = 0; i < ACTUAL_PLAYERS; i++) {
		PlayerID k = RelativePositionOf(i, static_cast<seatRelative>(OrigTurn % Players));
#ifdef GUOBIAO
		XhaifuBufferBody << _T("\t\t\t<final-hand player=\"player") << playerNumberList[currWindNum][k] << _T("\">") << std::endl;
#else /* GUOBIAO */
		XhaifuBufferBody << _T("\t\t\t<final-hand player=\"player") << static_cast<int>(k) << _T("\">") << std::endl;
#endif /* GUOBIAO */
		if (gameStat->chkGameType(SanmaT))
			if (((OrigTurn % static_cast<int>(Players)) + i) >= ACTUAL_PLAYERS)
				k = (k + 1) % Players;
		// 副露面子を出力する
		finalformWriter::hfFinalForm(gameStat, k, RoundEndType);
		finalformWriter::hfFlower(gameStat, k);
		finalformWriter::hfExposedMeld(gameStat, k);
		// 点棒状況を書き出す
		hfScoreWriteOut(gameStat, k, static_cast<seatAbsolute>(i), RoundEndType);
		// 色々書き出し
		XhaifuBufferBody << _T("\t\t\t</final-hand>") << std::endl;
	}
	XhaifuBufferBody << _T("\t\t</final-hands>") << std::endl;
	XhaifuBuffer << _T("\t\t\t</player-score>") << std::endl;
}

void haifu::tools::hfwriter::hfWriteBottom() {
	XhaifuBuffer << _T("\t\t</round-description>") << std::endl <<
		XhaifuBufferBody.str() << _T("\t</round>") << std::endl;
}

/* 配牌をバッファに出力 */
void haifu::haifuwritebuffer(
	const GameTable* const gameStat,
	int OrigTurn, int OrigHonba, bool tmpUraFlag, bool tmpAliceFlag,
	LPCTSTR ResultDesc, EndType RoundEndType
	) { /* 配牌をバッファに出力 */
		tools::hfwriter::hfWriteHead(
			gameStat, OrigTurn, OrigHonba, tmpUraFlag, tmpAliceFlag,
			ResultDesc, RoundEndType);
		tools::hfwriter::hfWriteFinalForms(
			gameStat, OrigTurn, RoundEndType);
		tools::hfwriter::hfWriteBottom();
}

/* 牌譜を保存 */
void haifu::haifusave(const GameTable* const gameStat) {
	XhaifuBuffer << _T("</haifu>") << std::endl; // Finalize XML

	tools::haifuRecTime(_T("time-finished"));
	XMLhaifuBuffer << XhaifuBuffer.str();

	/* ファイル名の自動決定 */
	std::string configPath = confpath::confPath();
	std::ostringstream filename1, filename2;
	filename1 << configPath << "haifu/";
	switch (gameStat->gameType) {
		case Yonma: filename1 << "mihajong"; break;
		case Sanma: filename1 << "mihasanm"; break;
		case Sanma4: filename1 << "mihaysnm"; break;
		case SanmaS: filename1 << "mihassnm"; break;
		case SanmaSeto: filename1 << "mihastsm"; break;
		case GuobiaoMJ: filename1 << "mihagbmj"; break;
	}
	filename1 << "_" << MIHAJONG_MAJOR_VER << "_" <<
		MIHAJONG_MINOR_VER << "_" << MIHAJONG_PATCH_VER;

#ifdef _WIN32
	SYSTEMTIME ltime; GetLocalTime(&ltime);
	filename2 << std::setw(4) << std::setfill('0') << ltime.wYear;
	filename2 << std::setw(2) << std::setfill('0') << ltime.wMonth;
	filename2 << std::setw(2) << std::setfill('0') << ltime.wDay << "_";
	filename2 << std::setw(2) << std::setfill('0') << ltime.wHour;
	filename2 << std::setw(2) << std::setfill('0') << ltime.wMinute;
#else /*_WIN32*/
	time_t tempus = time(nullptr);
	tm ltime;
	localtime_s(&ltime, &tempus);
	filename2 << std::setw(4) << std::setfill('0') << (ltime.tm_year + 1900);
	filename2 << std::setw(2) << std::setfill('0') << (ltime.tm_mon + 1);
	filename2 << std::setw(2) << std::setfill('0') << ltime.tm_mday << "_";
	filename2 << std::setw(2) << std::setfill('0') << ltime.tm_hour;
	filename2 << std::setw(2) << std::setfill('0') << ltime.tm_min;
#endif /*_WIN32*/

	/* ファイル書き出し */
	std::ofstream fileout;
	fileout.open((filename1.str() + std::string("_haifu_") + // XML形式牌譜
		filename2.str() + std::string(".xml")).c_str());
	fileout << CodeConv::EncodeStr(XMLhaifuBuffer.str()); fileout.close();
}
