#pragma once

#ifdef _WIN32
#include <tchar.h>
#else /*_WIN32*/
#include "../common/strcode.h"
#endif /*_WIN32*/
#include <functional>
#include "scenes/scene_id.h"
#include "scenes/proto.h"
#include "scenes/fps.h"
#include "input.h"
#include "../common/mutex.h"
#include "directx.h"

namespace mihajong_graphic {

class ScreenManipulator {
private:
#ifdef _WIN32
	HWND hWnd;
#else /*_WIN32*/
	/* TODO: 未実装 */
#endif /*_WIN32*/
	RenderingSysPtr pd3d; // Direct3D
#ifdef _WIN32
	DevicePtr pDevice; // Direct3Dデバイス/OpenGLデバイスコンテキスト
#if !defined(_WIN32) || !defined(WITH_DIRECTX)
	HGLRC rContext;
#endif
#else /*_WIN32*/
	/* TODO: 未実装 */
#endif /*_WIN32*/
	Scene* myScene; // シーン管理用のクラス
	FPSIndicator* myFPSIndicator; // FPS計算・表示
	uint64_t lastRedrawTime;
	bool redrawFlag; // 画面の再描画をするかどうかのフラグ
	void InitDevice(bool fullscreen); // Direct3D オブジェクト初期化
#ifdef _WIN32
	void inputProc(input::InputDevice* inputDev, std::function<void (Scene*, LPDIDEVICEOBJECTDATA)> f);
#else /*_WIN32*/
	/* TODO: 未実装 */
#endif /*_WIN32*/
	MHJMutex CS_SceneAccess; // シーンアクセスのクリティカルセクション
public:
#ifdef _WIN32
	void inputProc(WPARAM wParam, LPARAM lParam);
	void IMEvent(UINT message, WPARAM wParam, LPARAM lParam);
#else /*_WIN32*/
	/* TODO: 未実装 */
#endif /*_WIN32*/
	void Render(); // 画面の再描画
#ifdef _WIN32
	ScreenManipulator(HWND windowHandle, bool fullscreen);
#else /*_WIN32*/
	/* TODO: 未実装 */
#endif /*_WIN32*/
	~ScreenManipulator();
#ifdef _WIN32
	HWND getHWnd() {return hWnd;}
	DevicePtr getDevice() {return pDevice;}
	void inputProc(input::InputManipulator* iManip);
#else /*_WIN32*/
	/* TODO: 未実装 */
	DevicePtr getDevice() {return nullptr;}
#endif /*_WIN32*/
	void transit(sceneID scene);
	void subscene(unsigned int subsceneID);
};

}
