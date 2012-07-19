#include "catalog.h"

/* �V���O���g�� �C���X�^���X �A�N�Z�T */
yaku::yakuCalculator::YakuCatalog* yaku::yakuCalculator::YakuCatalog::Instantiate() {
	// Singleton instance accessor
	static YakuCatalog instance;
	return &instance;
}

// �ݒ肵�����[���Ɋ�Â��Ė��C���X�^���X������������
void yaku::yakuCalculator::init() {
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.clear(); // ���Z�b�g
	info("���J�^���O�����Z�b�g���܂����B");
	yaku::yakuCalculator::YakuCatalog::catalogInit::yakulst_pinhu();
	info("���J�^���O�̍\�z���������܂����B");
}
