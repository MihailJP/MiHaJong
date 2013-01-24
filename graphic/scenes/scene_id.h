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
	tblSubsceneChonbo, // �u���a�v
	tblSubsceneCall, // �����\�����
	tblSubsceneCallFade, // �����\�����(�t�F�[�h)
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
};

}
