#pragma once

#include "tablesub.h"
#include "../../text.h"
#include "../../gametbl.h"
#include <array>
#include "../../../common/strcode.h"
#include "titling.h"

namespace mihajong_graphic {

class TableSubsceneCallValue : public TableSubscene {
private:
	TableSubsceneMsg* captionRenderer;
	static const std::array<CodeConv::tstring, 17> digitName;
	CallDigitRenderer* fontRenderer;
	void ShowCall(PlayerID player, int x, int y);
	void ShowAllCall();
public:
	TableSubsceneCallValue(DevicePtr device, const CodeConv::tstring& caption = _T(""));
	TableSubsceneCallValue(const TableSubsceneCallValue&) = delete; // Delete unexpected copy constructor
	TableSubsceneCallValue& operator= (const TableSubsceneCallValue&) = delete; // Delete unexpected assign operator
	~TableSubsceneCallValue();
	void Render();
	virtual void skipEvent();
};

}
