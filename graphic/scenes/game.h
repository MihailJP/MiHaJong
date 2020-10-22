#pragma once

#include "proto.h"
#include "../showtile.h"
#include <array>
#include <tuple>
#include "../text.h"
#include "../timer.h"
#include "../widgets/checkbox.h"

namespace mihajong_graphic {

class TableProtoScene : public Scene {
private:
	class ScoreBoard; // 持ち点表示用のクラス
	std::array<ScoreBoard*, Players> scorePanel;
	static const unsigned int panelPosY = 550;
protected:
	TexturePtr tSideBar; // サイドバー
	void LoadTexture(TexturePtr* texture, LPCTSTR resource); // テクスチャ読み込み
	void ShowSidebar();
	void InitScorePanel();
	void ShowScorePanel();
	ArgbColor roundColor(); // 場ごとの色分け
protected:
	enum CheckBoxNames {ChkBoxAutoAgari, ChkBoxAutoDiscard, ChkBoxAutoPass, NumOfCheckBoxes,};
	static const std::array<CodeConv::tstring, NumOfCheckBoxes> labels;
	std::array<CheckBox*, NumOfCheckBoxes> checkBoxes;
	static const unsigned CheckboxRegionOffset = 50;
public:
	TableProtoScene(ScreenManipulator* const manipulator);
	TableProtoScene(const TableProtoScene&) = delete; // Delete unexpected copy constructor
	TableProtoScene& operator= (const TableProtoScene&) = delete; // Delete unexpected assign operator
	virtual ~TableProtoScene();
#ifdef _WIN32
	virtual void MouseInput(LPDIDEVICEOBJECTDATA od, int X, int Y);
#else /*_WIN32*/
	virtual void MouseInput(const XEvent* od, int X, int Y);
#endif /*_WIN32*/
};

enum class ScoreMode {points, diff, chip};
class TableProtoScene::ScoreBoard {
private:
	Timer myTimer;
private:
	DevicePtr myDevice;
	SeatRelative relativePlayerID;
	int xpos, ypos; float wScale;
	TexturePtr texture;
	static const unsigned int PanelWidth = 175, PanelHeight = 120;
	PlayerID playerID();
private:
	static const ArgbColor ledColorRed = 0xffff0000, // 昔ながらの3色LED風の色
		ledColorOrange = 0xffff9900,
		ledColorGreen = 0xffccff00;
	ScoreMode getScoreMode();
private:
	static const unsigned int WindPosX = 9, WindPosY = 30;
	static const unsigned int WindCharX = 180, WindCharY = 40;
	static const unsigned int WindCharWidth = 40, WindCharHeight = 40;
	void renderWind();
private:
	static const unsigned int NumCharX = 180, NumCharY = 0;
	static const unsigned int NumCharWidth = 30, NumCharHeight = 40;
	void renderNumeral(int x, int y, unsigned num, ArgbColor color);
	static const unsigned int digitDecimal = 10, digitPlus = 11, digitMinus = 12;
private:
	static const unsigned int RankPosX = 100, RankPosY = 30;
	void renderRank();
private:
	static const unsigned int ScorePosX = 38, ScorePosY = 74;
	int getScoreSign();
	std::tuple<unsigned, unsigned, signed, signed> scoreInfo(ScoreMode scoreMode);
	void renderScore();
private:
	static const unsigned int ScoreUnitCharX = 180, ScoreUnitCharY = 80;
	static const unsigned int ScoreUnitCharWidth = 40, ScoreUnitCharHeight = 40;
	static const unsigned int ScoreUnitPosX = ScorePosX + NumCharWidth * 3, ScoreUnitPosY = ScorePosY;
	void renderScoreUnit(unsigned unitnum, ArgbColor color);
private:
	static const unsigned int NamePosX = 5, NamePosY = 5;
	SmallTextRenderer* nameText;
	void renderName();
public:
	ScoreBoard(DevicePtr device, SeatRelative relativePos, int x, int y, float widthScale);
	ScoreBoard(const ScoreBoard&) = delete; // Delete unexpected copy constructor
	ScoreBoard& operator= (const ScoreBoard&) = delete; // Delete unexpected assign operator
	~ScoreBoard();
	void Render();
};

}
