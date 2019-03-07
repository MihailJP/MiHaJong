#pragma once

#include <cmath>
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
	static constexpr unsigned yakuWndWidth  = 720u;
	static constexpr unsigned yakuWndHeight = 864u;
	static constexpr int BaseX = (static_cast<signed>(Geometry::BaseSize) - static_cast<signed>(yakuWndWidth )) / 2;
	static constexpr int BaseY = (static_cast<signed>(Geometry::BaseSize) - static_cast<signed>(yakuWndHeight)) / 2;
	double seconds() {return static_cast<double>(myTimer.elapsed()) / 1000000.0;}
	static constexpr int handPosY = BaseY + 56;
	static constexpr double yakuInterval = 0.75;
	static constexpr double yakuAnimStartSecond = 2.0;
	enum AgariStyle {agariMine, agariFurikomi, agariOthers,};
	static AgariStyle getAgariStyle();
	static ArgbColor baseColor();
protected:
	DevicePtr myDevice;
	TexturePtr windowTexture;
	TextRenderer* myTextRenderer;
protected:
	mihajong_structs::YakuResult yakuData;
	YakuListType yakuList;

protected:
	void renderWindow();
	class AgariTehai; AgariTehai* agariTehai;
	class AgariNaki; AgariNaki* agariNaki;
	class DoraTiles; class DoraTilesOmote;
	DoraTilesOmote* doraTilesOmote;
	class ShowScore; ShowScore* showScore;
	bool renderYakuName(unsigned yakuNum);
	void renderYakuName();
protected:
	bool bgmFlag;
	int maxShown;
public:
	TableSubsceneAgariScreenProto(DevicePtr device);
	TableSubsceneAgariScreenProto(const TableSubsceneAgariScreenProto&) = delete; // Delete unexpected copy constructor
	TableSubsceneAgariScreenProto& operator= (const TableSubsceneAgariScreenProto&) = delete; // Delete unexpected assign operator
	virtual ~TableSubsceneAgariScreenProto();
	virtual void skipEvent();
};

class TableSubsceneAgariScreen : public TableSubsceneAgariScreenProto {
public:
	TableSubsceneAgariScreen(DevicePtr device);
	TableSubsceneAgariScreen(const TableSubsceneAgariScreen&) = delete; // Delete unexpected copy constructor
	TableSubsceneAgariScreen& operator= (const TableSubsceneAgariScreen&) = delete; // Delete unexpected assign operator
	~TableSubsceneAgariScreen();
	void Render();
};

class TableSubsceneAgariScreenUradora : public TableSubsceneAgariScreenProto {
private:
	class DoraTilesUra;
	DoraTilesUra* doraTilesUra;
public:
	TableSubsceneAgariScreenUradora(DevicePtr device);
	TableSubsceneAgariScreenUradora(const TableSubsceneAgariScreenUradora&) = delete; // Delete unexpected copy constructor
	TableSubsceneAgariScreenUradora& operator= (const TableSubsceneAgariScreenUradora&) = delete; // Delete unexpected assign operator
	~TableSubsceneAgariScreenUradora();
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
	AgariTehai(const AgariTehai&) = delete; // Delete unexpected copy constructor
	AgariTehai& operator= (const AgariTehai&) = delete; // Delete unexpected assign operator
	~AgariTehai();
};

// -------------------------------------------------------------------------

