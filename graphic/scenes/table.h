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
	LPDIRECT3DTEXTURE9 tBorder; LPD3DXSPRITE sBorder; // ��̘g
	LPDIRECT3DTEXTURE9 tBaize; LPD3DXSPRITE sBaize; // ���ђn
	logwnd::LogWindow* logWindow; // ���O�E�B���h�E
protected: /**** �R�v ****/
	static const unsigned int DeckChainLength = 17;
	static const unsigned int DeckPosH = (TableSize - ShowTile::VertTileWidth * (DeckChainLength - 1)) / 2;
	static const unsigned int DeckPosV = (TableSize / 2) - 300;
	void ReconstructYamahai(const GameTable* gameStat, PLAYER_ID targetPlayer, PLAYER_ID trueTargetPlayer); // �R�v�̍č\�z
protected: /**** ��v ****/
	static const unsigned int HandLength = 13;
	static const unsigned int HandPosH = (TableSize - ShowTile::VertTileWidth * (HandLength - 1)) / 2;
	static const unsigned int HandPosV = DeckPosV - 144;
	void ReconstructTehai(const GameTable* gameStat, PLAYER_ID targetPlayer); // ��v�̍č\�z
protected: /**** ���v ****/
	class NakihaiReconst; // �����͓����N���X�ɂ܂Ƃ߂Ă���
	friend class GameTableScreen::NakihaiReconst;
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
	LPDIRECT3DTEXTURE9 tRichi; LPD3DXSPRITE sRichi[PLAYERS];  // ���[�`�_
	void ShowRiichibou(const GameTable* gameStat); // ���[�`�_�̍č\�z
protected: /**** �T�C�R�� ****/
	static const unsigned int DiceWidth = 24;
	static const unsigned int DiceHeight = 34;
	static const unsigned int DicePadding = 2;
	static const unsigned int DicePosH = TableSize - 230;
	static const unsigned int DicePosV = TableSize - 70;
	static const unsigned int DicePosInterstice = DiceWidth / 3;
	LPDIRECT3DTEXTURE9 tDice; LPD3DXSPRITE sDice[2]; // �T�C�R��
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
	/* �e�N�X�`���͋N�ƃ}�[�N�Ɠ���t�@�C�� */ LPD3DXSPRITE sTray[PLAYERS]; // �N�ƃ}�[�N�u����
protected: /**** �N�ƃ}�[�N ****/
	static const unsigned int PlateWidthH = 45;
	static const unsigned int PlateHeightH = 35;
	static const unsigned int PlateWidthV = 32;
	static const unsigned int PlateHeightV = 48;
	static const unsigned int PlatePadding = 1;
	static const unsigned int PlatePosH = TrayPosH + (PlateWidthH / 2) + (PlatePadding * 2);
	static const unsigned int PlatePosV = TrayPosV;
	LPDIRECT3DTEXTURE9 tChiicha; LPD3DXSPRITE sChiicha; // �N�ƃ}�[�N
	void ShowChiicha(const GameTable* gameStat); // �N�ƃ}�[�N��\������
protected: /**** ���L�g���}�[�N ****/
	static const unsigned int PlateID_Yakitori = 7;
	static const unsigned int YakitoriPosH = TrayPosH - (PlateWidthH / 2) - (PlatePadding * 2);
	static const unsigned int YakitoriPosV = TrayPosV;
	/* �e�N�X�`���͋N�ƃ}�[�N�Ɠ���t�@�C�� */ LPD3DXSPRITE sYakitori[PLAYERS]; // �N�ƃ}�[�N
	void ShowYakitori(const GameTable* gameStat); // ���L�g���}�[�N��\������
protected:
	void ReconstructPlayer(const GameTable* gameStat, PLAYER_ID targetPlayer, PLAYER_ID trueTargetPlayer); // �v���C���[�֌W�̉�ʂ̍č\�z
	void Reconstruct(const GameTable* gameStat); // ��ʂ̍ĕ`��
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
	void ReconstructNakihai(const GameTable* gameStat, PLAYER_ID targetPlayer); // �����v�̍č\�z
	NakihaiReconst(GameTableScreen* parent) {caller = parent;}
};

}
