#pragma once

#include "tablesub.h"
#include "../../../common/yakurslt.h"
#include <vector>
#include <utility>
#include "../../../common/strcode.h"
#include "../../text.h"
#include "../table/tehai0.h"
#include "../table/nakihai0.h"

namespace mihajong_graphic {

// -------------------------------------------------------------------------

class TableSubsceneAgariScreenProto : public TableSubscene {
protected:
	static const unsigned yakuWndWidth  = 720u;
	static const unsigned yakuWndHeight = 864u;
	static const int BaseX = ((signed)Geometry::BaseSize - (signed)yakuWndWidth ) / 2;
	static const int BaseY = ((signed)Geometry::BaseSize - (signed)yakuWndHeight) / 2;
	double seconds() {return (double)(currTime() - startTime) / 10000000.0;}
	static const int handPosY = BaseY + 48;
protected:
	LPDIRECT3DDEVICE9 myDevice;
	LPDIRECT3DTEXTURE9 windowTexture;
	TextRenderer* myTextRenderer;
protected:
	mihajong_structs::YakuResult yakuData;
	std::vector<std::pair<CodeConv::tstring, CodeConv::tstring> > yakuList;
	void parseYakuList();
protected:
	void renderWindow();
	class AgariTehai; AgariTehai* agariTehai;
	class AgariNaki; AgariNaki* agariNaki;
	bool renderYakuName(unsigned yakuNum);
	void renderYakuName();
public:
	TableSubsceneAgariScreenProto(LPDIRECT3DDEVICE9 device);
	virtual ~TableSubsceneAgariScreenProto();
};

class TableSubsceneAgariScreen : public TableSubsceneAgariScreenProto {
public:
	TableSubsceneAgariScreen(LPDIRECT3DDEVICE9 device);
	~TableSubsceneAgariScreen();
	void Render();
};

// -------------------------------------------------------------------------

class TableSubsceneAgariScreenProto::AgariTehai : public ShowTehai {
private:
	TableSubsceneAgariScreenProto* myCaller;
	void Reconstruct(const GameTable* gameStat);
	bool reconstFlag;
public:
	void Render();
	explicit AgariTehai(TableSubsceneAgariScreenProto* caller);
	~AgariTehai();
};

// -------------------------------------------------------------------------

class TableSubsceneAgariScreenProto::AgariNaki : public ShowNakihai {
private:
	TableSubsceneAgariScreenProto* myCaller;
	void Reconstruct(const GameTable* gameStat);
	bool reconstFlag;
private:
	unsigned int MeldPosH() {return BaseX + yakuWndWidth - 24;}
	unsigned int MeldPosV() {
		const double Zeit = myCaller->seconds();
		const int yOffset = (Zeit >= 1.0) ? 0 : (int)(pow(1.0 - Zeit, 2) * (double)Geometry::BaseSize);
		return handPosY - yOffset;
	}
	unsigned int MPosVVert() {return MeldPosV();}
	unsigned int MPosVHorU() {return MeldPosV() - ShowTile::VertTileWidth + 6;}
	unsigned int MPosVHorL() {return MeldPosV() + 6;}
	std::tuple<std::function<unsigned (unsigned)>, std::function<int (unsigned)>, std::function<int (unsigned)>, TileDirection, TileDirection, TileDirection>
		playerPosition(const GameTable* gameStat, PlayerID targetPlayer, signed PositionOffset, unsigned IDOffset, unsigned meldID,
		unsigned h1, unsigned h2, unsigned h3, unsigned h4, unsigned v1, unsigned v2, unsigned v3, unsigned v4,
		bool r1, bool r2, bool r3, bool r4);
public:
	void Render();
	explicit AgariNaki(TableSubsceneAgariScreenProto* caller);
	~AgariNaki();
};

// -------------------------------------------------------------------------

}
