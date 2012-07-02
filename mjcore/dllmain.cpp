#include "dllmain.h"

HINSTANCE dllInst;
ErrorInfo errorInfo;
const ULONG_PTR errorInfoPtr[1] = {(ULONG_PTR)(&errorInfo)};

void translateException(unsigned int code, _EXCEPTION_POINTERS* ep) {
	throw ep;
}

void MJCore_Terminate_Handler() {
	try {throw;}
	catch (std::exception& e) { // ��O�N���X�̃C���X�^���X�������ꍇ
		const type_info& exceptionType = typeid(e);
		std::ostringstream dmsg, lmsg;
		dmsg << "�n���h������Ȃ���O " << exceptionType.name() <<
			" �������������ߋ����I������܂��B" << std::endl <<
			e.what();
		lmsg << "�n���h������Ȃ���O " << exceptionType.name() <<
			" �������������ߋ����I������܂��F" <<
			e.what();
		fatal(lmsg.str());
		/* �X�^�b�N�g���[�X�Ƃ��̊g���͂܂����x���� */
	}
	catch (...) { // ����ȊO�������ꍇ
		fatal("�n���h������Ȃ���O�������������ߋ����I������܂��B�ǉ��̏��͂���܂���B");
	}
	abort();
}

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved) {
	switch (fdwReason) {
	case DLL_PROCESS_ATTACH:
		dllInst = hinstDLL;
		set_terminate(MJCore_Terminate_Handler);
		_set_se_translator(translateException);
		break;
	case DLL_PROCESS_DETACH:
		dllInst = NULL;
		break;
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
		break;
	}

	return TRUE;
}
