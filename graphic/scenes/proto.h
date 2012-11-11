#ifndef GRAPHIC_SCENES_PROTO_H
#define GRAPHIC_SCENES_PROTO_H

#include <Windows.h>
#include <d3dx9.h>
#include <dxerr.h>

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
};

#endif
