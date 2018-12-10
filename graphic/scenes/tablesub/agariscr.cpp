#ifdef _MSC_VER
#define NOMINMAX
#endif
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
#include "../../utils.h"
#include "../../event.h"
#include <algorithm>

namespace mihajong_graphic {

// -------------------------------------------------------------------------

const double TableSubsceneAgariScreenProto::yakuInterval = 0.75;
const double TableSubsceneAgariScreenProto::yakuAnimStartSecond = 2.0;

TableSubsceneAgariScreenProto::TableSubsceneAgariScreenProto(DevicePtr device) : TableSubscene(device) {
	myDevice = device;
	LoadTexture(device, &windowTexture, MAKEINTRESOURCE(IDB_PNG_AGARI_WINDOW));
	yakuData = YakuResult::getYakuStat();
	(void)yakuData.yakuList(&yakuList);
	myTextRenderer = new TextRenderer(device);
	agariTehai = new AgariTehai(this);
	agariNaki = new AgariNaki(this);
	doraTilesOmote = new DoraTilesOmote(this);
	showScore = new ShowScore(this);
	bgmFlag = true;
	maxShown = -1;
}
TableSubsceneAgariScreenProto::~TableSubsceneAgariScreenProto() {
	delete showScore;
	delete doraTilesOmote;
	delete agariNaki;
	delete agariTehai;
	delete myTextRenderer;
#if defined(_WIN32) && defined(WITH_DIRECTX)
	windowTexture->Release();
#endif
}

TableSubsceneAgariScreenProto::AgariStyle TableSubsceneAgariScreenProto::getAgariStyle() {
	const GameTable* const gameStat = GameStatus::gameStat();
	if (gameStat->CurrentPlayer.Agari == gameStat->PlayerID) {
		return agariMine;
	} else {
		if (utils::isPaoAgari(gameStat, gameStat->CurrentPlayer.Agari)) {
			if (utils::isPao(gameStat, gameStat->CurrentPlayer.Agari, gameStat->PlayerID))
				return agariFurikomi;
			else if (gameStat->CurrentPlayer.Furikomi == gameStat->PlayerID)
				return agariFurikomi;
			else
				return agariOthers;
		} else {
			if (gameStat->TsumoAgariFlag)
				return agariFurikomi;
			else if (gameStat->CurrentPlayer.Furikomi == gameStat->PlayerID)
				return agariFurikomi;
			else
				return agariOthers;
		}
	}
}
ArgbColor TableSubsceneAgariScreenProto::baseColor() {
	switch (getAgariStyle()) {
		case agariMine:     return 0xffffdf00;
		case agariFurikomi: return 0xffff7f7f;
		case agariOthers:   return 0xffffffff;
		default:            return 0x00000000;
	}
}

void TableSubsceneAgariScreenProto::renderWindow() {
	const double Zeit = seconds();
	const int yOffset = (Zeit >= 1.0) ? 0 : (int)(pow(1.0 - Zeit, 2) * (double)Geometry::BaseSize);
	SpriteRenderer::instantiate(myDevice)->ShowSprite(
		windowTexture, BaseX, BaseY - yOffset, yakuWndWidth, yakuWndHeight, baseColor());
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
		if (bgmFlag) {
			if (YakuResult::getYakuStat().AgariPoints < (GameStatus::gameStat()->chkGameType(GuobiaoMJ) ? 24 : 2000)) {
				switch (getAgariStyle()) {
					case agariMine:     sound::util::bgmplay(sound::IDs::musAgariSelf1); break;
					case agariFurikomi: sound::util::bgmplay(sound::IDs::musAgariFurikomi1); break;
					case agariOthers:   sound::util::bgmplay(sound::IDs::musAgariOther1); break;
				}
			} else if (YakuResult::getYakuStat().AgariPoints < (GameStatus::gameStat()->chkGameType(GuobiaoMJ) ? 64 : 8000)) {
				switch (getAgariStyle()) {
					case agariMine:     sound::util::bgmplay(sound::IDs::musAgariSelf2); break;
					case agariFurikomi: sound::util::bgmplay(sound::IDs::musAgariFurikomi2); break;
					case agariOthers:   sound::util::bgmplay(sound::IDs::musAgariOther2); break;
				}
			} else {
				switch (getAgariStyle()) {
					case agariMine:     sound::util::bgmplay(sound::IDs::musAgariSelf3); break;
					case agariFurikomi: sound::util::bgmplay(sound::IDs::musAgariFurikomi3); break;
					case agariOthers:   sound::util::bgmplay(sound::IDs::musAgariOther3); break;
				}
			}
			bgmFlag = false;
		}
		// 幅計算
		const float compressRate = [this](const CodeConv::tstring& str) -> float {
			const int cols = myTextRenderer->strWidthByCols(str);
			return (cols > 12) ? (8.0f / (float)cols) : 1.0f;
		} (yakuList[yakuNum].first);
		// 表示
		const ArgbColor color = (Zeit >= anmTime) ? baseColor() : ((255 - (int)((anmTime - Zeit) * 300)) << 24 | (baseColor() & 0x00ffffff));
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
	for (unsigned i = 0; i < yakuList.size(); ++i) {
		const bool isShown = renderYakuName(i);
		if (isShown && (maxShown < (signed)i)) {
			maxShown = (signed)i;
			sound::Play(sound::IDs::sndYakulst1);
		}
	}
}

// -------------------------------------------------------------------------

TableSubsceneAgariScreen::TableSubsceneAgariScreen(DevicePtr device) : TableSubsceneAgariScreenProto(device) {
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

TableSubsceneAgariScreenUradora::TableSubsceneAgariScreenUradora(DevicePtr device) : TableSubsceneAgariScreenProto(device) {
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
		sSelf, [](int){return (ArgbColor)0xffffffff;},
		[](const int*, const int*, int){});
}

void TableSubsceneAgariScreenProto::AgariTehai::Render() {
	const double Zeit = myCaller->seconds();
	if (reconstFlag) Reconstruct(GameStatus::gameStat());
	if (Zeit >= 1.0) reconstFlag = false;
	TileTexture->Render();
}

// -------------------------------------------------------------------------

/* 鳴いた牌を表示する */
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
	if (gameStat->chkGameType(GuobiaoMJ)) return;
	const int numberOfTiles = [gameStat]() -> int {
		if (gameStat->chkGameType(AllSanma))
			return 108;
		else if (rules::chkRule("flower_tiles", "no"))
			return 136;
		else if (rules::chkRule("flower_tiles", "8tiles"))
			return 144;
		else
			return 140;
	}();
	const int DoraPosStart = numberOfTiles - tileIdOffset() - 4 - gameStat->ExtraRinshan -
		[gameStat]() -> int {
			if (gameStat->chkGameType(AllSanma))
				return 0;
			else if (rules::chkRule("flower_tiles", "no"))
				return 0;
			else if (rules::chkRule("flower_tiles", "8tiles"))
				return 8;
			else
				return 4;
		}();
	const int DoraPosEnd = numberOfTiles - tileIdOffset() - gameStat->DeadTiles;
	for (int i = DoraPosStart; i > DoraPosEnd; i -= 2) {
		const int tileIndex = (i - DoraPosStart) / (-2);
		const double Zeit = myCaller->seconds() - (startTime() + (double)tileIndex * 0.0625);
		if (Zeit >= 0.0) {
			const ArgbColor color = (Zeit >= 0.325) ? 0xffffffff : ((255 - (int)((0.325 - Zeit) * 700)) << 24 | 0x00ffffff);
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
	soundFlag = true;
	timeFlag = true;
}
TableSubsceneAgariScreenProto::ShowScore::~ShowScore() {
	delete txtRenderer;
	delete digitRenderer;
}
void TableSubsceneAgariScreenProto::ShowScore::ReconstructScoreFuHan() {
	const double Zeit = myCaller->seconds() - (yakuAnimStartSecond + yakuInterval * myCaller->yakuList.size());
	if (Zeit <= 0.0) return;
	if (soundFlag) { // 効果音をここで鳴らす
		sound::Play(sound::IDs::sndYakulst2);
		soundFlag = false;
	}
	if (YakuResult::getYakuStat().isYakuman) return;
	if (GameStatus::gameStat()->gameType & GuobiaoMJ) return; // 中国ルールでは不要な情報なので
	const double anmTime = 0.75;
	CodeConv::tostringstream o;
	o << std::setw(3) << std::setfill(_T(' ')) << YakuResult::getYakuStat().BasePoints << _T("符") <<
		std::setw(2) << std::setfill(_T(' ')) << YakuResult::getYakuStat().TotalHan() << _T("飜");
	const int x = BaseX + yakuWndWidth - 32 - 27 * 9;
	const int y = BaseY + 650;
	const ArgbColor color = (Zeit >= anmTime) ? baseColor() : ((255 - (int)((anmTime - Zeit) * 300)) << 24 | (0x00ffffff & baseColor()));
	txtRenderer->NewText(0, o.str(), x, y, 1.0f, (YakuResult::getYakuStat().TotalHan() >= 100) ? (1.5f * 0.9f) : 1.5f, color);
}
void TableSubsceneAgariScreenProto::ShowScore::ReconstructScoreTxt() {
	const double Zeit = myCaller->seconds() - (yakuAnimStartSecond + yakuInterval * myCaller->yakuList.size());
	if (Zeit <= 0.0) return;
	const double anmTime = 0.75;
	const CodeConv::tstring scoreTxt = YakuResult::getAgariScore().to_str(_T(""), _T("-"));
	assert(scoreTxt != _T("0"));
	const unsigned txtWidthNoAdj = digitRenderer->strWidthByCols(scoreTxt) / 2u;
	const unsigned txtWidth = std::min(txtWidthNoAdj, 6u);
	const int x = BaseX + yakuWndWidth - 24 - 72 * txtWidth;
	const int y = BaseY + 700;
	const float scale = (Zeit >= anmTime) ? 1.0f : (pow(2.5f * (float)(anmTime - Zeit), 2) + 1.0f);
	const ArgbColor color = (Zeit >= anmTime) ? baseColor() : ((255 - (int)((anmTime - Zeit) * 300)) << 24 | (0x00ffffff & baseColor()));
	digitRenderer->NewText(0, scoreTxt,
		x - (int)(36.0f * (float)txtWidth * (scale - 1.0f)),
		y - (int)(48.0f * (scale - 1.0f)),
		scale,
		(txtWidthNoAdj < 6) ? 1.5f : (float)(1.5 * 6.0 / (double)txtWidthNoAdj),
		color);
	if ((timeFlag) && (Zeit >= 2.0)) {
		ui::UIEvent->set(0);
		timeFlag = false;
	}
}
void TableSubsceneAgariScreenProto::ShowScore::ReconstructScoreRank() {
	if ((GameStatus::gameStat()->gameType & RichiMJ) && rules::chkRule("limitless", "no")) {
		const double Zeit = myCaller->seconds() - (yakuAnimStartSecond + yakuInterval * myCaller->yakuList.size());
		if (Zeit <= 0.0) return;
		const double anmTime = 0.75;

		const unsigned score = static_cast<unsigned>(YakuResult::getYakuStat().AgariPoints);
		CodeConv::tstring tmptxt; unsigned strWidth = 0;
		if      (score ==  2000) {tmptxt = _T("満貫");   strWidth = 4;}
		else if (score ==  3000) {tmptxt = _T("跳満");   strWidth = 4;}
		else if (score ==  4000) {tmptxt = _T("倍満");   strWidth = 4;}
		else if (score ==  6000) {tmptxt = _T("三倍満"); strWidth = 6;}
		else if (score ==  8000) {
			if (YakuResult::getYakuStat().CoreSemiMangan + YakuResult::getYakuStat().BonusSemiMangan >= 8) {
				tmptxt = _T("役満");     strWidth = 4;
			} else {
				tmptxt = _T("数え役満"); strWidth = 8;
			}
		}
		else if (score == 16000) {tmptxt = _T("二倍役満"); strWidth = 8;}
		else if (score == 24000) {tmptxt = _T("三倍役満"); strWidth = 8;}
		else if (score == 32000) {tmptxt = _T("四倍役満"); strWidth = 8;}
		else if (score == 40000) {tmptxt = _T("五倍役満"); strWidth = 8;}
		else if (score == 48000) {tmptxt = _T("六倍役満"); strWidth = 8;}
		else if (score == 56000) {tmptxt = _T("七倍役満"); strWidth = 8;}
		else if (score == 64000) {tmptxt = _T("八倍役満"); strWidth = 8;}
		else if (score == 72000) {tmptxt = _T("九倍役満"); strWidth = 8;}
		else if (score == 80000) {tmptxt = _T("十倍役満"); strWidth = 8;}
		else if (score >= 88000) {
			CodeConv::tostringstream o;
			o << score / 8000; strWidth = o.str().length();
			o << "倍役満"; strWidth += 6;
			tmptxt = o.str();
		}

		if (strWidth) {
			const int x = BaseX + 30;
			const int y = BaseY + 720;
			const float scale = (Zeit >= anmTime) ? 1.0f : (pow(2.5f * (float)(anmTime - Zeit), 2) + 1.0f);
			const ArgbColor color = (Zeit >= anmTime) ? baseColor() : ((255 - (int)((anmTime - Zeit) * 300)) << 24 | (0x00ffffff & baseColor()));
			txtRenderer->NewText(1, tmptxt,
				x - (int)(54.0f * (scale - 1.0f)),
				y - (int)(36.0f * (scale - 1.0f)),
				2.0f * scale, 6.0f / (float)strWidth, color);
		}
	}
}
void TableSubsceneAgariScreenProto::ShowScore::ReconstructChipAmount() {
	if (GameStatus::gameStat()->gameType & GuobiaoMJ) return;
	if (rules::chkRule("chip", "no")) return;
	const double Zeit = myCaller->seconds() - (yakuAnimStartSecond + yakuInterval * myCaller->yakuList.size());
	if (Zeit <= 0.0) return;
	if (YakuResult::getYakuStat().isYakuman) return;
	const double anmTime = 0.75;
	CodeConv::tostringstream o;
	o << _T("チップ") << std::setw(2) << std::setfill(_T(' ')) << YakuResult::getChipVal();
	if (GameStatus::gameStat()->TsumoAgariFlag) {
		if (GameStatus::gameStat()->chkGameType(SanmaT))
			o << _T("ｘ２");
		else
			o << _T("ｘ３");
	}
	o << _T("枚");
	const int x = BaseX + yakuWndWidth - 32 - 27 * 10;
	const int y = BaseY + yakuWndHeight - 70;
	const ArgbColor color = (Zeit >= anmTime) ? baseColor() : ((255 - (int)((anmTime - Zeit) * 300)) << 24 | (0x00ffffff & baseColor()));
	txtRenderer->NewText(2, o.str(), x, y, 1.0f, (GameStatus::gameStat()->TsumoAgariFlag) ? (1.5f * 5.0f / 7.0f) : 1.5f, color);
}
void TableSubsceneAgariScreenProto::ShowScore::Reconstruct() {
	ReconstructScoreFuHan();
	ReconstructScoreTxt();
	ReconstructScoreRank();
	ReconstructChipAmount();
}
void TableSubsceneAgariScreenProto::ShowScore::Render() {
	Reconstruct();
	txtRenderer->Render();
	digitRenderer->Render();
}

void TableSubsceneAgariScreenProto::skipEvent() {
	if (seconds() < yakuAnimStartSecond) {
		sound::Play(sound::IDs::sndClick);
		myTimer.skipTo(static_cast<TimerMicrosec>(yakuAnimStartSecond * 1000000));
	} else if (seconds() < (yakuAnimStartSecond + yakuInterval * yakuList.size())) {
		sound::Play(sound::IDs::sndClick);
		myTimer.skipTo(static_cast<TimerMicrosec>((yakuAnimStartSecond + yakuInterval * yakuList.size()) * 1000000));
	} else {
		sound::Play(sound::IDs::sndClick);
		ui::UIEvent->set(0);
	}
}

// -------------------------------------------------------------------------

}
