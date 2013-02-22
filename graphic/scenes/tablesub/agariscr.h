#pragma once

#include "tablesub.h"
#include "../../../common/yakurslt.h"
#include <vector>
#include <utility>
#include "../../../common/strcode.h"
#include "../../text.h"
#include "../table/tehai0.h"

namespace mihajong_graphic {

// -------------------------------------------------------------------------

class TableSubsceneAgariScreenProto : public TableSubscene {
protected:
	static const int BaseX = ((signed)Geometry::BaseSize - 720) / 2;
	static const int BaseY = ((signed)Geometry::BaseSize - 864) / 2;
	static const unsigned yakuWndWidth  = 720u;
	static const unsigned yakuWndHeight = 864u;
	double seconds() {return (double)(currTime() - startTime) / 10000000.0;}
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
public:
	void Render();
	explicit AgariTehai(TableSubsceneAgariScreenProto* caller);
	~AgariTehai();
};

// -------------------------------------------------------------------------

}
