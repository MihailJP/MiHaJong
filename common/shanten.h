#pragma once

#include <cstdint>

namespace mihajong_structs {

typedef int8_t Shanten; // �������̂��߂̃f�[�^�^�i0=���v�A-1=�a���A127=�����j

static const Shanten ShantenImpossible = SCHAR_MAX;
enum ShantenType : uint8_t { // getShanten�ɓn���X�C�b�`�p
	shantenAll, // ���ׂċ��߂�
	shantenRegular, // ���ʂ̘a���`�̂�
	shantenPairs, // ���Ύq
	shantenOrphans, // ���m���o
	shantenStellar, // ��������
	shantenCivilWar, // ��k�푈
	shantenTohokuGreen, // ���k�V�����O���[����
	shantenSyzygy, // �f������
	shantenQuanbukao, // �S�s��
	shantenSevenup, // �Z�u���A�b�v
	shantenZuhelong, // �g����
	ShantenPages,
};

} /* namespace */
