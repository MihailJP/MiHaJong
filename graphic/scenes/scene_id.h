#pragma once

namespace mihajong_graphic {

enum sceneID { // �V�[���ԍ�
	sceneSplash, // �X�v���b�V���X�N���[���I�ȃV�[��
	sceneTitle, // �^�C�g�����
	sceneConfig, // �R���t�B�O���
	sceneSetting, // ���ݒ���
	sceneServerWaiting, // �T�[�o�[�ҋ@���
	sceneClientWaiting, // �N���C�A���g�ҋ@���
	sceneGameTable, // ��̉��
	sceneResult, // ���ʉ��
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
	tblSubsceneChonbo, // �u���a�v
	tblSubsceneAlice, // �u�A���X����v
	tblSubsceneCall, // �����\�����
	tblSubsceneCallFade, // �����\�����(�t�F�[�h)
	tblSubsceneCallCut, // �����\�����(�J�b�g)
	tblSubsceneCallChankanPre, // �����\�����(���ȗp����)
	tblSubsceneCallChankan, // �����\�����(���ȗp����)
	tblSubsceneCallVal, // �_���̑����\��
	tblSubsceneCallValNotenBappu, // �_���̑����\���i�m�[�e�������j
	tblSubsceneCallValAgariten, // �_���̑����\���i�a����_�j
	tblSubsceneCallValTsumibou, // �_���̑����\���i�ςݖ_�j
	tblSubsceneCallValChip, // �_���̑����\���i�`�b�v�j
	tblSubsceneCallValKyoutaku, // �_���̑����\���i�����j
	tblSubsceneCallValChonboBappu, // �_���̑����\���i���a�����j
	tblSubsceneCallValNagashiMangan, // �_���̑����\���i�������сj
	tblSubsceneCallValDobon, // �_���̑����\���i��є����j
	tblSubsceneCallValKitamakura, // �_���̑����\���i��є����j
	tblSubsceneCallValYakuman, // �_���̑����\���i�𖞏j�V�j
	tblSubsceneChkTenpai, // ���ǎ��̒��v�m�F
	tblSubscenePlayerDahai, // �v���C���[�̑Ŕv�I��p
	tblSubscenePlayerNaki, // �v���C���[�̕��I�I��p
	tblSubscenePlayerChankan, // �v���C���[�̕��I�I��p(����)
	tblSubsceneAgari, // �a����\�����
	tblSubsceneAgariUradora, // �a����\�����(���h������)
};

enum ServerWaitingSubsceneID : unsigned int { // �T�[�o�[�ҋ@��ʂ̃T�u�V�[���ԍ�
	srvwSubsceneNone, // �ʏ�
	srvwSubscene1of4, // �ҋ@���̐l��
	srvwSubscene1of3, //      �V
	srvwSubscene2of4, //      �V
	srvwSubscene2of3, //      �V
	srvwSubscene3of4, //      �V
	srvwSubscene3of3, //      �V
	srvwSubscene4of4, //      �V
};

}
