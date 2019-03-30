#include "game.h"
#include "../scrmanip.h"
#include "../resource.h"
#include "../init.h"
#include "../loadtex.h"
#include "../sprite.h"
#include "../gametbl.h"
#include "../utils.h"
#include "../rule.h"
#include "../matrix.h"
#include <cassert>
#include <cstdlib>
#include "../../sound/sound.h"
#include "../../common/bgmid.h"

namespace mihajong_graphic {

// -------------------------------------------------------------------------

void TableProtoScene::LoadTexture(TexturePtr* texture, LPCTSTR resource) {
	mihajong_graphic::LoadTexture(caller->getDevice(), texture, resource);
}

const std::array<CodeConv::tstring, TableProtoScene::NumOfCheckBoxes> TableProtoScene::labels = {
	_T("自動和了"), _T("ツモ切り"), _T("オートパス"),
};

TableProtoScene::TableProtoScene(ScreenManipulator* const manipulator) : Scene(manipulator) {
	scorePanel = {}; checkBoxes = {};
	LoadTexture(&tSideBar, MAKEINTRESOURCE(IDB_PNG_SDBAR));
	InitScorePanel();
	constexpr Region nullRegion = {0, 0, -1, -1};
	for (int i = 0; i < NumOfCheckBoxes; ++i) {
		checkBoxes[i] = new CheckBox(manipulator->getDevice(), labels[i], Geometry::BaseSize + 20, 940 + i * 40);
		setRegion(i + CheckboxRegionOffset,
			Geometry::BaseSize + 20,                                        940 + i * 40,
			Geometry::BaseSize + 20 + 36 + checkBoxes[i]->captionWidthPx(), 940 + i * 40 + 36);
	}
}

TableProtoScene::~TableProtoScene() {
	for (int i = 0; i < NumOfCheckBoxes; ++i)
		delete checkBoxes[i];
	for (const auto& k : scorePanel)
		delete k;
#if defined(_WIN32) && defined(WITH_DIRECTX)
	if (tSideBar) tSideBar->Release();
#endif
}

void TableProtoScene::ShowSidebar() {
	SpriteRenderer::instantiate(caller->getDevice())->ShowSprite(
		tSideBar, Geometry::BaseSize, 0, Geometry::SidebarWidth(), 1080);
	for (int i = 0; i < NumOfCheckBoxes; ++i)
		checkBoxes[i]->Render();
}

void TableProtoScene::InitScorePanel() {
	const float widthScale = Geometry::SidebarWidth() / (Geometry::BaseSize / 0.75f - Geometry::BaseSize);
	if (widthScale >= 1.5f) {
		scorePanel[0] = new ScoreBoard(caller->getDevice(), sOpposite,
			Geometry::BaseSize + Geometry::SidebarWidth() / 3     + 3, panelPosY +  62, widthScale / 1.5f);
		scorePanel[1] = new ScoreBoard(caller->getDevice(), sLeft,
			Geometry::BaseSize                                    + 3, panelPosY + 125, widthScale / 1.5f);
		scorePanel[2] = new ScoreBoard(caller->getDevice(), sRight,
			Geometry::BaseSize + Geometry::SidebarWidth() / 3 * 2 + 3, panelPosY + 125, widthScale / 1.5f);
		scorePanel[3] = new ScoreBoard(caller->getDevice(), sSelf,
			Geometry::BaseSize + Geometry::SidebarWidth() / 3     + 3, panelPosY + 188, widthScale / 1.5f);
	} else {
		scorePanel[0] = new ScoreBoard(caller->getDevice(), sOpposite,
			Geometry::BaseSize + Geometry::SidebarWidth() / 4    , panelPosY      , widthScale);
		scorePanel[1] = new ScoreBoard(caller->getDevice(), sLeft,
			Geometry::BaseSize                                + 3, panelPosY + 125, widthScale);
		scorePanel[2] = new ScoreBoard(caller->getDevice(), sRight,
			Geometry::BaseSize + Geometry::SidebarWidth() / 2 + 3, panelPosY + 125, widthScale);
		scorePanel[3] = new ScoreBoard(caller->getDevice(), sSelf,
			Geometry::BaseSize + Geometry::SidebarWidth() / 4    , panelPosY + 250, widthScale);
	}
}

void TableProtoScene::ShowScorePanel() {
	for (const auto& k : scorePanel)
		k->Render();
}

ArgbColor TableProtoScene::roundColor() {
	switch (GameStatus::gameStat()->GameRound / Players) { // 場風で分岐
	case 0: // 東場
		if (GameStatus::gameStat()->LoopRound == 0)
			return 0xff008000;
		else if ((GameStatus::gameStat()->LoopRound % 2) == 0)
			return 0xff408000;
		else
			return 0xff808080;
	case 1: // 南場
		if (GameStatus::gameStat()->LoopRound == 0)
			return 0xff008080;
		else if ((GameStatus::gameStat()->LoopRound % 2) == 0)
			return 0xff808040;
		else
			return 0xff808000;
	case 2: // 西場
		if ((GameStatus::gameStat()->LoopRound % 2) == 0)
			return 0xff404080;
		else
			return 0xff800000;
	case 3: // 北場
		if ((GameStatus::gameStat()->LoopRound % 2) == 0)
			return 0xff800080;
		else
			return 0xff404040;
	case 4: // 白場
		return     0xff606080;
	case 5: // 發場
		return     0xff608060;
	case 6: // 中場
		return     0xff806060;
	default:
		return     0xff000000;
	}
}

#ifdef _WIN32
void TableProtoScene::MouseInput(LPDIDEVICEOBJECTDATA od, int X, int Y)
#else /*_WIN32*/
void TableProtoScene::MouseInput(const XEvent* od, int X, int Y)
#endif /*_WIN32*/
{
	const int scaledX = static_cast<int>(static_cast<float>(X) / Geometry::WindowScale());
	const int scaledY = static_cast<int>(static_cast<float>(Y) / Geometry::WindowScale());
	const int region = whichRegion(scaledX, scaledY);
	const bool isCheckBox = (region >= CheckboxRegionOffset) &&
		(region < (CheckboxRegionOffset + NumOfCheckBoxes));
#ifdef _WIN32
	switch (od->dwOfs)
#else /*_WIN32*/
	switch (od->type)
#endif /*_WIN32*/
	{
#ifdef _WIN32
	case DIMOFS_X: case DIMOFS_Y: // マウスカーソルを動かした場合
#else /*_WIN32*/
	case MotionNotify: // マウスカーソルを動かした場合
#endif /*_WIN32*/
		if ((isCheckBox) && (!checkBoxes[static_cast<std::size_t>(region) - CheckboxRegionOffset]->isFocused())) {
			checkBoxes[static_cast<std::size_t>(region) - CheckboxRegionOffset]->focus(true);
			sound::Play(sound::IDs::sndCursor);
		}
		for (int i = 0; i < NumOfCheckBoxes; ++i)
			if (region != i + CheckboxRegionOffset)
				checkBoxes[i]->focus(false);
		break;
#ifdef _WIN32
	case DIMOFS_BUTTON0: // マウスクリック
		if ((isCheckBox) && (od->dwData))
#else /*_WIN32*/
	case ButtonPress: // マウスクリック
		if ((isCheckBox) && (od->xbutton.button == Button1))
#endif /*_WIN32*/
		{
			checkBoxes[static_cast<std::size_t>(region) - CheckboxRegionOffset]->check(
				!(checkBoxes[static_cast<std::size_t>(region) - CheckboxRegionOffset]->isChecked()));
			sound::Play(sound::IDs::sndClick);
		}
		break;
	}
}

// -------------------------------------------------------------------------

PlayerID TableProtoScene::ScoreBoard::playerID() {
	return utils::RelativePositionOf(GameStatus::gameStat()->PlayerID, relativePlayerID);
}

TableProtoScene::ScoreBoard::ScoreBoard(DevicePtr device, seatRelative relativePos, int x, int y, float widthScale) {
	myDevice = device; relativePlayerID = relativePos; xpos = x; ypos = y; wScale = widthScale;
	mihajong_graphic::LoadTexture(myDevice, &texture, MAKEINTRESOURCE(IDB_PNG_SCORE_INDICATOR));
	nameText = new SmallTextRenderer(device);
#ifndef _WIN32
	initialized = false;
}
void TableProtoScene::ScoreBoard::objInit() {
	const int x = xpos, y = ypos;
#endif /*_WIN32*/
	// 行列の構築
	myMatrix = getMatrix(static_cast<float>(x), static_cast<float>(y), wScale, 1.0f);
}

TableProtoScene::ScoreBoard::~ScoreBoard() {
#if defined(_WIN32) && defined(WITH_DIRECTX)
	if (texture) texture->Release();
#endif
	delete nameText;
}

TableProtoScene::ScoreBoard::ScoreMode TableProtoScene::ScoreBoard::getScoreMode() {
	return static_cast<ScoreMode>((myTimer.currTime() / 2000000) % (((GameStatus::gameStat()->gameType & GuobiaoMJ) || rules::chkRule("chip", "no")) ? 2 : 3));
}

void TableProtoScene::ScoreBoard::Render() {
#ifndef _WIN32
	if (!initialized) {
		objInit();
		initialized = true;
	}
#endif /*_WIN32*/
	const RECT rect = {0, 0, PanelWidth, PanelHeight};
	SpriteRenderer::instantiate(myDevice)->ShowSprite(texture, static_cast<int>(xpos), static_cast<int>(ypos),
		PanelWidth, PanelHeight, 0xffffffff, &rect, 0, 0, &myMatrix);
	if ((playerID() >= 0) && (playerID() < (GameStatus::gameStat()->chkGameType(SanmaT) ? 3 : 4))) {
		renderWind();
		renderRank();
		renderScore();
		renderName();
	}
}

void TableProtoScene::ScoreBoard::renderWind() {
	if ((myTimer.currTime() % 1000000 >= 500000) && (GameStatus::gameStat()->CurrentPlayer.Active == playerID())) return; // ツモ番の時は表示を点滅させる
	const seatAbsolute wind = GameStatus::gameStat()->playerwind(playerID());
	if (GameStatus::gameStat()->chkGameType(Sanma4) && (wind == sNorth)) return; // 四人三麻の時の抜け番は何も表示しないようにする
	const RECT rect = {
		static_cast<int32_t>(WindCharX + WindCharWidth * (static_cast<int>(wind)    )), WindCharY,
		static_cast<int32_t>(WindCharX + WindCharWidth * (static_cast<int>(wind) + 1)), WindCharY + WindCharHeight
	};
	SpriteRenderer::instantiate(myDevice)->ShowSprite(texture, static_cast<int>(xpos) + WindPosX, static_cast<int>(ypos) + WindPosY,
		WindCharWidth, WindCharHeight,
		(wind == sEast) ? ledColorRed : ledColorGreen, // 「東」のみ赤で、それ以外を緑で表示すればわかりやすいと思うのでそうする
		&rect, 0, 0, &myMatrix);
}

void TableProtoScene::ScoreBoard::renderNumeral(int x, int y, unsigned num, ArgbColor color) {
	const RECT rect = {
		static_cast<int32_t>(NumCharX + NumCharWidth * (num    )), NumCharY,
		static_cast<int32_t>(NumCharX + NumCharWidth * (num + 1)), NumCharY + NumCharHeight
	};
	SpriteRenderer::instantiate(myDevice)->ShowSprite(texture, static_cast<int>(xpos) + x, static_cast<int>(ypos) + y,
		NumCharWidth, NumCharHeight, color, &rect, 0, 0, &myMatrix);
}

void TableProtoScene::ScoreBoard::renderRank() {
	PlayerRankList rankList = utils::calcRank(GameStatus::gameStat());
	const ArgbColor color =
		(rankList[playerID()] == 1) ? ledColorRed : // トップは赤
		(rankList[playerID()] == (GameStatus::gameStat()->chkGameType(SanmaT) ? 3 : 4) ? ledColorOrange : // ラスはオレンジ
		ledColorGreen); // その他は緑で表示

	renderNumeral(RankPosX, RankPosY, rankList[playerID()], color); // その他は緑で表示
	if ((myTimer.currTime() % 1000000 < 500000) && (GameStatus::gameStat()->CurrentPlayer.Active == playerID()))
		renderNumeral(RankPosX, RankPosY, digitDecimal, color);
}

int TableProtoScene::ScoreBoard::getScoreSign() {
	for (int i = DigitGroups - 1; i >= 0; --i)
		if (GameStatus::gameStat()->Player[playerID()].PlayerScore.digitGroup[i] > 0) return 1;
		else if (GameStatus::gameStat()->Player[playerID()].PlayerScore.digitGroup[i] < 0) return -1;
	return 0;
}

std::tuple<unsigned, unsigned, signed, signed> TableProtoScene::ScoreBoard::scoreInfo(ScoreMode scoreMode) {
	if (scoreMode == scoreChip) {
		const int& chipAmount = GameStatus::gameStat()->Player[playerID()].playerChip;
		return std::make_tuple(abs(chipAmount), 0, 0, (chipAmount > 0) ? 1 : (chipAmount < 0) ? -1 : 0);
	} else {
		const LargeNum playerScoreDiff = GameStatus::gameStat()->Player[playerID()].PlayerScore - GameStatus::gameStat()->statOfMine().PlayerScore;
		const LargeNum* const score = (scoreMode == scoreDiff) ?
			&playerScoreDiff :
			&(GameStatus::gameStat()->Player[playerID()].PlayerScore);
		constexpr int digit[10] = {1, 10, 100, 1000, 10000, 100000, 1000000, 10000000, 100000000, 1000000000};
		int sign;
		if      (*score > 0) sign =  1;
		else if (*score < 0) sign = -1;
		else                 sign =  0;
		for (int i = DigitGroups - 1; i >= 0; --i) {
			for (int j = ((i == DigitGroups - 1) ? 9 : 7); j >= 0; --j) {
				if ((GameStatus::gameStat()->gameType & RichiMJ) && (i == 0) && (j == 4)) {
					return std::make_tuple(abs(score->digitGroup[0] / 100), 0, 0, sign);
				}
				else if ((GameStatus::gameStat()->gameType & GuobiaoMJ) && (i == 0) && (j == 2)) {
					return std::make_tuple(abs(score->digitGroup[0]), 0, 0, sign);
				}
				else if (score->digitGroup[i] / digit[j]) {
					unsigned digitCode = i * 8 + j; unsigned digits;
					if (j >= 2) {
						digits = abs(score->digitGroup[i] / digit[j - 2]);
					} else {
						assert(i > 0);
						digits = abs(score->digitGroup[i] * digit[2 - j]) + abs(score->digitGroup[i - 1] / digit[j + 6]);
					}
					if (digitCode % 4 == 3)
						return std::make_tuple(digits, digitCode / 4 + 1, -1, sign);
					else
						return std::make_tuple(digits, digitCode / 4, digitCode % 4, sign);
				}
			}
		}
	}
	throw _T("ScoreBoard::scoreInfo() : Unexpected control flow");
}

void TableProtoScene::ScoreBoard::renderScore() {
	unsigned digits, unitcode; signed decimalPos, sign;
	ArgbColor color;
	ScoreMode scoreMode = getScoreMode();

	std::tie(digits, unitcode, decimalPos, sign) = scoreInfo(scoreMode);
	switch (scoreMode) {
	case scorePoints:
		if (utils::isAboveBase(GameStatus::gameStat(), playerID()))
			color = ledColorRed; // 浮いていれば赤
		else
			color = ledColorGreen; // 沈みは緑
		break;
	case scoreDiff: case scoreChip:
		if      (sign ==  1) color = ledColorRed;
		else if (sign == -1) color = ledColorGreen;
		else                 color = ledColorOrange;
		break;
	default:
		color = ledColorGreen; // 一応
	}

	if ((scoreMode != scoreDiff) || (playerID() != GameStatus::gameStat()->PlayerID)) {
		if (sign == 1)
			renderNumeral(ScorePosX - NumCharWidth    , ScorePosY, digitPlus         , color);
		else if (sign == -1)
			renderNumeral(ScorePosX - NumCharWidth    , ScorePosY, digitMinus        , color);
		if ((unitcode != 0) || (digits / 100))
			renderNumeral(ScorePosX                   , ScorePosY, digits       / 100, color);
		if ((unitcode != 0) || (digits / 100) || (digits % 100 / 10))
			renderNumeral(ScorePosX + NumCharWidth    , ScorePosY, digits % 100 / 10 , color);
		renderNumeral(    ScorePosX + NumCharWidth * 2, ScorePosY, digits % 10       , color);
		if (unitcode != 0)
			renderNumeral(ScorePosX + NumCharWidth * decimalPos, ScorePosY, digitDecimal, color);
		if ((GameStatus::gameStat()->gameType & RichiMJ) && (scoreMode != scoreChip))
			renderScoreUnit(unitcode, color);
	}
}

void TableProtoScene::ScoreBoard::renderScoreUnit(unsigned unitnum, ArgbColor color) {
	const RECT rect = {
		static_cast<int32_t>(ScoreUnitCharX + ScoreUnitCharWidth * (unitnum    )), ScoreUnitCharY,
		static_cast<int32_t>(ScoreUnitCharX + ScoreUnitCharWidth * (unitnum + 1)), ScoreUnitCharY + ScoreUnitCharHeight
	};
	SpriteRenderer::instantiate(myDevice)->ShowSprite(texture, static_cast<int>(xpos) + ScoreUnitPosX, static_cast<int>(ypos) + ScoreUnitPosY,
		ScoreUnitCharWidth, ScoreUnitCharHeight, color, &rect, 0, 0, &myMatrix);
}

void TableProtoScene::ScoreBoard::renderName() {
	const CodeConv::tstring pName(utils::getName(playerID()));
	const unsigned tmpWidth = nameText->strWidthByCols(pName);
	nameText->NewText(0,
		(getScoreMode() == scoreDiff) ? _T("点差表示") : ((getScoreMode() == scoreChip) ? _T("チップ表示") : pName),
		xpos + NamePosX, ypos + NamePosY, 1.0, ((tmpWidth > 18) ? (18.0f / static_cast<float>(tmpWidth)) : 1.0f) * wScale);
	nameText->Render();
}

}
