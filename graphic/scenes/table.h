#pragma once

#include "game.h"
#include "../geometry.h"
#include <tuple>
#include <functional>
#include "tablesub/tblsubs.h"
#include "../text.h"
#include "../../common/mutex.h"

namespace mihajong_graphic {

class GameTableScreen : public TableProtoScene {
protected:
	static constexpr unsigned int TableSize = Geometry::BaseSize;
	static constexpr unsigned int TileThickness = ShowTile::HoriTileHeight - ShowTile::VertTileWidth;
	static constexpr unsigned int DeckChainLength = 17;
	static constexpr unsigned int DeckPosH = (TableSize - ShowTile::VertTileWidth * (DeckChainLength - 1)) / 2;
	static constexpr unsigned int DeckPosV = (TableSize / 2) - 300;
	TexturePtr tBorder; // 卓の枠
	TexturePtr tBaize; // 羅紗地
	TextRenderer* myTextRenderer; // 文字表示
	Timer myTimer; // デモ画面の文字点滅用
#ifndef _WIN32
	bool reconstructFlag;
#endif /*_WIN32*/
protected:
	TableSubscene* mySubScene; // サブシーンオブジェクト
	MUTEXLIB::recursive_mutex subSceneCS; // サブシーン切り替え用クリティカルセクション
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
protected: /**** 時計 ****/
	class Clock;
	Clock* clockPanel;
protected: /**** 情報表示 ****/
	void ShowStatus(const GameTable* gameStat); // 供託点棒などの情報を表示
protected: /**** 捨牌ツールチップ(というのだろうか) ****/
	class TileTipReconst;
	TileTipReconst* tileTipReconst;
protected:
	void ReconstructPlayer(const GameTable* gameStat, PlayerID deckTargetPlayer, PlayerID targetPlayer); // プレイヤー関係の画面の再構築
	void Reconstruct(const GameTable* gameStat); // 画面の再描画
protected:
	void cls(); // 画面をクリア
	void RenderTable(); // 卓を表示
	void RenderSideBar(); // サイドバー
public:
	GameTableScreen(ScreenManipulator* const manipulator);
	GameTableScreen(const GameTableScreen&) = delete; // Delete unexpected copy constructor
	GameTableScreen& operator= (const GameTableScreen&) = delete; // Delete unexpected assign operator
	~GameTableScreen();
	void Render();
#ifdef _WIN32
	void KeyboardInput(LPDIDEVICEOBJECTDATA od);
	void MouseInput(LPDIDEVICEOBJECTDATA od, int X, int Y);
#else /*_WIN32*/
	void KeyboardInput(const XEvent* od);
	void MouseInput(const XEvent* od, int X, int Y);
#endif /*_WIN32*/
private:
	void checkTimeout();
	int tileSelectMode;
	void FinishTileChoice();
	void CallTsumoAgari();
	void CallKyuushuKyuuhai();
	int countTiles(std::function<bool(TileCode, TileCode)>);
};

}
