#pragma once

#include <Windows.h>
#include "directx.h"
#include <vector>
#include "../input.h"

namespace mihajong_graphic {

class ScreenManipulator;

/* �V�[�� �I�u�W�F�N�g�̃X�[�p�[�N���X */
class Scene {

protected:
	ScreenManipulator* caller; // �Ăяo�����ւ̃|�C���^
	struct Region {int Left, Top, Right, Bottom;}; // �N���b�N�ʒu����p
	std::vector<Region> regions; // �N���b�N�ʒu����p
	int whichRegion(int X, int Y); // �ǂ̗̈�Ƀ}�E�X�����邩���擾
public:
	virtual void Render() = 0; // �`�揈��
	Scene(ScreenManipulator* const manipulator) {
		caller = manipulator;
	}
	virtual ~Scene() {}
	virtual void IMEvent(UINT message, WPARAM wParam, LPARAM lParam) {}
	virtual void KeyboardInput(WPARAM wParam, LPARAM lParam) {}
	virtual void KeyboardInput(LPDIDEVICEOBJECTDATA od) {}
	virtual void MouseInput(LPDIDEVICEOBJECTDATA od, int X, int Y) {}
	virtual void SetSubscene(unsigned int scene_ID) {}
};

}
