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
	tblSubsceneBeginning, // �ǊJ�n���
	tblSubsceneHonba, // �u���{��v
	tblSubsceneRyuukyoku, // �u���ǁv
	tblSubsceneSifeng, // �u�l���A�Łv
	tblSubsceneTripleRon, // �u�O�Ƙa�v
	tblSubsceneSikang, // �u�l�J�ȁv
	tblSubsceneFourRiichi, // �u�l�l�����v
	tblSubsceneCall, // �����\�����
	tblSubsceneCallFade, // �����\�����(�t�F�[�h)
	tblSubsceneChkTenpai, // ���ǎ��̒��v�m�F
};

}
