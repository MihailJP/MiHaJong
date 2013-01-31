#include "game.h"
#include "../scrmanip.h"
#include "../resource.h"
#include "../init.h"
#include "../loadtex.h"
#include "../sprite.h"
#include "../gametbl.h"
#include "../utils.h"
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

// -------------------------------------------------------------------------

unsigned long long TableProtoScene::ScoreBoard::currTime() { // åªç›éûçè
	FILETIME czas; GetSystemTimeAsFileTime(&czas);
	return ((unsigned long long)czas.dwHighDateTime << 32) | czas.dwLowDateTime;
}

PLAYER_ID TableProtoScene::ScoreBoard::playerID() {
	return utils::RelativePositionOf(GameStatus::gameStat()->PlayerID, relativePlayerID);
}

TableProtoScene::ScoreBoard::ScoreBoard(LPDIRECT3DDEVICE9 device, seatRelative relativePos, int x, int y, float widthScale) {
	myDevice = device; relativePlayerID = relativePos; xpos = x; ypos = y; wScale = widthScale;
	mihajong_graphic::LoadTexture(myDevice, &texture, MAKEINTRESOURCE(IDB_PNG_SCORE_INDICATOR), 860, 120);
	// çsóÒÇÃç\íz
	D3DXMATRIX tmpmtx;
	D3DXMatrixIdentity(&myMatrix); D3DXMatrixIdentity(&tmpmtx);
	D3DXMatrixScaling(&tmpmtx, Geometry::WindowScale(), Geometry::WindowScale(), 0.0f); D3DXMatrixMultiply(&myMatrix, &myMatrix, &tmpmtx);
	D3DXMatrixTranslation(&tmpmtx, (float)(-x) * Geometry::WindowScale(), (float)(-y) * Geometry::WindowScale(), 0.0f); D3DXMatrixMultiply(&myMatrix, &myMatrix, &tmpmtx);
	D3DXMatrixScaling(&tmpmtx, wScale, 1.0f, 0.0f); D3DXMatrixMultiply(&myMatrix, &myMatrix, &tmpmtx);
	D3DXMatrixTranslation(&tmpmtx, (float)x * Geometry::WindowScale(), (float)y * Geometry::WindowScale(), 0.0f); D3DXMatrixMultiply(&myMatrix, &myMatrix, &tmpmtx);
}

TableProtoScene::ScoreBoard::~ScoreBoard() {
	if (texture) texture->Release();
}

void TableProtoScene::ScoreBoard::Render() {
	RECT rect = {0, 0, PanelWidth, PanelHeight};
	SpriteRenderer::instantiate(myDevice)->ShowSprite(texture, (int)xpos, (int)ypos,
		PanelWidth, PanelHeight, 0xffffffff, &rect, 0, 0, &myMatrix);
	renderWind();
	renderRank();
	renderScore();
}

void TableProtoScene::ScoreBoard::renderWind() {
	if ((currTime() % 10000000 >= 5000000) && (GameStatus::gameStat()->CurrentPlayer.Active == playerID())) return;
	RECT rect = {
		WindCharX + WindCharWidth * (utils::playerwind(GameStatus::gameStat(), playerID(), GameStatus::gameStat()->GameRound)    ), WindCharY,
		WindCharX + WindCharWidth * (utils::playerwind(GameStatus::gameStat(), playerID(), GameStatus::gameStat()->GameRound) + 1), WindCharY + WindCharHeight
	};
	SpriteRenderer::instantiate(myDevice)->ShowSprite(texture, (int)xpos + WindPosX, (int)ypos + WindPosY,
		WindCharWidth, WindCharHeight, 0xffff0000, &rect, 0, 0, &myMatrix);
}

void TableProtoScene::ScoreBoard::renderNumeral(int x, int y, unsigned num) {
	RECT rect = {
		NumCharX + NumCharWidth * (num    ), NumCharY,
		NumCharX + NumCharWidth * (num + 1), NumCharY + NumCharHeight
	};
	SpriteRenderer::instantiate(myDevice)->ShowSprite(texture, (int)xpos + x, (int)ypos + y,
		NumCharWidth, NumCharHeight, 0xffff0000, &rect, 0, 0, &myMatrix);
}

void TableProtoScene::ScoreBoard::renderRank() {
	PlayerRankList rankList = utils::calcRank(GameStatus::gameStat());
	renderNumeral(RankPosX, RankPosY, rankList.val[playerID()]);
	if ((currTime() % 10000000 < 5000000) && (GameStatus::gameStat()->CurrentPlayer.Active == playerID()))
		renderNumeral(RankPosX, RankPosY, digitDecimal);
}

int TableProtoScene::ScoreBoard::getScoreSign() {
	for (int i = DIGIT_GROUPS - 1; i >= 0; --i)
		if (GameStatus::gameStat()->Player.val[playerID()].PlayerScore.digitGroup[i] > 0) return 1;
		else if (GameStatus::gameStat()->Player.val[playerID()].PlayerScore.digitGroup[i] < 0) return -1;
	return 0;
}

std::tuple<unsigned, unsigned, signed> TableProtoScene::ScoreBoard::scoreInfo() {
	const LargeNum* const score = &(GameStatus::gameStat()->Player.val[playerID()].PlayerScore);
	const int digit[10] = {1, 10, 100, 1000, 10000, 100000, 1000000, 10000000, 100000000, 1000000000};
	for (int i = DIGIT_GROUPS - 1; i >= 0; --i) {
		for (int j = ((i == DIGIT_GROUPS - 1) ? 9 : 7); j >= 0; --j) {
			if ((i == 0) && (j == 4)) {
				return std::make_tuple(abs(score->digitGroup[0] / 100), 0, 0);
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
					return std::make_tuple(digits, digitCode / 4 + 1, -1);
				else
					return std::make_tuple(digits, digitCode / 4, digitCode % 4);
			}
		}
	}
	throw _T("ScoreBoard::scoreInfo() : Unexpected control flow");
}

void TableProtoScene::ScoreBoard::renderScore() {
	unsigned digits, unitcode; signed decimalPos;
	std::tie(digits, unitcode, decimalPos) = scoreInfo();
	if (getScoreSign() == 1)
		renderNumeral(ScorePosX - NumCharWidth, ScorePosY, digitPlus);
	else if (getScoreSign() == -1)
		renderNumeral(ScorePosX - NumCharWidth, ScorePosY, digitMinus);
	renderNumeral(ScorePosX                   , ScorePosY, digits       / 100);
	renderNumeral(ScorePosX + NumCharWidth    , ScorePosY, digits % 100 / 10);
	renderNumeral(ScorePosX + NumCharWidth * 2, ScorePosY, digits % 10);
	if (unitcode != 0)
		renderNumeral(ScorePosX + NumCharWidth * decimalPos, ScorePosY, digitDecimal);
	renderScoreUnit(unitcode);
}

void TableProtoScene::ScoreBoard::renderScoreUnit(unsigned unitnum) {
	RECT rect = {
		ScoreUnitCharX + ScoreUnitCharWidth * (unitnum    ), ScoreUnitCharY,
		ScoreUnitCharX + ScoreUnitCharWidth * (unitnum + 1), ScoreUnitCharY + ScoreUnitCharHeight
	};
	SpriteRenderer::instantiate(myDevice)->ShowSprite(texture, (int)xpos + ScoreUnitPosX, (int)ypos + ScoreUnitPosY,
		ScoreUnitCharWidth, ScoreUnitCharHeight, 0xffff0000, &rect, 0, 0, &myMatrix);
}

}
