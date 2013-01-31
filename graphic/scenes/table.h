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
	class TehaiReconst;
	TehaiReconst* tehaiReconst;
protected: /**** ���v ****/
	class NakihaiReconst; // �����͓����N���X�ɂ܂Ƃ߂Ă���
	NakihaiReconst* nakihaiReconst;
protected: /**** �̔v ****/
	class SutehaiReconst;
	SutehaiReconst* sutehaiReconst;
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
	class TrayReconst;
	TrayReconst* trayReconst;
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
