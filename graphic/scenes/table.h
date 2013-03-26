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
	LPDIRECT3DTEXTURE9 tBorder; // ��̘g
	LPDIRECT3DTEXTURE9 tBaize; // ���ђn
	logwnd::LogWindow* logWindow; // ���O�E�B���h�E
	TextRenderer* myTextRenderer; // �����\��
protected:
	TableSubscene* mySubScene; // �T�u�V�[���I�u�W�F�N�g
	MHJMutex subSceneCS; // �T�u�V�[���؂�ւ��p�N���e�B�J���Z�N�V����
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
	class RichibouReconst;
	RichibouReconst* richibouReconst;
protected: /**** �T�C�R�� ****/
	class DiceReconst;
	DiceReconst* diceReconst;
protected: /**** �N�ƃ}�[�N�E���L�g���}�[�N��u������ ****/
	class TrayReconst;
	TrayReconst* trayReconst;
protected: /**** �����k�E�Ԕv ****/
	class GariReconst;
	GariReconst* gariReconst;
protected: /**** ���I��p�{�^�� ****/
	class ButtonReconst;
	ButtonReconst* buttonReconst;
protected: /**** ���\�� ****/
	void ShowStatus(const GameTable* gameStat); // �����_�_�Ȃǂ̏���\��
protected:
	void ReconstructPlayer(const GameTable* gameStat, PlayerID targetPlayer, PlayerID trueTargetPlayer); // �v���C���[�֌W�̉�ʂ̍č\�z
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
	void checkTimeout();
	int tileSelectMode;
	void FinishTileChoice();
	void CallTsumoAgari();
	void CallKyuushuKyuuhai();
};

}
