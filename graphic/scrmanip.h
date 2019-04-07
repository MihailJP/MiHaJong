#pragma once

#ifdef _WIN32
#include <tchar.h>
#else /*_WIN32*/
#include "../common/strcode.h"
#include <X11/Xlib.h>
#endif /*_WIN32*/
#include <functional>
#include "scenes/scene_id.h"
#include "scenes/proto.h"
#include "scenes/fps.h"
#include "input.h"
#include "directx.h"

namespace mihajong_graphic {

class ScreenManipulator {
private:
#ifdef _WIN32
	HWND hWnd;
#else /*_WIN32*/
	Window hWnd;
#endif /*_WIN32*/
	RenderingSysPtr pd3d; // Direct3D
	DevicePtr pDevice; // Direct3Dデバイス/OpenGLデバイスコンテキスト
#if defined(_WIN32) && !defined(WITH_DIRECTX)
	HGLRC rContext;
#elif !defined(_WIN32)
	Display* disp;
#endif
	Scene* myScene; // シーン管理用のクラス
	FPSIndicator* myFPSIndicator; // FPS計算・表示
	uint64_t lastRedrawTime;
	bool redrawFlag; // 画面の再描画をするかどうかのフラグ
	void InitDevice(bool fullscreen); // Direct3D オブジェクト初期化
#ifdef _WIN32
	void inputProc(input::InputDevice* inputDev, std::function<void (Scene*, LPDIDEVICEOBJECTDATA)> f);
	// Linuxでは別の箇所で入力イベントを処理するため不要
#endif /*_WIN32*/
	std::recursive_mutex CS_SceneAccess; // シーンアクセスのクリティカルセクション
#ifndef WITH_DIRECTX
#ifdef _WIN32
	HGLRC getContext();
	void discardContext(HGLRC);
#else /* _WIN32 */
	GLXContext getContext(bool);
	void discardContext(GLXContext);
#endif /* _WIN32 */
#endif /* WITH_DIRECTX */
public:
#ifdef _WIN32
	void inputProc(WPARAM wParam, LPARAM lParam);
	void IMEvent(UINT message, WPARAM wParam, LPARAM lParam);
#else /*_WIN32*/
	void kbdInputProc(const XEvent* event);
#endif /*_WIN32*/
	void Render(); // 画面の再描画
#ifdef _WIN32
	ScreenManipulator(HWND windowHandle, bool fullscreen);
#else /*_WIN32*/
	ScreenManipulator(Display* displayPtr, Window windowHandle, bool fullscreen);
#endif /*_WIN32*/
	ScreenManipulator(const ScreenManipulator&) = delete; // Delete unexpected copy constructor
	ScreenManipulator& operator= (const ScreenManipulator&) = delete; // Delete unexpected assign operator
	~ScreenManipulator();
#ifdef _WIN32
	HWND getHWnd() {return hWnd;}
	DevicePtr getDevice() {return pDevice;}
	void inputProc(input::InputManipulator* iManip);
#else /*_WIN32*/
	Window getHWnd() {return hWnd;}
	DevicePtr getDevice() {return nullptr;}
	void mouseInputProc(const XEvent* event);
#endif /*_WIN32*/
#ifndef WITH_DIRECTX
	void preloadTextures();
	void disposeTextures();
#endif /*WITH_DIRECTX*/
	void transit(sceneID scene);
	void subscene(unsigned int subsceneID);
};

}
