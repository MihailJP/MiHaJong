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
}
}

#endif
