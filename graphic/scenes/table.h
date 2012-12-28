#pragma once

#include "game.h"
#include "../geometry.h"
#include <tuple>
#include <functional>
#include "../logwnd.h"

namespace mihajong_graphic {

class GameTableScreen : public TableProtoScene {
protected:
	static const unsigned int TableSize = Geometry::BaseSize;
	static const unsigned int TileThickness = ShowTile::HoriTileHeight - ShowTile::VertTileWidth;
	LPDIRECT3DTEXTURE9 tBorder; LPD3DXSPRITE sBorder; // 卓の枠
	LPDIRECT3DTEXTURE9 tBaize; LPD3DXSPRITE sBaize; // 羅紗地
	logwnd::LogWindow* logWindow; // ログウィンドウ
protected: /**** 山牌 ****/
	static const unsigned int DeckChainLength = 17;
	static const unsigned int DeckPosH = (TableSize - ShowTile::VertTileWidth * (DeckChainLength - 1)) / 2;
	static const unsigned int DeckPosV = (TableSize / 2) - 300;
	void ReconstructYamahai(const GameTable* gameStat, PLAYER_ID targetPlayer, PLAYER_ID trueTargetPlayer); // 山牌の再構築
protected: /**** 手牌 ****/
	static const unsigned int HandLength = 13;
	static const unsigned int HandPosH = (TableSize - ShowTile::VertTileWidth * (HandLength - 1)) / 2;
	static const unsigned int HandPosV = DeckPosV - 144;
	void ReconstructTehai(const GameTable* gameStat, PLAYER_ID targetPlayer); // 手牌の再構築
protected: /**** 鳴き牌 ****/
	class NakihaiReconst; // 処理は内部クラスにまとめてある
	friend class GameTableScreen::NakihaiReconst;
	NakihaiReconst* nakihaiReconst;
protected: /**** 捨牌 ****/
	static const unsigned int DiscardLineLength = 6;
	static const unsigned int DiscardPosH = (TableSize - ShowTile::VertTileWidth * (DiscardLineLength - 1)) / 2;
	static const unsigned int DiscardPosV = (TableSize / 2) - 130;
	void ReconstructSutehai_portrait(const GameTable* gameStat, PLAYER_ID targetPlayer,
		unsigned tileID, unsigned& tilePosCol, unsigned& tilePosRow, bool& shiftPos);
	void ReconstructSutehai_rotated(const GameTable* gameStat, PLAYER_ID targetPlayer,
		unsigned tileID, unsigned& tilePosCol, unsigned& tilePosRow, bool& shiftPos);
	void ReconstructSutehai(const GameTable* gameStat, PLAYER_ID targetPlayer); // 捨牌の再構築
protected: /**** リーチ棒 ****/
	static const unsigned int RiichiPosH = TableSize / 2;
	static const unsigned int RiichiPosV = (TableSize / 2) + 86;
	LPDIRECT3DTEXTURE9 tRichi; LPD3DXSPRITE sRichi[PLAYERS];  // リーチ棒
	void ShowRiichibou(const GameTable* gameStat); // リーチ棒の再構築
protected: /**** サイコロ ****/
	static const unsigned int DiceWidth = 24;
	static const unsigned int DiceHeight = 34;
	static const unsigned int DicePadding = 2;
	static const unsigned int DicePosH = TableSize - 230;
	static const unsigned int DicePosV = TableSize - 70;
	static const unsigned int DicePosInterstice = DiceWidth / 3;
	LPDIRECT3DTEXTURE9 tDice; LPD3DXSPRITE sDice[2]; // サイコロ
	void ShowDice(const GameTable* gameStat); // サイコロを表示する
protected: /**** 起家マーク・ヤキトリマークを置く凹み ****/
	static const unsigned int TrayHLeft = 275;
	static const unsigned int TrayHTop = 75;
	static const unsigned int TrayHRight = 373;
	static const unsigned int TrayHBottom = 115;
	static const unsigned int TrayHWidth = TrayHRight - TrayHLeft;
	static const unsigned int TrayHHeight = TrayHBottom - TrayHTop;
	static const unsigned int TrayVLeft = 377;
	static const unsigned int TrayVTop = 1;
	static const unsigned int TrayVRight = 417;
	static const unsigned int TrayVBottom = 99;
	static const unsigned int TrayVWidth = TrayVRight - TrayVLeft;
	static const unsigned int TrayVHeight = TrayVBottom - TrayVTop;
	static const unsigned int TrayPosH = TableSize - 200;
	static const unsigned int TrayPosV = TableSize - 24;
	void ShowTray(); // 起家マークを置く場所表示
	/* テクスチャは起家マークと同一ファイル */ LPD3DXSPRITE sTray[PLAYERS]; // 起家マーク置き場
protected: /**** 起家マーク ****/
	static const unsigned int PlateWidthH = 45;
	static const unsigned int PlateHeightH = 35;
	static const unsigned int PlateWidthV = 32;
	static const unsigned int PlateHeightV = 48;
	static const unsigned int PlatePadding = 1;
	static const unsigned int PlatePosH = TrayPosH + (PlateWidthH / 2) + (PlatePadding * 2);
	static const unsigned int PlatePosV = TrayPosV;
	LPDIRECT3DTEXTURE9 tChiicha; LPD3DXSPRITE sChiicha; // 起家マーク
	void ShowChiicha(const GameTable* gameStat); // 起家マークを表示する
protected: /**** ヤキトリマーク ****/
	static const unsigned int PlateID_Yakitori = 7;
	static const unsigned int YakitoriPosH = TrayPosH - (PlateWidthH / 2) - (PlatePadding * 2);
	static const unsigned int YakitoriPosV = TrayPosV;
	/* テクスチャは起家マークと同一ファイル */ LPD3DXSPRITE sYakitori[PLAYERS]; // 起家マーク
	void ShowYakitori(const GameTable* gameStat); // ヤキトリマークを表示する
protected:
	void ReconstructPlayer(const GameTable* gameStat, PLAYER_ID targetPlayer, PLAYER_ID trueTargetPlayer); // プレイヤー関係の画面の再構築
	void Reconstruct(const GameTable* gameStat); // 画面の再描画
public:
	GameTableScreen(ScreenManipulator* const manipulator);
	virtual ~GameTableScreen();
	void Render();
};

class GameTableScreen::NakihaiReconst {
private:
	static const unsigned int MeldPosH = TableSize - 31;
	static const unsigned int MeldPosV = DeckPosV - 166;
	static const unsigned int MPosVVert = TableSize - MeldPosV;
	static const unsigned int MPosVHorU = TableSize - MeldPosV - ShowTile::VertTileWidth + 6;
	static const unsigned int MPosVHorL = TableSize - MeldPosV + 6;
	static inline const unsigned int MPosHVertR(unsigned p) {return MeldPosH - ShowTile::VertTileWidth * p;}
	static inline const unsigned int MPosHVertL(unsigned p) {return MeldPosH - ShowTile::VertTileWidth * (p - 1) - ShowTile::HoriTileHeight - 5;}
	static inline const unsigned int MPosHHor(unsigned p) {return MeldPosH - ShowTile::VertTileWidth * (p - 1) - ShowTile::HoriTileHeight + 2;}
	std::tuple<std::function<unsigned (unsigned)>, std::function<int (unsigned)>, std::function<int (unsigned)>, TileDirection, TileDirection, TileDirection>
		playerPosition(const GameTable* gameStat, PLAYER_ID targetPlayer, signed PositionOffset, unsigned IDOffset, unsigned meldID,
		unsigned h1, unsigned h2, unsigned h3, unsigned h4, unsigned v1, unsigned v2, unsigned v3, unsigned v4,
		bool r1, bool r2, bool r3, bool r4);
	void NakihaiAnkan(const GameTable* gameStat, PLAYER_ID targetPlayer, signed PositionOffset, unsigned IDOffset, unsigned meldID);
	void NakihaiKamicha(const GameTable* gameStat, PLAYER_ID targetPlayer, signed PositionOffset, unsigned IDOffset, unsigned meldID);
	void NakihaiToimen(const GameTable* gameStat, PLAYER_ID targetPlayer, signed PositionOffset, unsigned IDOffset, unsigned meldID);
	void NakihaiShimocha(const GameTable* gameStat, PLAYER_ID targetPlayer, signed PositionOffset, unsigned IDOffset, unsigned meldID);
	void MinkanKamicha(const GameTable* gameStat, PLAYER_ID targetPlayer, signed PositionOffset, unsigned IDOffset, unsigned meldID);
	void MinkanToimen(const GameTable* gameStat, PLAYER_ID targetPlayer, signed PositionOffset, unsigned IDOffset, unsigned meldID);
	void MinkanShimocha(const GameTable* gameStat, PLAYER_ID targetPlayer, signed PositionOffset, unsigned IDOffset, unsigned meldID);
	void NakihaiSelRoutine(const GameTable* gameStat, PLAYER_ID targetPlayer, signed PositionOffset, unsigned IDOffset, unsigned meldID);
public:
	GameTableScreen* caller;
	void ReconstructNakihai(const GameTable* gameStat, PLAYER_ID targetPlayer); // 鳴いた牌の再構築
	NakihaiReconst(GameTableScreen* parent) {caller = parent;}
};

}
