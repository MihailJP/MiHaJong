#include "agariscr.h"
#include "../../loadtex.h"
#include "../../resource.h"
#include "../../sprite.h"
#include "../../geometry.h"
#include "../../yakustat.h"
#include <cmath>
#include "../../../sound/sound.h"
#include "../../../common/bgmid.h"
#include "../../gametbl.h"
#include "../../rule.h"
#include <cassert>
#include <iomanip>

namespace mihajong_graphic {

// -------------------------------------------------------------------------

const double TableSubsceneAgariScreenProto::yakuInterval = 0.75;
const double TableSubsceneAgariScreenProto::yakuAnimStartSecond = 2.0;

TableSubsceneAgariScreenProto::TableSubsceneAgariScreenProto(LPDIRECT3DDEVICE9 device) : TableSubscene(device) {
	myDevice = device;
	LoadTexture(device, &windowTexture, MAKEINTRESOURCE(IDB_PNG_AGARI_WINDOW), yakuWndWidth, yakuWndHeight);
	yakuData = YakuResult::getYakuStat();
	parseYakuList();
	myTextRenderer = new TextRenderer(device);
	agariTehai = new AgariTehai(this);
	agariNaki = new AgariNaki(this);
	doraTilesOmote = new DoraTilesOmote(this);
	showScore = new ShowScore(this);
}
TableSubsceneAgariScreenProto::~TableSubsceneAgariScreenProto() {
	delete showScore;
	delete doraTilesOmote;
	delete agariNaki;
	delete agariTehai;
	delete myTextRenderer;
	windowTexture->Release();
}

bool TableSubsceneAgariScreenProto::YakumanMode() {
	static bool cached = false, agariScreenMode = false;
	if (!cached) {
		const mihajong_structs::YakuResult yakuInfo = YakuResult::getYakuStat();
		const int tmpTotalHan = yakuInfo.CoreHan + yakuInfo.BonusHan;
		const int tmpTotalMangan = yakuInfo.CoreSemiMangan + yakuInfo.BonusSemiMangan;
		if (tmpTotalMangan >= 8)
			agariScreenMode = true;
		else if ((tmpTotalMangan >= 6) &&
			((tmpTotalHan < 12) ||
			((tmpTotalHan < 13) && (rules::chkRule("kazoe_border", "13han_or_more"))) ||
			((tmpTotalMangan < 8) && (rules::chkRule("kazoe_border", "no")))))
			agariScreenMode = true;
		else if ((tmpTotalMangan >= 4) &&
			((tmpTotalHan < 10) ||
			((tmpTotalHan < 11) && (rules::chkRule("sanbaiman_border", "11han_or_more")))))
			agariScreenMode = true;
		else if ((tmpTotalMangan >= 3) && (tmpTotalHan < 8))
			agariScreenMode = true;
		else if ((tmpTotalMangan >= 2) && (tmpTotalHan < 6))
			agariScreenMode = true;
		cached = true;
	}
	return agariScreenMode;
}

void TableSubsceneAgariScreenProto::parseYakuList() {
	typedef std::vector<CodeConv::tstring> TStrList;
	LPTSTR yakuNameUnified = nullptr, yakuValUnified = nullptr;
	LPCTSTR yakuName = YakumanMode() ? yakuData.yakumanNameList : yakuData.yakuNameList;
	LPCTSTR yakuVal  = YakumanMode() ? yakuData.yakumanValList  : yakuData.yakuValList;
	if (!rules::chkRule("limitless", "no")) {
		const size_t bufsz = yakuData.nameBufSize * 2;
		yakuNameUnified = new TCHAR[bufsz]; yakuNameUnified[0] = _T('\0');
		yakuValUnified = new TCHAR[bufsz]; yakuValUnified[0] = _T('\0');
		_tcscpy_s(yakuNameUnified, bufsz, yakuData.yakumanNameList);
		for (TCHAR* k = yakuNameUnified; *k != _T('\0'); ++k)
			if (*k == _T('\n'))
#ifdef _WIN32
				_tcscat_s(yakuValUnified, bufsz, _T("\r\n"));
#else
				_tcscat_s(yakuValUnified, bufsz, _T("\n"));
#endif
		_tcscat_s(yakuNameUnified, bufsz, yakuData.yakuNameList);
		_tcscat_s(yakuValUnified, bufsz, yakuData.yakuValList);
		yakuName = yakuNameUnified; yakuVal = yakuValUnified;
	}
	CodeConv::tstring yakuNameTxt, yakuValTxt;
	auto splitstr = [](LPCTSTR str) -> TStrList { // â¸çsÇ≈ï™äÑ
		TStrList txtlst;
		LPCTSTR ssptr = str; LPCTSTR sptr = ssptr;
		for (; *sptr != _T('\0'); ++sptr) {
			if (*sptr == _T('\n')) { // â¸çsÇ™åªÇÍÇΩÇÁ
				txtlst.push_back(CodeConv::tstring(ssptr, sptr - (*(sptr - 1) == _T('\r') ? 1 : 0)));
				ssptr = sptr + 1;
			}
		}
		CodeConv::tstring laststr(ssptr, sptr - (*(sptr - 1) == _T('\r') ? 1 : 0));
		if (!laststr.empty()) txtlst.push_back(laststr);
		return txtlst;
	};
	TStrList yakuNameList(splitstr(yakuName)), yakuValList(splitstr(yakuVal));
	for (int i = 0; i < min(yakuNameList.size(), yakuValList.size()); ++i)
		if (YakumanMode())
			yakuList.push_back(std::make_pair(yakuNameList[i], _T("")));
		else
			yakuList.push_back(std::make_pair(yakuNameList[i], yakuValList[i]));
	if (!rules::chkRule("limitless", "no")) {
		delete[] yakuNameUnified;
		delete[] yakuValUnified;
	}
}

void TableSubsceneAgariScreenProto::renderWindow() {
	const double Zeit = seconds();
	const int yOffset = (Zeit >= 1.0) ? 0 : (int)(pow(1.0 - Zeit, 2) * (double)Geometry::BaseSize);
	SpriteRenderer::instantiate(myDevice)->ShowSprite(
		windowTexture, BaseX, BaseY - yOffset, yakuWndWidth, yakuWndHeight);
}

bool TableSubsceneAgariScreenProto::renderYakuName(unsigned yakuNum) {
	const double anmTime = 0.5;
	const double Zeit = seconds() - (yakuAnimStartSecond + yakuInterval * yakuNum);
	const int xOffset = (Zeit >= anmTime) ? 0 : (int)(pow(2.0 * (anmTime - Zeit), 2) * (double)(yakuWndWidth / 5));
	if (Zeit <= 0) {
		myTextRenderer->DelText(yakuNum * 2);
		myTextRenderer->DelText(yakuNum * 2 + 1);
		return false;
	} else {
		// ïùåvéZ
		const float compressRate = [](const CodeConv::tstring& str) -> float {
			const std::wstring tmpstr(
#ifdef _UNICODE
				str
#else
				CodeConv::ANSItoWIDE(str)
#endif
				);
			int cols = 0;
			for (auto k = tmpstr.begin(); k != tmpstr.end(); ++k)
				cols += ((*k > L'\x7f') ? 2 : 1);
			return (cols > 12) ? (8.0f / (float)cols) : 1.0f;
		} (yakuList[yakuNum].first);
		// ï\é¶
		const D3DCOLOR color = (Zeit >= anmTime) ? 0xffffffff : ((255 - (int)((anmTime - Zeit) * 300)) << 24 | 0x00ffffff);
		const int x = BaseX + ((yakuNum % 2 == 0) ? 50 : 390);
		const int y = BaseY + 200;
		myTextRenderer->NewText(yakuNum * 2, yakuList[yakuNum].first,
			x + xOffset, y + (yakuNum / 2) * 50,
			1.0f, compressRate, color);
		myTextRenderer->NewText(yakuNum * 2 + 1, yakuList[yakuNum].second,
			x + 220, y + 8 + (yakuNum / 2) * 50,
			0.75f, 1.0f, color);
		return true;
	}
}

void TableSubsceneAgariScreenProto::renderYakuName() {
	static int maxShown = -1;
	for (unsigned i = 0; i < yakuList.size(); ++i) {
		const bool isShown = renderYakuName(i);
		if (isShown && (maxShown < (signed)i)) {
			maxShown = (signed)i;
			sound::Play(sound::IDs::sndPage);
		}
	}
}

// -------------------------------------------------------------------------

TableSubsceneAgariScreen::TableSubsceneAgariScreen(LPDIRECT3DDEVICE9 device) : TableSubsceneAgariScreenProto(device) {
}
TableSubsceneAgariScreen::~TableSubsceneAgariScreen() {
}
void TableSubsceneAgariScreen::Render() {
	renderWindow();
	renderYakuName();
	myTextRenderer->Render();
	agariTehai->Render();
	agariNaki->Render();
	doraTilesOmote->Render();
	showScore->Render();
}

// -------------------------------------------------------------------------

TableSubsceneAgariScreenUradora::TableSubsceneAgariScreenUradora(LPDIRECT3DDEVICE9 device) : TableSubsceneAgariScreenProto(device) {
	doraTilesUra = new DoraTilesUra(this);
}
TableSubsceneAgariScreenUradora::~TableSubsceneAgariScreenUradora() {
	delete doraTilesUra;
}
void TableSubsceneAgariScreenUradora::Render() {
	renderWindow();
	renderYakuName();
	myTextRenderer->Render();
	agariTehai->Render();
	agariNaki->Render();
	doraTilesOmote->Render();
	doraTilesUra->Render();
	showScore->Render();
}

// -------------------------------------------------------------------------

TableSubsceneAgariScreenProto::AgariTehai::AgariTehai(TableSubsceneAgariScreenProto* caller) : ShowTehai(caller->myDevice) {
	myCaller = caller;
	reconstFlag = true;
}
TableSubsceneAgariScreenProto::AgariTehai::~AgariTehai() {
}

void TableSubsceneAgariScreenProto::AgariTehai::Reconstruct(const GameTable* gameStat) {
	ShowTehai::Reconstruct(gameStat, gameStat->CurrentPlayer.Agari,
		[this](seatRelative) -> std::tuple<int, int> {
			const double Zeit = myCaller->seconds();
			const int yOffset = (Zeit >= 1.0) ? 0 : (int)(pow(1.0 - Zeit, 2) * (double)Geometry::BaseSize);
			return std::make_tuple(BaseX + 28, handPosY - yOffset);
		},
		sSelf, [](int){return (D3DCOLOR)0xffffffff;},
		[](const int*, const int*, int){});
}

void TableSubsceneAgariScreenProto::AgariTehai::Render() {
	const double Zeit = myCaller->seconds();
	if (reconstFlag) Reconstruct(GameStatus::gameStat());
	if (Zeit >= 1.0) reconstFlag = false;
	TileTexture->Render();
}

// -------------------------------------------------------------------------

/* ñ¬Ç¢ÇΩîvÇï\é¶Ç∑ÇÈ */
std::tuple<std::function<unsigned (unsigned)>, std::function<int (unsigned)>, std::function<int (unsigned)>, TileDirection, TileDirection, TileDirection>
	TableSubsceneAgariScreenProto::AgariNaki::playerPosition(const GameTable* gameStat, PlayerID targetPlayer, signed PositionOffset, unsigned IDOffset, unsigned meldID,
	unsigned h1, unsigned h2, unsigned h3, unsigned h4, unsigned v1, unsigned v2, unsigned v3, unsigned v4,
	bool r1, bool r2, bool r3, bool r4)
{
	return std::make_tuple(
		[=](unsigned i) -> unsigned {return i + IDOffset;},
		[=](unsigned i) -> int {
			switch (i) {
				case 0: return h1 - PositionOffset; break;
				case 1: return h2 - PositionOffset; break;
				case 2: return h3 - PositionOffset; break;
				case 3: return h4 - PositionOffset; break;
				default: throw "Out of range";
			}
		},
		[=](unsigned i) -> int {
			switch (i) {
				case 0: return v1; break;
				case 1: return v2; break;
				case 2: return v3; break;
				case 3: return v4; break;
				default: throw "Out of range";
			}
		},
		Withershins, Portrait, Clockwise);
}
void TableSubsceneAgariScreenProto::AgariNaki::Reconstruct(const GameTable* gameStat) {
	const PlayerID targetPlayer = gameStat->CurrentPlayer.Agari;
	unsigned posOffset[5] = {0,};
	for (int i = 1; i <= gameStat->Player[targetPlayer].MeldPointer; ++i) {
		switch (gameStat->Player[targetPlayer].Meld[i].mstat) {
		case meldQuadExposedLeft: case meldQuadExposedCenter: case meldQuadExposedRight:
			posOffset[i] = posOffset[i - 1] + ShowTile::VertTileWidth * 3 + ShowTile::HoriTileWidth;
			break;
		case meldQuadConcealed:
			posOffset[i] = posOffset[i - 1] + ShowTile::VertTileWidth * 4;
			break;
		default:
			posOffset[i] = posOffset[i - 1] + ShowTile::VertTileWidth * 2 + ShowTile::HoriTileWidth;
			break;
		}
	}
	for (int i = 1; i <= gameStat->Player[targetPlayer].MeldPointer; ++i)
		NakihaiSelRoutine(gameStat, targetPlayer, posOffset[i - 1],
			(gameStat->Player[targetPlayer].MeldPointer - i) * 4, i);
}

void TableSubsceneAgariScreenProto::AgariNaki::Render() {
	const double Zeit = myCaller->seconds();
	if (reconstFlag) Reconstruct(GameStatus::gameStat());
	if (Zeit >= 1.0) reconstFlag = false;
	TileTexture->Render();
}

TableSubsceneAgariScreenProto::AgariNaki::AgariNaki(TableSubsceneAgariScreenProto* caller) : ShowNakihai(caller->myDevice) {
	myCaller = caller;
}

TableSubsceneAgariScreenProto::AgariNaki::~AgariNaki() {
}

// -------------------------------------------------------------------------

TableSubsceneAgariScreenProto::DoraTiles::DoraTiles(TableSubsceneAgariScreenProto* caller) {
	myCaller = caller;
	tileObj = new ShowTile(caller->myDevice);
}
TableSubsceneAgariScreenProto::DoraTiles::~DoraTiles() {
	delete tileObj;
}
void TableSubsceneAgariScreenProto::DoraTiles::Reconstruct() {
	const GameTable* const gameStat = GameStatus::gameStat();
	const int numberOfTiles = [gameStat]() -> int {
		if (chkGameType(gameStat, AllSanma))
			return 108;
		else if (rules::chkRule("flower_tiles", "no"))
			return 136;
		else if (rules::chkRule("flower_tiles", "8tiles"))
			return 144;
		else
			return 140;
	}();
	const int DoraPosStart = numberOfTiles - tileIdOffset() - 4 - gameStat->ExtraRinshan;
	const int DoraPosEnd = numberOfTiles - tileIdOffset() - gameStat->DeadTiles;
	for (int i = DoraPosStart; i > DoraPosEnd; i -= 2) {
		const int tileIndex = (i - DoraPosStart) / (-2);
		const double Zeit = myCaller->seconds() - (startTime() + (double)tileIndex * 0.0625);
		if (Zeit >= 0.0) {
			const D3DCOLOR color = (Zeit >= 0.325) ? 0xffffffff : ((255 - (int)((0.325 - Zeit) * 700)) << 24 | 0x00ffffff);
			tileObj->NewTile(tileIndex,
				(gameStat->DoraPointer <= i) ? gameStat->Deck[i].tile : BackSide,
				(gameStat->DoraPointer <= i) ? gameStat->Deck[i].red : Normal,
				xPos() + ShowTile::VertTileWidth * tileIndex, yPos(), Portrait, Obverse, color);
		}
	}
}
void TableSubsceneAgariScreenProto::DoraTiles::Render() {
	Reconstruct();
	tileObj->Render();
}

TableSubsceneAgariScreenProto::DoraTilesOmote::DoraTilesOmote(TableSubsceneAgariScreenProto* caller) : DoraTiles(caller) {
	Reconstruct();
}
TableSubsceneAgariScreenProto::DoraTilesOmote::~DoraTilesOmote() {
}

TableSubsceneAgariScreenUradora::DoraTilesUra::DoraTilesUra(TableSubsceneAgariScreenProto* caller) : DoraTiles(caller) {
	Reconstruct();
}
TableSubsceneAgariScreenUradora::DoraTilesUra::~DoraTilesUra() {
}

// -------------------------------------------------------------------------

TableSubsceneAgariScreenProto::ShowScore::ShowScore(TableSubsceneAgariScreenProto* caller) {
	myCaller = caller;
	digitRenderer = new CallDigitRenderer(caller->myDevice);
	txtRenderer = new TextRenderer(caller->myDevice);
}
TableSubsceneAgariScreenProto::ShowScore::~ShowScore() {
	delete txtRenderer;
	delete digitRenderer;
}
void TableSubsceneAgariScreenProto::ShowScore::ReconstructScoreFuHan() {
	const double Zeit = myCaller->seconds() - (yakuAnimStartSecond + yakuInterval * myCaller->yakuList.size());
	if (Zeit <= 0.0) return;
	if (YakumanMode()) return;
	const double anmTime = 0.75;
	const int han = (YakuResult::getYakuStat().CoreHan + YakuResult::getYakuStat().BonusHan);
	CodeConv::tostringstream o;
	o << std::setw(3) << std::setfill(_T(' ')) << YakuResult::getYakuStat().BasePoints << _T("ïÑ") <<
		std::setw(2) << std::setfill(_T(' ')) << han << _T("„ ");
	const int x = BaseX + yakuWndWidth - 32 - 27 * 9;
	const int y = BaseY + 650;
	const D3DCOLOR color = (Zeit >= anmTime) ? 0xffffffff : ((255 - (int)((anmTime - Zeit) * 300)) << 24 | 0x00ffffff);
	txtRenderer->NewText(0, o.str(), x, y, 1.0f, (han >= 100) ? (1.5f * 0.9f) : 1.5f, color);
}
void TableSubsceneAgariScreenProto::ShowScore::ReconstructScoreTxt() {
	const double Zeit = myCaller->seconds() - (yakuAnimStartSecond + yakuInterval * myCaller->yakuList.size());
	if (Zeit <= 0.0) return;
	const double anmTime = 0.75;
	const CodeConv::tstring scoreTxt = YakuResult::getAgariScore().bignumtotext(_T(""), _T("-"));
	assert(scoreTxt != _T("0"));
	std::wstring scoreTxtW =
#ifdef _UNICODE
		scoreTxt;
#else
		CodeConv::ANSItoWIDE(scoreTxt);
#endif
	const int x = BaseX + yakuWndWidth - 24 - 72 * ((scoreTxtW.size() < 6) ? scoreTxtW.size() : 6);
	const int y = BaseY + 700;
	const float scale = (Zeit >= anmTime) ? 1.0f : (pow(2.5f * (float)(anmTime - Zeit), 2) + 1.0f);
	const D3DCOLOR color = (Zeit >= anmTime) ? 0xffffffff : ((255 - (int)((anmTime - Zeit) * 300)) << 24 | 0x00ffffff);
	digitRenderer->NewText(0, scoreTxt,
		x - (int)(36.0f * ((scoreTxtW.size() < 6) ? (float)scoreTxtW.size() : 6.0f) * (scale - 1.0f)),
		y - (int)(48.0f * (scale - 1.0f)),
		scale,
		(scoreTxtW.size() < 6) ? 1.5f : (float)(1.5 * 6.0 / (double)scoreTxtW.size()),
		color);
}
void TableSubsceneAgariScreenProto::ShowScore::ReconstructScoreRank() {
	if (rules::chkRule("limitless", "no")) {
		const double Zeit = myCaller->seconds() - (yakuAnimStartSecond + yakuInterval * myCaller->yakuList.size());
		if (Zeit <= 0.0) return;
		const double anmTime = 0.75;

		const unsigned score = (unsigned)(YakuResult::getYakuStat().AgariPoints.bignumtodbl());
		CodeConv::tstring tmptxt; unsigned strWidth = 0;
		if      (score ==  2000) {tmptxt = _T("ñûä—");   strWidth = 4;}
		else if (score ==  3000) {tmptxt = _T("íµñû");   strWidth = 4;}
		else if (score ==  4000) {tmptxt = _T("î{ñû");   strWidth = 4;}
		else if (score ==  6000) {tmptxt = _T("éOî{ñû"); strWidth = 6;}
		else if (score ==  8000) {
			if (YakuResult::getYakuStat().CoreSemiMangan + YakuResult::getYakuStat().BonusSemiMangan >= 8) {
				tmptxt = _T("ññû");     strWidth = 4;
			} else {
				tmptxt = _T("êîÇ¶ññû"); strWidth = 8;
			}
		}
		else if (score == 16000) {tmptxt = _T("ìÒî{ññû"); strWidth = 8;}
		else if (score == 24000) {tmptxt = _T("éOî{ññû"); strWidth = 8;}
		else if (score == 32000) {tmptxt = _T("élî{ññû"); strWidth = 8;}
		else if (score == 40000) {tmptxt = _T("å‹î{ññû"); strWidth = 8;}
		else if (score == 48000) {tmptxt = _T("òZî{ññû"); strWidth = 8;}
		else if (score == 56000) {tmptxt = _T("éµî{ññû"); strWidth = 8;}
		else if (score == 64000) {tmptxt = _T("î™î{ññû"); strWidth = 8;}
		else if (score == 72000) {tmptxt = _T("ã„î{ññû"); strWidth = 8;}
		else if (score == 80000) {tmptxt = _T("è\î{ññû"); strWidth = 8;}
		else if (score >= 88000) {
			CodeConv::tostringstream o;
			o << score / 8000; strWidth = o.str().length();
			o << "î{ññû"; strWidth += 6;
			tmptxt = o.str();
		}

		if (strWidth) {
			const int x = BaseX + 30;
			const int y = BaseY + 720;
			const float scale = (Zeit >= anmTime) ? 1.0f : (pow(2.5f * (float)(anmTime - Zeit), 2) + 1.0f);
			const D3DCOLOR color = (Zeit >= anmTime) ? 0xffffffff : ((255 - (int)((anmTime - Zeit) * 300)) << 24 | 0x00ffffff);
			txtRenderer->NewText(1, tmptxt,
				x - (int)(54.0f * (scale - 1.0f)),
				y - (int)(36.0f * (scale - 1.0f)),
				2.0f * scale, 6.0f / (float)strWidth, color);
		}
	}
}
void TableSubsceneAgariScreenProto::ShowScore::Reconstruct() {
	ReconstructScoreFuHan();
	ReconstructScoreTxt();
	ReconstructScoreRank();
}
void TableSubsceneAgariScreenProto::ShowScore::Render() {
	Reconstruct();
	txtRenderer->Render();
	digitRenderer->Render();
}

// -------------------------------------------------------------------------

}
