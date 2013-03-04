#pragma once

#include "tablesub.h"
#include "../../text.h"
#include "call.h"

namespace mihajong_graphic {

class TableSubscenePlayerProto : public TableSubscene {
protected:
	static const int timeX = 725;
	static const int timeY = 900;
	static const unsigned timeResolution = 10000000u;
	TextRenderer* myTextRenderer;
	int timeout_val; /* �������Ԃ��~���b�� */
	void showTimeout();
public:
	TableSubscenePlayerProto(LPDIRECT3DDEVICE9 device);
	virtual ~TableSubscenePlayerProto();
	std::int32_t timeout(); // �^�C���A�E�g����p�i�I�[�o�[���C�h�j
};

class TableSubscenePlayerDahai : public TableSubscenePlayerProto {
public:
	TableSubscenePlayerDahai(LPDIRECT3DDEVICE9 device);
	~TableSubscenePlayerDahai();
	void Render();
};

class TableSubscenePlayerNakiProto : public TableSubscenePlayerProto {
public:
	TableSubscenePlayerNakiProto(LPDIRECT3DDEVICE9 device);
	~TableSubscenePlayerNakiProto();
};

class TableSubscenePlayerNaki : public TableSubscenePlayerNakiProto {
public:
	TableSubscenePlayerNaki(LPDIRECT3DDEVICE9 device);
	~TableSubscenePlayerNaki();
	void Render();
};

class TableSubscenePlayerNakiChankan : public TableSubscenePlayerNakiProto {
private:
	TableSubsceneCallCut* callScreen;
public:
	TableSubscenePlayerNakiChankan(LPDIRECT3DDEVICE9 device);
	~TableSubscenePlayerNakiChankan();
	void Render();
};

}