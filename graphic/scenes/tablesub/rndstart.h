#pragma once

#include "tablesub.h"
#include "../../gametbl.h"
#include "../../text.h"

namespace mihajong_graphic {

using std::uint64_t;

class TableSubsceneBeginning : public TableSubscene {
protected:
	HugeTextRenderer* myTextRenderer;
	static const std::wstring WindName, Numeral;
	void ZoomChar(unsigned ID, const std::wstring& str, int xOffset, uint64_t Anfang, uint64_t Ende);
public:
	TableSubsceneBeginning(LPDIRECT3DDEVICE9 device);
	~TableSubsceneBeginning();
	void Render();
	virtual void skipEvent();
};

}
