#include "init.h"

// �ݒ肵�����[���Ɋ�Â��Ė��C���X�^���X������������
__declspec(dllexport) std::list<Yaku>* initYaku() {
	static std::list<Yaku> yakuCatalog; // ����������
	/*
	 * �����ɃR���X�g���N�^����ׂ�
	 */
	return &yakuCatalog;
}
