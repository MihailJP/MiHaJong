#include "game.h"
#include "../scrmanip.h"
#include "../resource.h"
#include "../init.h"
#include "../loadtex.h"
#include "../sprite.h"
#include "../gametbl.h"
#include "../utils.h"
#include "../rule.h"
#include <cassert>
#include <cstdlib>

namespace mihajong_graphic {

// -------------------------------------------------------------------------

void TableProtoScene::LoadTexture(LPDIRECT3DTEXTURE9* texture, LPCTSTR resource, unsigned width, unsigned height) {
	mihajong_graphic::LoadTexture(caller->getDevice(), texture, resource, width, height);
}

TableProtoScene::TableProtoScene(ScreenManipulator* const manipulator) : Scene(manipulator) {
	LoadTexture(&tSideBar, MAKEINTRESOURCE(IDB_PNG_SDBAR), 960, 1080);
	InitScorePanel();
}

TableProtoScene::~TableProtoScene() {
	for (auto k = scorePanel.begin(); k != scorePanel.end(); ++k)
		delete *k;
	if (tSideBar) tSideBar->Release();
}

void TableProtoScene::ShowSidebar() {
	SpriteRenderer::instantiate(caller->getDevice())->ShowSprite(
		tSideBar, Geometry::BaseSize, 0, Geometry::SidebarWidth(), 1080);
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
	for (auto k = scorePanel.begin(); k != scorePanel.end(); ++k)
		(*k)->Render();
}

D3DCOLOR TableProtoScene::roundColor() {
	switch (GameStatus::gameStat()->GameRound / Players) { // 場風で分岐
	case 0: // 東場
		if (GameStatus::gameStat()->LoopRound == 0)
			return D3DCOLOR_XRGB(  0, 128,   0);
		else if ((GameStatus::gameStat()->LoopRound % 2) == 0)
			return D3DCOLOR_XRGB( 64, 128,   0);
		else
			return D3DCOLOR_XRGB(128, 128, 128);
	case 1: // 南場
		if (GameStatus::gameStat()->LoopRound == 0)
			return D3DCOLOR_XRGB(  0, 128, 128);
		else if ((GameStatus::gameStat()->LoopRound % 2) == 0)
			return D3DCOLOR_XRGB(128, 128,  64);
		else
			return D3DCOLOR_XRGB(128, 128,   0);
	case 2: // 西場
		if ((GameStatus::gameStat()->LoopRound % 2) == 0)
			return D3DCOLOR_XRGB( 64,  64, 128);
		else
			return D3DCOLOR_XRGB(128,   0,   0);
	case 3: // 北場
		if ((GameStatus::gameStat()->LoopRound % 2) == 0)
			return D3DCOLOR_XRGB(128,   0, 128);
		else
			return D3DCOLOR_XRGB( 64,  64,  64);
	case 4: // 白場
		return     D3DCOLOR_XRGB( 96,  96, 128);
	case 5: // 發場
		return     D3DCOLOR_XRGB( 96, 128,  96);
	case 6: // 中場
		return     D3DCOLOR_XRGB(128,  96,  96);
	default:
		return     D3DCOLOR_XRGB(  0,   0,   0);
	}
}

// -------------------------------------------------------------------------

unsigned long long TableProtoScene::ScoreBoard::currTime() { // 現在時刻
	FILETIME czas; GetSystemTimeAsFileTime(&czas);
	return ((unsigned long long)czas.dwHighDateTime << 32) | czas.dwLowDateTime;
}

PlayerID TableProtoScene::ScoreBoard::playerID() {
	return utils::RelativePositionOf(GameStatus::gameStat()->PlayerID, relativePlayerID);
}

TableProtoScene::ScoreBoard::ScoreBoard(LPDIRECT3DDEVICE9 device, seatRelative relativePos, int x, int y, float widthScale) {
	myDevice = device; relativePlayerID = relativePos; xpos = x; ypos = y; wScale = widthScale;
	mihajong_graphic::LoadTexture(myDevice, &texture, MAKEINTRESOURCE(IDB_PNG_SCORE_INDICATOR), 860, 120);
	nameText = new SmallTextRenderer(device);
	// 行列の構築
	D3DXMATRIX tmpmtx;
	D3DXMatrixIdentity(&myMatrix); D3DXMatrixIdentity(&tmpmtx);
	D3DXMatrixScaling(&tmpmtx, Geometry::WindowScale(), Geometry::WindowScale(), 0.0f); D3DXMatrixMultiply(&myMatrix, &myMatrix, &tmpmtx);
	D3DXMatrixTranslation(&tmpmtx, (float)(-x) * Geometry::WindowScale(), (float)(-y) * Geometry::WindowScale(), 0.0f); D3DXMatrixMultiply(&myMatrix, &myMatrix, &tmpmtx);
	D3DXMatrixScaling(&tmpmtx, wScale, 1.0f, 0.0f); D3DXMatrixMultiply(&myMatrix, &myMatrix, &tmpmtx);
	D3DXMatrixTranslation(&tmpmtx, (float)x * Geometry::WindowScale(), (float)y * Geometry::WindowScale(), 0.0f); D3DXMatrixMultiply(&myMatrix, &myMatrix, &tmpmtx);
}

TableProtoScene::ScoreBoard::~ScoreBoard() {
	if (texture) texture->Release();
	delete nameText;
}

TableProtoScene::ScoreBoard::ScoreMode TableProtoScene::ScoreBoard::getScoreMode() {
	return (ScoreMode)((currTime() / 20000000) % ((rules::chkRule("chip", "no")) ? 2 : 3));
}

void TableProtoScene::ScoreBoard::Render() {
	RECT rect = {0, 0, PanelWidth, PanelHeight};
	SpriteRenderer::instantiate(myDevice)->ShowSprite(texture, (int)xpos, (int)ypos,
		PanelWidth, PanelHeight, 0xffffffff, &rect, 0, 0, &myMatrix);
	renderWind();
	renderRank();
	renderScore();
	renderName();
}

void TableProtoScene::ScoreBoard::renderWind() {
	if ((currTime() % 10000000 >= 5000000) && (GameStatus::gameStat()->CurrentPlayer.Active == playerID())) return; // ツモ番の時は表示を点滅させる
	const seatAbsolute wind = utils::playerwind(GameStatus::gameStat(), playerID(), GameStatus::gameStat()->GameRound);
	RECT rect = {
		WindCharX + WindCharWidth * ((int)wind    ), WindCharY,
		WindCharX + WindCharWidth * ((int)wind + 1), WindCharY + WindCharHeight
	};
	SpriteRenderer::instantiate(myDevice)->ShowSprite(texture, (int)xpos + WindPosX, (int)ypos + WindPosY,
		WindCharWidth, WindCharHeight,
		(wind == sEast) ? ledColorRed : ledColorGreen, // 「東」のみ赤で、それ以外を緑で表示すればわかりやすいと思うのでそうする
		&rect, 0, 0, &myMatrix);
}

void TableProtoScene::ScoreBoard::renderNumeral(int x, int y, unsigned num, D3DCOLOR color) {
	RECT rect = {
		NumCharX + NumCharWidth * (num    ), NumCharY,
		NumCharX + NumCharWidth * (num + 1), NumCharY + NumCharHeight
	};
	SpriteRenderer::instantiate(myDevice)->ShowSprite(texture, (int)xpos + x, (int)ypos + y,
		NumCharWidth, NumCharHeight, color, &rect, 0, 0, &myMatrix);
}

void TableProtoScene::ScoreBoard::renderRank() {
	PlayerRankList rankList = utils::calcRank(GameStatus::gameStat());
	const D3DCOLOR color =
		(rankList[playerID()] == 1) ? ledColorRed : // トップは赤
		(rankList[playerID()] == (chkGameType(GameStatus::gameStat(), SanmaT) ? 3 : 4) ? ledColorOrange : // ラスはオレンジ
		ledColorGreen); // その他は緑で表示

	renderNumeral(RankPosX, RankPosY, rankList[playerID()], color); // その他は緑で表示
	if ((currTime() % 10000000 < 5000000) && (GameStatus::gameStat()->CurrentPlayer.Active == playerID()))
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
		const int digit[10] = {1, 10, 100, 1000, 10000, 100000, 1000000, 10000000, 100000000, 1000000000};
		int sign;
		if      (*score > LargeNum::fromInt(0)) sign =  1;
		else if (*score < LargeNum::fromInt(0)) sign = -1;
		else                                    sign =  0;
		for (int i = DigitGroups - 1; i >= 0; --i) {
			for (int j = ((i == DigitGroups - 1) ? 9 : 7); j >= 0; --j) {
				if ((i == 0) && (j == 4)) {
					return std::make_tuple(abs(score->digitGroup[0] / 100), 0, 0, sign);
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
	D3DCOLOR color;
	ScoreMode scoreMode = getScoreMode();

	std::tie(digits, unitcode, decimalPos, sign) = scoreInfo(scoreMode);
	switch (scoreMode) {
	case scorePoints:
		if (sign < 1) // プラスじゃなかったら緑
			color = ledColorGreen;
		else if (unitcode != 0) // 万単位以上の表示だったら赤
			color = ledColorRed;
		else if (digits >= 400) // 4万以上は浮き
			color = ledColorRed;
		else if ((digits >= 300) && (!chkGameType(GameStatus::gameStat(), SanmaT))) // 三人打ち以外で3万以上は浮き
			color = ledColorRed;
		else if ((digits >= 300) && (// 30000点返しの浮きの場合
			rules::chkRule("starting_point", "25000pts_oka15") ||
			rules::chkRule("starting_point", "27000pts_oka9") ||
			rules::chkRule("starting_point", "30000pts_oka0")
			))
			color = ledColorRed;
		else
			color = ledColorGreen;
		break;
	case scoreDiff: case scoreChip:
		if      (sign ==  1) color = ledColorRed;
		else if (sign == -1) color = ledColorGreen;
		else                 color = ledColorOrange;
		break;
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
		if (scoreMode != scoreChip)
			renderScoreUnit(unitcode, color);
	}
}

void TableProtoScene::ScoreBoard::renderScoreUnit(unsigned unitnum, D3DCOLOR color) {
	RECT rect = {
		ScoreUnitCharX + ScoreUnitCharWidth * (unitnum    ), ScoreUnitCharY,
		ScoreUnitCharX + ScoreUnitCharWidth * (unitnum + 1), ScoreUnitCharY + ScoreUnitCharHeight
	};
	SpriteRenderer::instantiate(myDevice)->ShowSprite(texture, (int)xpos + ScoreUnitPosX, (int)ypos + ScoreUnitPosY,
		ScoreUnitCharWidth, ScoreUnitCharHeight, color, &rect, 0, 0, &myMatrix);
}

void TableProtoScene::ScoreBoard::renderName() {
	unsigned tmpWidth = 0;
	for (LPCTSTR k = utils::getName(playerID()); *k != _T('\0'); ++k) {
#ifdef _UNICODE
		if (*k >= 0x0080)
			tmpWidth += 2;
		else
#endif
			tmpWidth += 1;
	}
	nameText->NewText(0,
		(getScoreMode() == scoreDiff) ? _T("点差表示") : ((getScoreMode() == scoreChip) ? _T("チップ表示") : utils::getName(playerID())),
		xpos + NamePosX, ypos + NamePosY, 1.0, (tmpWidth > 18) ? (18.0f / (float)tmpWidth) : 1.0f);
	nameText->Render();
}

}