class TableSubsceneAgariScreenProto::AgariNaki : public ShowNakihai {
private:
	TableSubsceneAgariScreenProto* myCaller;
	void Reconstruct(const GameTable* gameStat);
	bool reconstFlag;
private:
	unsigned const int MeldPosH() override {return BaseX + yakuWndWidth - 24;}
	unsigned const int MeldPosV() override {
		const double Zeit = myCaller->seconds();
		const int yOffset = (Zeit >= 1.0) ? 0 : static_cast<int>(pow(1.0 - Zeit, 2) * static_cast<double>(Geometry::BaseSize));
		return handPosY - yOffset;
	}
	unsigned const int MPosVVert() override {return MeldPosV();}
	unsigned const int MPosVHorU() override {return MeldPosV() - ShowTile::VertTileWidth + 6;}
	unsigned const int MPosVHorL() override {return MeldPosV() + 6;}
	std::tuple<std::function<unsigned (unsigned)>, std::function<int (unsigned)>, std::function<int (unsigned)>, TileDirection, TileDirection, TileDirection>
		playerPosition(const GameTable* gameStat, PlayerID targetPlayer, signed PositionOffset, unsigned IDOffset, unsigned meldID,
		unsigned h1, unsigned h2, unsigned h3, unsigned h4, unsigned v1, unsigned v2, unsigned v3, unsigned v4,
		bool r1, bool r2, bool r3, bool r4);
public:
	void Render();
	explicit AgariNaki(TableSubsceneAgariScreenProto* caller);
	AgariNaki(const AgariNaki&) = delete; // Delete unexpected copy constructor
	AgariNaki& operator= (const AgariNaki&) = delete; // Delete unexpected assign operator
	~AgariNaki();
};

// -------------------------------------------------------------------------

class TableSubsceneAgariScreenProto::DoraTiles {
protected:
	TableSubsceneAgariScreenProto* myCaller;
	ShowTile* tileObj;
	virtual const int xPos() = 0;
	virtual const int yPos() = 0;
	virtual const int tileIdOffset() = 0;
	virtual const double startTime() = 0;
	virtual void Reconstruct();
public:
	virtual void Render();
	explicit DoraTiles(TableSubsceneAgariScreenProto* caller);
	DoraTiles(const DoraTiles&) = delete; // Delete unexpected copy constructor
	DoraTiles& operator= (const DoraTiles&) = delete; // Delete unexpected assign operator
	virtual ~DoraTiles() = 0;
};

class TableSubsceneAgariScreenProto::DoraTilesOmote : public TableSubsceneAgariScreenProto::DoraTiles {
protected:
	const int xPos() override {return BaseX + 32;}
	const int yPos() override {return BaseY + 120;}
	const int tileIdOffset() override {return 2;}
	const double startTime() override {return 1.0;}
public:
	explicit DoraTilesOmote(TableSubsceneAgariScreenProto* caller);
	DoraTilesOmote(const DoraTilesOmote&) = delete; // Delete unexpected copy constructor
	DoraTilesOmote& operator= (const DoraTilesOmote&) = delete; // Delete unexpected assign operator
	~DoraTilesOmote();
};

class TableSubsceneAgariScreenUradora::DoraTilesUra : public TableSubsceneAgariScreenProto::DoraTiles {
protected:
	const int xPos() override {return BaseX + 32;}
	const int yPos() override {return BaseY + 120 + ShowTile::HoriTileWidth;}
	const int tileIdOffset() override {return 1;}
	const double startTime() override {return 1.0625;}
public:
	explicit DoraTilesUra(TableSubsceneAgariScreenProto* caller);
	DoraTilesUra(const DoraTilesUra&) = delete; // Delete unexpected copy constructor
	DoraTilesUra& operator= (const DoraTilesUra&) = delete; // Delete unexpected assign operator
	~DoraTilesUra();
};

// -------------------------------------------------------------------------

class TableSubsceneAgariScreenProto::ShowScore {
private:
	TableSubsceneAgariScreenProto* myCaller;
	CallDigitRenderer* digitRenderer;
	TextRenderer* txtRenderer;
	void ReconstructScoreFuHan();
	void ReconstructScoreTxt();
	void ReconstructScoreRank();
	void ReconstructChipAmount();
	void Reconstruct();
private:
	bool soundFlag;
	bool timeFlag;
public:
	void Render();
	explicit ShowScore(TableSubsceneAgariScreenProto* caller);
	ShowScore(const ShowScore&) = delete; // Delete unexpected copy constructor
	ShowScore& operator= (const ShowScore&) = delete; // Delete unexpected assign operator
	~ShowScore();
};

// -------------------------------------------------------------------------

}
