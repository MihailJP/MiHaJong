#pragma once

#include "game.h"
#include "../geometry.h"
#include <tuple>
#include <functional>

namespace mihajong_graphic {

class GameTableScreen : public TableProtoScene {
protected:
	static const unsigned int TableSize = Geometry::BaseSize;
	static const unsigned int TileThickness = ShowTile::HoriTileHeight - ShowTile::VertTileWidth;
	LPDIRECT3DTEXTURE9 tBorder; LPD3DXSPRITE sBorder; // ëÏÇÃòg
	LPDIRECT3DTEXTURE9 tBaize; LPD3DXSPRITE sBaize; // óÖé—ín
protected: /**** éRîv ****/
	static const unsigned int DeckChainLength = 17;
	static const unsigned int DeckPosH = (TableSize - ShowTile::VertTileWidth * (DeckChainLength - 1)) / 2;
	static const unsigned int DeckPosV = (TableSize / 2) - 300;
	void ReconstructYamahai(const GameTable* gameStat, PLAYER_ID targetPlayer, PLAYER_ID trueTargetPlayer); // éRîvÇÃçƒç\íz
protected: /**** éËîv ****/
	static const unsigned int HandLength = 13;
	static const unsigned int HandPosH = (TableSize - ShowTile::VertTileWidth * (HandLength - 1)) / 2;
	static const unsigned int HandPosV = DeckPosV - 144;
	void ReconstructTehai(const GameTable* gameStat, PLAYER_ID targetPlayer); // éËîvÇÃçƒç\íz
protected: /**** ñ¬Ç´îv ****/
	class NakihaiReconst; // èàóùÇÕì‡ïîÉNÉâÉXÇ…Ç‹Ç∆ÇﬂÇƒÇ†ÇÈ
	friend class GameTableScreen::NakihaiReconst;
	NakihaiReconst* nakihaiReconst;
protected: /**** éÃîv ****/
	static const unsigned int DiscardLineLength = 6;
	static const unsigned int DiscardPosH = (TableSize - ShowTile::VertTileWidth * (DiscardLineLength - 1)) / 2;
	static const unsigned int DiscardPosV = (TableSize / 2) - 130;
	void ReconstructSutehai_portrait(const GameTable* gameStat, PLAYER_ID targetPlayer,
		unsigned tileID, unsigned& tilePosCol, unsigned& tilePosRow, bool& shiftPos);
	void ReconstructSutehai_rotated(const GameTable* gameStat, PLAYER_ID targetPlayer,
		unsigned tileID, unsigned& tilePosCol, unsigned& tilePosRow, bool& shiftPos);
	void ReconstructSutehai(const GameTable* gameStat, PLAYER_ID targetPlayer); // éÃîvÇÃçƒç\íz
protected: /**** ÉäÅ[É`ñ_ ****/
	static const unsigned int RiichiPosH = TableSize / 2;
	static const unsigned int RiichiPosV = (TableSize / 2) + 86;
	LPDIRECT3DTEXTURE9 tRichi; LPD3DXSPRITE sRichi[PLAYERS];  // ÉäÅ[É`ñ_
	void ShowRiichibou(const GameTable* gameStat); // ÉäÅ[É`ñ_ÇÃçƒç\íz
protected: /**** ÉTÉCÉRÉç ****/
	static const unsigned int DiceWidth = 24;
	static const unsigned int DiceHeight = 34;
	static const unsigned int DicePadding = 2;
	static const unsigned int DicePosH = TableSize - 230;
	static const unsigned int DicePosV = TableSize - 70;
	static const unsigned int DicePosInterstice = DiceWidth / 3;
	LPDIRECT3DTEXTURE9 tDice; LPD3DXSPRITE sDice[2]; // ÉTÉCÉRÉç
	void ShowDice(const GameTable* gameStat); // ÉTÉCÉRÉçÇï\é¶Ç∑ÇÈ
protected:
	void ReconstructPlayer(const GameTable* gameStat, PLAYER_ID targetPlayer, PLAYER_ID trueTargetPlayer); // ÉvÉåÉCÉÑÅ[ä÷åWÇÃâÊñ ÇÃçƒç\íz
	void Reconstruct(const GameTable* gameStat); // âÊñ ÇÃçƒï`âÊ
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
	void ReconstructNakihai(const GameTable* gameStat, PLAYER_ID targetPlayer); // ñ¬Ç¢ÇΩîvÇÃçƒç\íz
	NakihaiReconst(GameTableScreen* parent) {caller = parent;}
};

}
