#pragma once

namespace mihajong_graphic {

enum sceneID { // �V�[���ԍ�
	sceneSplash, // �X�v���b�V���X�N���[���I�ȃV�[��
	sceneTitle, // �^�C�g�����
	sceneConfig, // �R���t�B�O���
	sceneGameTable, // ��̉��
};

enum TableSubsceneID : unsigned int { // ���ʂ̃T�u�V�[���ԍ�
	tblSubsceneNone, // �ʏ�
	tblSubsceneCall, // �����\�����
};

}
