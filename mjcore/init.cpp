#include "init.h"

// �ݒ肵�����[���Ɋ�Â��Ė��C���X�^���X������������
__declspec(dllexport) std::list<Yaku>* initYaku() {
	static std::list<Yaku> yakuCatalog; // ����������
	/*
	 * �����ɃR���X�g���N�^����ׂ�
	 */
	yakuCatalog.push_back(Yaku( // �e�X�g�p�̃_�~�[�̖�
		"�_�~�[", 1, 0,
		[](){return true;}
	));
	return &yakuCatalog;
}
