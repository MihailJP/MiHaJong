#pragma once

#include <Windows.h>
#include <d3dx9.h>
#include <dxerr.h>
#include "../input.h"

namespace mihajong_graphic {

class ScreenManipulator;

/* �V�[�� �I�u�W�F�N�g�̃X�[�p�[�N���X */
class Scene {
protected:
	ScreenManipulator* caller; // �Ăяo�����ւ̃|�C���^
public:
	virtual void Render() = 0; // �`�揈��
	Scene(ScreenManipulator* const manipulator) {
		caller = manipulator;
	}
	virtual ~Scene() {}
	virtual void KeyboardInput(LPDIDEVICEOBJECTDATA od) {};
	virtual void MouseInput(LPDIDEVICEOBJECTDATA od, int X, int Y) {};
};

}
