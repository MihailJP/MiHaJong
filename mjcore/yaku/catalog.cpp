#include "catalog.h"

// �ݒ肵�����[���Ɋ�Â��Ė��C���X�^���X������������
__declspec(dllexport) void yaku::yakuCalculator::init() {
	YakuCatalog::Instantiate()->catalog.clear(); // ���Z�b�g
	info("���J�^���O�����Z�b�g���܂����B");
	/*
	 * �����ɃR���X�g���N�^����ׂ�
	 */
	YakuCatalog::Instantiate()->catalog.push_back(Yaku( // �e�X�g�p�̃_�~�[�̖�
		"�_�~�[", Yaku::YAKU_HAN::HAN(1, Han), Yaku::YAKU_HAN::HAN(),
		[](const GameTable* const, const MENTSU_ANALYSIS* const) {
			return true;
		}
	));
	info("���J�^���O�̍\�z���������܂����B");
}

