#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <cstdint>

namespace mihajong_socket {
namespace protocol {
	// Port 50000 �ɃN���C�A���g�ڑ����A�T�[�o�[�̓N���C�A���g�ԍ�(1 byte)��Ԃ��Đؒf����B
	// Port (50000 + Client�� - 1) �ɍĐڑ����A�N���C�A���g�͎����̖��O�𑗐M����B
	// ���̌�A�N���C�A���g�͊J�n���}��ҋ@����B
	// �����̃|�[�g�ԍ��͎l�l�ł��o�[�W�����̏ꍇ

	// ������c�c B8 LL SS SS SS ...
	//            LL = ������̒���(bytes)
	//            SS = ������̒��g(LL bytes, up to 255)
	const uint8_t StartString_Signature = 0xb8; // ������J�n�̃V�O�l�`��
	
	// �J�n�̃V�O�l�`���c�c B4 NAME1 NAME2 NAME3 NAME4 RULES
	//                      NAME1 to NAME4 = �v���C���[��
	//                      RULES = ���[���ݒ�(80�������ɋ�؂�)
	const uint8_t Server_StartGame_Signature = 0xb4; // �T�[�o�[����v���C�J�n�̃V�O�l�`��

	// �̔v�R�[�h����M�p
	const uint8_t Dahai_Type_Normal_Offset = 0x60; // ���ʂ̎̔v (0x60 .. 0x6d)
	const uint8_t Dahai_Type_Ankan_Offset = 0x70; // �Þ�
	const uint8_t Dahai_Type_Kakan_Offset = 0x80; // ����
	const uint8_t Dahai_Type_Riichi_Offset = 0x90; // ����
	const uint8_t Dahai_Type_ORiichi_Offset = 0xc0; // �I�[�v������
	const uint8_t Dahai_Type_Flower_Offset = 0xd0; // �Ԕv
	const uint8_t Dahai_Kyuushu = 0x6e; // ����v
	const uint8_t Dahai_Tsumo = 0x6f; // �a��
	const uint8_t Dahai_Remote_Disconnect = 0x9f; // �c�����̃v���C���[�̐ڑ����؂ꂽ�ꍇ�̕���

	// ���R�[�h����M�p
	const uint8_t Naki_Ignore = 0xa0; // ����
	const uint8_t Naki_Ron = 0xa1; // ����
	const uint8_t Naki_Pon = 0xa2; // �|��
	const uint8_t Naki_Kan = 0xa3; // ����
	const uint8_t Naki_Chii_Lower = 0xa4; // ���`�[
	const uint8_t Naki_Chii_Middle = 0xa5; // ���`�[
	const uint8_t Naki_Chii_Upper = 0xa6; // �E�`�[
	const uint8_t Naki_Remote_Disconnect = 0xa7; // ���̖₢���킹���Ƀv���C���[�̐ڑ����؂ꂽ�ꍇ�̕���
}
}

#endif
