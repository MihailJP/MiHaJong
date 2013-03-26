#pragma once

#include <d3dx9.h>
#include <dxerr.h>


namespace mihajong_graphic {

class CheckBox {
private:
	LPDIRECT3DTEXTURE9 myTexture;
	LPDIRECT3DDEVICE9 myDevice;
public:
	explicit CheckBox(LPDIRECT3DDEVICE9 device);
	~CheckBox();
};

}
