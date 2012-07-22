#include "catalog.h"

/* �V���O���g�� �C���X�^���X �A�N�Z�T */
yaku::yakuCalculator::YakuCatalog* yaku::yakuCalculator::YakuCatalog::Instantiate() {
	// Singleton instance accessor
	static YakuCatalog instance;
	return &instance;
}

// �ݒ肵�����[���Ɋ�Â��Ė��C���X�^���X������������
void yaku::yakuCalculator::init() {
	YakuCatalog::Instantiate()->catalog.clear(); // ���Z�b�g
	info("���J�^���O�����Z�b�g���܂����B");
	YakuCatalog::catalogInit::yakulst_pinhu();
	YakuCatalog::catalogInit::yakulst_yaojiu();
	info("���J�^���O�̍\�z���������܂����B");
}
