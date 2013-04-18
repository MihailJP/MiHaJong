#pragma once

#ifdef _WIN32
#include <windows.h>
#endif /*_WIN32*/
#include "directx.h"
#include "../common/strcode.h"
#include <map>

namespace mihajong_graphic {

class SpriteRenderer {
private: /* Singleton class idioms */
	SpriteRenderer(const SpriteRenderer&) {throw _T("Attempt to copy singleton instance");}
	SpriteRenderer& operator=(const SpriteRenderer&) {throw _T("Attempt to copy singleton instance");}
	SpriteRenderer(DevicePtr device);
private:
	SpritePtr sprite;
	static std::map<int, SpriteRenderer*> renderer;
public:
	void ShowSprite(TexturePtr texture, int X, int Y, int Width, int Height,
		ArgbColor color = 0xffffffff, RECT* rect = nullptr, int CenterX = 0, int CenterY = 0, TransformMatrix* matrix = nullptr);
	void Start();
	void End();
	static SpriteRenderer* instantiate(DevicePtr device);
	static void delInstance(DevicePtr device);
	~SpriteRenderer();
};

}
