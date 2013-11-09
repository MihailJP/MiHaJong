#pragma once

#include "tablesub.h"

namespace mihajong_graphic {

class TableSubsceneNormal : public TableSubscene {
public:
	TableSubsceneNormal(DevicePtr device);
	TableSubsceneNormal(const TableSubsceneNormal&) = delete; // Delete unexpected copy constructor
	TableSubsceneNormal& operator= (const TableSubsceneNormal&) = delete; // Delete unexpected assign operator
	~TableSubsceneNormal();
	void Render();
};

}
