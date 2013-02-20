#pragma once

#include "tablesub.h"
#include "../../../common/yakurslt.h"
#include <vector>
#include <utility>
#include "../../../common/strcode.h"

namespace mihajong_graphic {

class TableSubsceneAgariScreenProto : public TableSubscene {
protected:
	static const int BaseX = ((signed)Geometry::BaseSize - 600) / 2;
	static const int BaseY = ((signed)Geometry::BaseSize - 864) / 2;
	static const unsigned yakuWndWidth  = 600u;
	static const unsigned yakuWndHeight = 864u;
	double seconds() {return (double)(currTime() - startTime) / 10000000.0;}
protected:
	LPDIRECT3DDEVICE9 myDevice;
	LPDIRECT3DTEXTURE9 windowTexture;
protected:
	mihajong_structs::YakuResult yakuData;
	std::vector<std::pair<CodeConv::tstring, CodeConv::tstring> > yakuList;
	void parseYakuList();
protected:
	void renderWindow();
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

}
