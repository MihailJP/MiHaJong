#include "init.h"

using namespace std;

// �ݒ肵�����[���Ɋ�Â��Ė��C���X�^���X������������
__declspec(dllexport) list<Yaku>* initYaku() {
	static list<Yaku> yakuCatalog; // ����������
	/*
	 * �����ɃR���X�g���N�^����ׂ�
	 */
	return &yakuCatalog;
}
