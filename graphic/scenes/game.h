#pragma once

#include "proto.h"
#include "../showtile.h"
#include <array>
#include <tuple>

namespace mihajong_graphic {

class TableProtoScene : public Scene {
private:
	class ScoreBoard; // 持ち点表示用のクラス
	std::array<ScoreBoard*, Players> scorePanel;
	static const unsigned int panelPosY = 550;
protected:
	LPDIRECT3DTEXTURE9 tSideBar; // サイドバー
	void LoadTexture(LPDIRECT3DTEXTURE9* texture, LPCTSTR resource, unsigned width, unsigned height); // テクスチャ読み込み
	void ShowSidebar();
	void InitScorePanel();
	void ShowScorePanel();
public:
	TableProtoScene(ScreenManipulator* const manipulator);
	virtual ~TableProtoScene();
};

class TableProtoScene::ScoreBoard {
private:
	unsigned long long currTime();
private:
	LPDIRECT3DDEVICE9 myDevice;
	seatRelative relativePlayerID;
	int xpos, ypos; float wScale;
	LPDIRECT3DTEXTURE9 texture;
	D3DXMATRIX myMatrix;
	static const unsigned int PanelWidth = 175, PanelHeight = 120;
	PlayerID playerID();
private:
	static const unsigned int WindPosX = 9, WindPosY = 30;
	static const unsigned int WindCharX = 180, WindCharY = 40;
	static const unsigned int WindCharWidth = 40, WindCharHeight = 40;
	void renderWind();
private:
	static const unsigned int NumCharX = 180, NumCharY = 0;
	static const unsigned int NumCharWidth = 30, NumCharHeight = 40;
	void renderNumeral(int x, int y, unsigned num);
	static const unsigned int digitDecimal = 10, digitPlus = 11, digitMinus = 12;
private:
	static const unsigned int RankPosX = 100, RankPosY = 30;
	void renderRank();
private:
	static const unsigned int ScorePosX = 38, ScorePosY = 74;
	int getScoreSign();
	std::tuple<unsigned, unsigned, signed> scoreInfo();
	void renderScore();
private:
	static const unsigned int ScoreUnitCharX = 180, ScoreUnitCharY = 80;
	static const unsigned int ScoreUnitCharWidth = 40, ScoreUnitCharHeight = 40;
	static const unsigned int ScoreUnitPosX = ScorePosX + NumCharWidth * 3, ScoreUnitPosY = ScorePosY;
	void renderScoreUnit(unsigned unitnum);
public:
	ScoreBoard(LPDIRECT3DDEVICE9 device, seatRelative relativePos, int x, int y, float widthScale);
	~ScoreBoard();
	void Render();
};

}
