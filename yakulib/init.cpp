#include "init.h"

// �ݒ肵�����[���Ɋ�Â��Ė��C���X�^���X������������
void createYakuCatalog(std::list<Yaku> yakuCatalog) {
	/*
	 * �����ɃR���X�g���N�^����ׂ�
	 */
	return;
}

__declspec(dllexport) void* initYaku() {
	static std::list<Yaku> yakuCatalog; // ����������
	/*
	 * �����ɃR���X�g���N�^����ׂ�
	 */
	return &yakuCatalog;
}
