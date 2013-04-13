#pragma once

#include "../../directx.h"
#include <tuple>
#include <functional>
#include "../../showtile.h"

namespace mihajong_graphic {

class ShowNakihai {
private:
	DevicePtr myDevice;
protected:
	ShowTile* TileTexture;
protected:
	virtual unsigned int MeldPosH() = 0;
	virtual unsigned int MeldPosV() = 0;
	virtual unsigned int MPosVVert() = 0;
	virtual unsigned int MPosVHorU() = 0;
	virtual unsigned int MPosVHorL() = 0;
	virtual unsigned int MPosHVertR(unsigned p) {return MeldPosH() - ShowTile::VertTileWidth * p;}
	virtual unsigned int MPosHVertL(unsigned p) {return MeldPosH() - ShowTile::VertTileWidth * (p - 1) - ShowTile::HoriTileHeight - 5;}
	virtual unsigned int MPosHHor(unsigned p) {return MeldPosH() - ShowTile::VertTileWidth * (p - 1) - ShowTile::HoriTileHeight + 2;}
	virtual std::tuple<std::function<unsigned (unsigned)>, std::function<int (unsigned)>, std::function<int (unsigned)>, TileDirection, TileDirection, TileDirection>
		playerPosition(const GameTable* gameStat, PlayerID targetPlayer, signed PositionOffset, unsigned IDOffset, unsigned meldID,
		unsigned h1, unsigned h2, unsigned h3, unsigned h4, unsigned v1, unsigned v2, unsigned v3, unsigned v4,
		bool r1, bool r2, bool r3, bool r4) = 0;
	void NakihaiAnkan(const GameTable* gameStat, PlayerID targetPlayer, signed PositionOffset, unsigned IDOffset, unsigned meldID);
	void NakihaiKamicha(const GameTable* gameStat, PlayerID targetPlayer, signed PositionOffset, unsigned IDOffset, unsigned meldID);
	void NakihaiToimen(const GameTable* gameStat, PlayerID targetPlayer, signed PositionOffset, unsigned IDOffset, unsigned meldID);
	void NakihaiShimocha(const GameTable* gameStat, PlayerID targetPlayer, signed PositionOffset, unsigned IDOffset, unsigned meldID);
	void MinkanKamicha(const GameTable* gameStat, PlayerID targetPlayer, signed PositionOffset, unsigned IDOffset, unsigned meldID);
	void MinkanToimen(const GameTable* gameStat, PlayerID targetPlayer, signed PositionOffset, unsigned IDOffset, unsigned meldID);
	void MinkanShimocha(const GameTable* gameStat, PlayerID targetPlayer, signed PositionOffset, unsigned IDOffset, unsigned meldID);
	void NakihaiSelRoutine(const GameTable* gameStat, PlayerID targetPlayer, signed PositionOffset, unsigned IDOffset, unsigned meldID);
public:
	virtual void Render() = 0;
	explicit ShowNakihai(DevicePtr device);
	virtual ~ShowNakihai();
};

}
