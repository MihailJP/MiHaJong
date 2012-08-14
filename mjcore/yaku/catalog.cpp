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
	YakuCatalog::catalogInit::yakulst_contextual();
	YakuCatalog::catalogInit::yakulst_pinhu();
	YakuCatalog::catalogInit::yakulst_suit();
	YakuCatalog::catalogInit::yakulst_yaojiu();
	YakuCatalog::catalogInit::yakulst_triplet_1();
	YakuCatalog::catalogInit::yakulst_triplet_2();
	YakuCatalog::catalogInit::yakulst_quad();
	YakuCatalog::catalogInit::yakulst_sequence();
	YakuCatalog::catalogInit::yakulst_misc();
	YakuCatalog::catalogInit::yakulst_dora();
	info("���J�^���O�̍\�z���������܂����B");
}
