#pragma once

#include "tablesub.h"
#include "../../../common/yakurslt.h"
#include <vector>
#include <utility>
#include "../../../common/strcode.h"

namespace mihajong_graphic {

class TableSubsceneAgariScreenProto : public TableSubscene {
protected:
	LPDIRECT3DDEVICE9 myDevice;
	LPDIRECT3DTEXTURE9 windowTexture;
protected:
	mihajong_structs::YakuResult yakuData;
	std::vector<std::pair<CodeConv::tstring, CodeConv::tstring> > yakuList;
	void parseYakuList();
protected:
	void renderWindow();
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
