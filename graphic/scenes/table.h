#pragma once

#include "game.h"
#include "../geometry.h"
#include <tuple>
#include <functional>
#include "../logwnd.h"
#include "tablesub/tblsubs.h"

namespace mihajong_graphic {

class GameTableScreen : public TableProtoScene {
protected:
	static const unsigned int TableSize = Geometry::BaseSize;
	static const unsigned int TileThickness = ShowTile::HoriTileHeight - ShowTile::VertTileWidth;
	static const unsigned int DeckChainLength = 17;
	static const unsigned int DeckPosH = (TableSize - ShowTile::VertTileWidth * (DeckChainLength - 1)) / 2;
	static const unsigned int DeckPosV = (TableSize / 2) - 300;
	LPDIRECT3DTEXTURE9 tBorder; // 卓の枠
	LPDIRECT3DTEXTURE9 tBaize; // 羅紗地
	logwnd::LogWindow* logWindow; // ログウィンドウ
protected:
	TableSubscene* mySubScene; // サブシーンオブジェクト
	CRITICAL_SECTION subSceneCS; // サブシーン切り替え用クリティカルセクション
	void SetSubscene(unsigned int scene_ID); // サブシーン切り替え
protected: /**** 山牌 ****/
	class YamahaiReconst;
	YamahaiReconst* yamahaiReconst;
protected: /**** 手牌 ****/
	class TehaiReconst;
	TehaiReconst* tehaiReconst;
protected: /**** 鳴き牌 ****/
	class NakihaiReconst; // 処理は内部クラスにまとめてある
	NakihaiReconst* nakihaiReconst;
protected: /**** 捨牌 ****/
	class SutehaiReconst;
	SutehaiReconst* sutehaiReconst;
protected: /**** リーチ棒 ****/
	static const unsigned int RiichiPosH = TableSize / 2;
	static const unsigned int RiichiPosV = (TableSize / 2) + 86;
	LPDIRECT3DTEXTURE9 tRichi;  // リーチ棒
	void ShowRiichibou(const GameTable* gameStat); // リーチ棒の再構築
protected: /**** サイコロ ****/
	static const unsigned int DiceWidth = 24;
	static const unsigned int DiceHeight = 34;
	static const unsigned int DicePadding = 2;
	static const unsigned int DicePosH = TableSize - 230;
	static const unsigned int DicePosV = TableSize - 70;
	static const unsigned int DicePosInterstice = DiceWidth / 3;
	LPDIRECT3DTEXTURE9 tDice; // サイコロ
	void ShowDice(const GameTable* gameStat); // サイコロを表示する
protected: /**** 起家マーク・ヤキトリマークを置く凹み ****/
	class TrayReconst;
	TrayReconst* trayReconst;
protected:
	void ReconstructPlayer(const GameTable* gameStat, PLAYER_ID targetPlayer, PLAYER_ID trueTargetPlayer); // プレイヤー関係の画面の再構築
	void Reconstruct(const GameTable* gameStat); // 画面の再描画
protected:
	void cls(); // 画面をクリア
	void RenderTable(); // 卓を表示
	void RenderSideBar(); // サイドバー
public:
	GameTableScreen(ScreenManipulator* const manipulator);
	~GameTableScreen();
	void Render();
	void KeyboardInput(LPDIDEVICEOBJECTDATA od);
	void MouseInput(LPDIDEVICEOBJECTDATA od, int X, int Y);
private:
	void FinishTileChoice();
};

}
