#pragma once

#include "game.h"
#include "../geometry.h"
#include <tuple>
#include <functional>
#include "../widgets/logwnd.h"
#include "tablesub/tblsubs.h"
#include "../text.h"
#include "../../common/mutex.h"

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
	TextRenderer* myTextRenderer; // 文字表示
protected:
	TableSubscene* mySubScene; // サブシーンオブジェクト
	MHJMutex subSceneCS; // サブシーン切り替え用クリティカルセクション
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
	class RichibouReconst;
	RichibouReconst* richibouReconst;
protected: /**** サイコロ ****/
	class DiceReconst;
	DiceReconst* diceReconst;
protected: /**** 起家マーク・ヤキトリマークを置く凹み ****/
	class TrayReconst;
	TrayReconst* trayReconst;
protected: /**** 抜き北・花牌 ****/
	class GariReconst;
	GariReconst* gariReconst;
protected: /**** 鳴き選択用ボタン ****/
	class ButtonReconst;
	ButtonReconst* buttonReconst;
protected: /**** 情報表示 ****/
	void ShowStatus(const GameTable* gameStat); // 供託点棒などの情報を表示
protected:
	void ReconstructPlayer(const GameTable* gameStat, PlayerID targetPlayer, PlayerID trueTargetPlayer); // プレイヤー関係の画面の再構築
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
	void checkTimeout();
	int tileSelectMode;
	void FinishTileChoice();
	void CallTsumoAgari();
	void CallKyuushuKyuuhai();
};

}
