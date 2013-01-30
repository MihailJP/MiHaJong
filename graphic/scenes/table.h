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
	LPDIRECT3DTEXTURE9 tBorder; // ��̘g
	LPDIRECT3DTEXTURE9 tBaize; // ���ђn
	logwnd::LogWindow* logWindow; // ���O�E�B���h�E
protected:
	TableSubscene* mySubScene; // �T�u�V�[���I�u�W�F�N�g
	CRITICAL_SECTION subSceneCS; // �T�u�V�[���؂�ւ��p�N���e�B�J���Z�N�V����
	void SetSubscene(unsigned int scene_ID); // �T�u�V�[���؂�ւ�
protected: /**** �R�v ****/
	class YamahaiReconst;
	YamahaiReconst* yamahaiReconst;
protected: /**** ��v ****/
	static const unsigned int HandLength = 13;
	static const unsigned int HandPosH = (TableSize - ShowTile::VertTileWidth * (HandLength - 1)) / 2;
	static const unsigned int HandPosV = DeckPosV - 144;
	static const int tileCursorOff = -3; // ��v�J�[�\��������
	int tileCursor; // ��v�J�[�\��
	void ReconstructTehai(const GameTable* gameStat, PLAYER_ID targetPlayer); // ��v�̍č\�z
protected: /**** ���v ****/
	class NakihaiReconst; // �����͓����N���X�ɂ܂Ƃ߂Ă���
	NakihaiReconst* nakihaiReconst;
protected: /**** �̔v ****/
	static const unsigned int DiscardLineLength = 6;
	static const unsigned int DiscardPosH = (TableSize - ShowTile::VertTileWidth * (DiscardLineLength - 1)) / 2;
	static const unsigned int DiscardPosV = (TableSize / 2) - 130;
	void ReconstructSutehai_portrait(const GameTable* gameStat, PLAYER_ID targetPlayer,
		unsigned tileID, unsigned& tilePosCol, unsigned& tilePosRow, bool& shiftPos);
	void ReconstructSutehai_rotated(const GameTable* gameStat, PLAYER_ID targetPlayer,
		unsigned tileID, unsigned& tilePosCol, unsigned& tilePosRow, bool& shiftPos);
	void ReconstructSutehai(const GameTable* gameStat, PLAYER_ID targetPlayer); // �̔v�̍č\�z
protected: /**** ���[�`�_ ****/
	static const unsigned int RiichiPosH = TableSize / 2;
	static const unsigned int RiichiPosV = (TableSize / 2) + 86;
	LPDIRECT3DTEXTURE9 tRichi;  // ���[�`�_
	void ShowRiichibou(const GameTable* gameStat); // ���[�`�_�̍č\�z
protected: /**** �T�C�R�� ****/
	static const unsigned int DiceWidth = 24;
	static const unsigned int DiceHeight = 34;
	static const unsigned int DicePadding = 2;
	static const unsigned int DicePosH = TableSize - 230;
	static const unsigned int DicePosV = TableSize - 70;
	static const unsigned int DicePosInterstice = DiceWidth / 3;
	LPDIRECT3DTEXTURE9 tDice; // �T�C�R��
	void ShowDice(const GameTable* gameStat); // �T�C�R����\������
protected: /**** �N�ƃ}�[�N�E���L�g���}�[�N��u������ ****/
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
	void ShowTray(); // �N�ƃ}�[�N��u���ꏊ�\��
protected: /**** �N�ƃ}�[�N ****/
	static const unsigned int PlateWidthH = 45;
	static const unsigned int PlateHeightH = 35;
	static const unsigned int PlateWidthV = 32;
	static const unsigned int PlateHeightV = 48;
	static const unsigned int PlatePadding = 1;
	static const unsigned int PlatePosH = TrayPosH + (PlateWidthH / 2) + (PlatePadding * 2);
	static const unsigned int PlatePosV = TrayPosV;
	LPDIRECT3DTEXTURE9 tChiicha; // �N�ƃ}�[�N
	void ShowChiicha(const GameTable* gameStat); // �N�ƃ}�[�N��\������
protected: /**** ���L�g���}�[�N ****/
	static const unsigned int PlateID_Yakitori = 7;
	static const unsigned int YakitoriPosH = TrayPosH - (PlateWidthH / 2) - (PlatePadding * 2);
	static const unsigned int YakitoriPosV = TrayPosV;
	void ShowYakitori(const GameTable* gameStat); // ���L�g���}�[�N��\������
protected:
	void ReconstructPlayer(const GameTable* gameStat, PLAYER_ID targetPlayer, PLAYER_ID trueTargetPlayer); // �v���C���[�֌W�̉�ʂ̍č\�z
	void Reconstruct(const GameTable* gameStat); // ��ʂ̍ĕ`��
protected:
	void cls(); // ��ʂ��N���A
	void RenderTable(); // ���\��
	void RenderSideBar(); // �T�C�h�o�[
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
