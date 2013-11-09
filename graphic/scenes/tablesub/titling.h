#pragma once

#include "tablesub.h"
#include "../../gametbl.h"
#include "../../text.h"

namespace mihajong_graphic {

using std::uint64_t;

class TableSubsceneTitling : public TableSubscene {
protected:
	HugeTextRenderer* myTextRenderer;
	void FadeinStr(const std::wstring& str);
public:
	TableSubsceneTitling(DevicePtr device);
	TableSubsceneTitling(const TableSubsceneTitling&) = delete; // Delete unexpected copy constructor
	TableSubsceneTitling& operator= (const TableSubsceneTitling&) = delete; // Delete unexpected assign operator
	virtual ~TableSubsceneTitling();
	virtual void skipEvent();
};

class TableSubsceneTitlingHonba : public TableSubsceneTitling {
private:
	static const std::wstring Numeral;
public:
	TableSubsceneTitlingHonba(DevicePtr device);
	TableSubsceneTitlingHonba(const TableSubsceneTitlingHonba&) = delete; // Delete unexpected copy constructor
	TableSubsceneTitlingHonba& operator= (const TableSubsceneTitlingHonba&) = delete; // Delete unexpected assign operator
	~TableSubsceneTitlingHonba();
	void Render();
};

class TableSubsceneMsg : public TableSubsceneTitling {
private:
	std::wstring myString;
public:
	TableSubsceneMsg(DevicePtr device, const std::wstring& str = L"");
	TableSubsceneMsg(DevicePtr device, const std::string& str);
	TableSubsceneMsg(const TableSubsceneMsg&) = delete; // Delete unexpected copy constructor
	TableSubsceneMsg& operator= (const TableSubsceneMsg&) = delete; // Delete unexpected assign operator
	~TableSubsceneMsg();
	void Render();
};

}
