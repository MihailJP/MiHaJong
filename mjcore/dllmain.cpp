#include "dllmain.h"

HINSTANCE dllInst;
ErrorInfo errorInfo;
const ULONG_PTR errorInfoPtr[1] = {(ULONG_PTR)(&errorInfo)};

void translateException(unsigned int code, _EXCEPTION_POINTERS* ep) {
	throw ep;
}

void MJCore_Terminate_Handler() {
	try {throw;}
	catch (std::exception& e) { // 例外クラスのインスタンスだった場合
		const type_info& exceptionType = typeid(e);
		std::ostringstream dmsg, lmsg;
		dmsg << "ハンドルされない例外 " << exceptionType.name() <<
			" が発生したため強制終了されます。" << std::endl <<
			e.what();
		lmsg << "ハンドルされない例外 " << exceptionType.name() <<
			" が発生したため強制終了されます：" <<
			e.what();
		fatal(lmsg.str());
		/* スタックトレースとかの拡張はまた今度書く */
	}
	catch (...) { // それ以外だった場合
		fatal("ハンドルされない例外が発生したため強制終了されます。追加の情報はありません。");
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
