#pragma once

#include "tablesub.h"
#include "../../text.h"
#include "call.h"

namespace mihajong_graphic {

class TableSubscenePlayerProto : public TableSubscene {
protected:
	static const int timeX = 725;
	static const int timeY = 900;
	static const unsigned timeResolution = 1000000u;
	TextRenderer* myTextRenderer;
	int timeout_val; /* 制限時間をミリ秒で */
	void showTimeout();
public:
	TableSubscenePlayerProto(DevicePtr device);
	TableSubscenePlayerProto(const TableSubscenePlayerProto&) = delete; // Delete unexpected copy constructor
	TableSubscenePlayerProto& operator= (const TableSubscenePlayerProto&) = delete; // Delete unexpected assign operator
	virtual ~TableSubscenePlayerProto();
	std::int32_t timeout(); // タイムアウト判定用（オーバーライド）
};

class TableSubscenePlayerDahai : public TableSubscenePlayerProto {
public:
	TableSubscenePlayerDahai(DevicePtr device);
	TableSubscenePlayerDahai(const TableSubscenePlayerDahai&) = delete; // Delete unexpected copy constructor
	TableSubscenePlayerDahai& operator= (const TableSubscenePlayerDahai&) = delete; // Delete unexpected assign operator
	~TableSubscenePlayerDahai();
	void Render();
};

class TableSubscenePlayerNakiProto : public TableSubscenePlayerProto {
public:
	TableSubscenePlayerNakiProto(DevicePtr device);
	TableSubscenePlayerNakiProto(const TableSubscenePlayerNakiProto&) = delete; // Delete unexpected copy constructor
	TableSubscenePlayerNakiProto& operator= (const TableSubscenePlayerNakiProto&) = delete; // Delete unexpected assign operator
	~TableSubscenePlayerNakiProto();
};

class TableSubscenePlayerNaki : public TableSubscenePlayerNakiProto {
public:
	TableSubscenePlayerNaki(DevicePtr device);
	TableSubscenePlayerNaki(const TableSubscenePlayerNaki&) = delete; // Delete unexpected copy constructor
	TableSubscenePlayerNaki& operator= (const TableSubscenePlayerNaki&) = delete; // Delete unexpected assign operator
	~TableSubscenePlayerNaki();
	void Render();
};

class TableSubscenePlayerNakiChankan : public TableSubscenePlayerNakiProto {
private:
	TableSubsceneCallCut* callScreen;
public:
	TableSubscenePlayerNakiChankan(DevicePtr device);
	TableSubscenePlayerNakiChankan(const TableSubscenePlayerNakiChankan&) = delete; // Delete unexpected copy constructor
	TableSubscenePlayerNakiChankan& operator= (const TableSubscenePlayerNakiChankan&) = delete; // Delete unexpected assign operator
	~TableSubscenePlayerNakiChankan();
	void Render();
};

}
