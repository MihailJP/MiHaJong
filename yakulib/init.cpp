#include "init.h"

// �ݒ肵�����[���Ɋ�Â��Ė��C���X�^���X������������
__declspec(dllexport) std::vector<Yaku>* initYaku() {
	static std::vector<Yaku> yakuCatalog; // ����������
	/*
	 * �����ɃR���X�g���N�^����ׂ�
	 */
	return &yakuCatalog;
}
