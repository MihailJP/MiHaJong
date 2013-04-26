#pragma once

#ifdef _WIN32
#include <windows.h>
#endif /*_WIN32*/
#include "../directx.h"
#include <vector>
#include "../input.h"

namespace mihajong_graphic {

class ScreenManipulator;

/* �V�[�� �I�u�W�F�N�g�̃X�[�p�[�N���X */
class Scene {
protected:
	struct Region {int Left, Top, Right, Bottom;}; // �N���b�N�ʒu����p
	static const Region NullRegion;
	void setRegion(unsigned regionID, int Left, int Top, int Right, int Bottom);
	void setRegion(unsigned regionID, const Region& region);
	int whichRegion(int X, int Y); // �ǂ̗̈�Ƀ}�E�X�����邩���擾
private:
	std::vector<Region> regions; // �N���b�N�ʒu����p
protected:
	ScreenManipulator* caller; // �Ăяo�����ւ̃|�C���^
public:
	virtual void Render() = 0; // �`�揈��
	Scene(ScreenManipulator* const manipulator) {
		caller = manipulator;
	}
	virtual ~Scene() {}
#ifdef _WIN32
	virtual void IMEvent(UINT message, WPARAM wParam, LPARAM lParam) {}
	virtual void KeyboardInput(WPARAM wParam, LPARAM lParam) {}
	virtual void KeyboardInput(LPDIDEVICEOBJECTDATA od) {}
	virtual void MouseInput(LPDIDEVICEOBJECTDATA od, int X, int Y) {}
#else /*_WIN32*/
	/* TODO: Linux�œ����̂��̂��������邱�� */
	virtual void KeyboardInput(const XEvent* od) {}
	virtual void MouseInput(const XEvent* od, int X, int Y) {}
#endif /*_WIN32*/
	virtual void SetSubscene(unsigned int scene_ID) {}
};

}
