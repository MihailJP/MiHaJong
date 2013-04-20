#include "dllmain.h"

#ifdef _WIN32

#ifdef _MSC_VER
#include <dbghelp.h>
#endif
#include <cstdlib>
#include <sstream>
#include <iomanip>
#include <typeinfo>
#include "logging.h"

HINSTANCE dllInst;
ErrorInfo errorInfo;
const ULONG_PTR errorInfoPtr[1] = {(ULONG_PTR)(&errorInfo)};

#ifdef _MSC_VER
void translateException(unsigned int code, _EXCEPTION_POINTERS* ep) {
	CodeConv::tostringstream lmsg;
	lmsg << _T("構造化例外 ") <<
	std::hex << std::setw(8) << std::setfill(_T('0')) << ep->ExceptionRecord->ExceptionCode <<
	_T(" をC++例外に変換します。");
	info(lmsg.str().c_str());

	CONTEXT context; memcpy(&context, ep->ContextRecord, sizeof(CONTEXT));
	traceLog(ep->ContextRecord, nullptr, 0);
	throw ep;
}

void StackTraceToArray() {
	CONTEXT context; memset(&context, 0, sizeof(context));
	context.ContextFlags = CONTEXT_FULL;
	__asm	call(x);
	__asm x:	pop eax;
	__asm	mov context.Eip, eax;
	__asm	mov context.Ebp, ebp;
	__asm	mov context.Esp, esp;

	traceLog(&context, errorInfo.traceBack, sizeof(errorInfo.traceBack));
}

void traceLog(CONTEXT* ex, int* const addrList, int addrListSize) {
#ifdef _DEBUG
#ifdef _M_IX86
	/* スタックトレース(I386専用です) */
	CodeConv::tostringstream lmsg;
	if (addrList != nullptr) memset(addrList, 0, addrListSize);

	HANDLE hProcess = GetCurrentProcess();
	DWORD disp;
	HANDLE hThread = GetCurrentThread();

	PIMAGEHLP_SYMBOL pSymbol = (PIMAGEHLP_SYMBOL)GlobalAlloc(GMEM_FIXED, 16384);
	pSymbol->SizeOfStruct = 16384; pSymbol->MaxNameLength = 16384 - sizeof(IMAGEHLP_SYMBOL);
	SymInitialize(GetCurrentProcess(), nullptr, TRUE);

	STACKFRAME stackFrame; memset(&stackFrame, 0, sizeof(stackFrame));
	stackFrame.AddrPC.Offset = ex->Eip;
	stackFrame.AddrFrame.Offset = ex->Ebp;
	stackFrame.AddrStack.Offset = ex->Esp;
	stackFrame.AddrPC.Mode = stackFrame.AddrFrame.Mode =
		stackFrame.AddrStack.Mode = AddrModeFlat;

	CONTEXT context;
	memcpy(&context,  ex, sizeof(CONTEXT));
	context.ContextFlags = CONTEXT_FULL;

	for (unsigned int i = 0; true; i++) {
		if (!StackWalk(IMAGE_FILE_MACHINE_I386, hProcess, hThread,
			&stackFrame, &context, nullptr, SymFunctionTableAccess, SymGetModuleBase, nullptr)) break;
		if (stackFrame.AddrPC.Offset == 0) break;

		if (addrList != nullptr) {
			addrList[i] = stackFrame.AddrPC.Offset;
			if (i >= (addrListSize / sizeof(int))) break;
		} else {
			if (SymGetSymFromAddr(hProcess, stackFrame.AddrPC.Offset, &disp, pSymbol))
				lmsg << std::hex << std::setw(8) << std::setfill(_T('0')) << stackFrame.AddrPC.Offset <<
				_T(" ") << pSymbol->Name << _T("() + ") <<
				std::setfill(_T('0')) << disp;
			else lmsg << std::hex << std::setw(8) << std::setfill(_T('0')) << stackFrame.AddrPC.Offset <<
				_T(" Unknown");
			debug(lmsg.str().c_str()); lmsg.str(_T(""));
		}
	}
	
	SymCleanup(hProcess);
	GlobalFree(pSymbol);
#endif
#endif
}
#endif

LONG CALLBACK MJCore_Exception_Filter(_EXCEPTION_POINTERS *ex) {
	CodeConv::tostringstream dmsg, lmsg;
#ifdef _MSC_VER
	PIMAGEHLP_SYMBOL pSymbol;
#endif
	DWORD disp;
	ErrorInfo *errinf = nullptr;

	lmsg << _T("ハンドルされていない例外 ") <<
	std::hex << std::setw(8) << std::setfill(_T('0')) << ex->ExceptionRecord->ExceptionCode <<
	_T(" が発生したため、強制終了されます。");
	fatal(lmsg.str().c_str()); dmsg << lmsg.str() << std::endl; lmsg.str(_T(""));

	CONTEXT context; memcpy(&context, ex->ContextRecord, sizeof(CONTEXT));
	switch (ex->ExceptionRecord->ExceptionCode) {
	case EXCEPTION_MJCORE_SUBSCRIPT_OUT_OF_RANGE:
	case EXCEPTION_MJCORE_INVALID_ARGUMENT:
	case EXCEPTION_MJCORE_INVALID_DATA:
	case EXCEPTION_MJCORE_OVERFLOW:
	case EXCEPTION_MJCORE_DECOMPRESSION_FAILURE:
	case EXCEPTION_MJCORE_HASH_MISMATCH:
		errinf = (ErrorInfo *)(ex->ExceptionRecord->ExceptionInformation[0]);
		lmsg << _T(">>> ") << errinf->msg;
		fatal(lmsg.str().c_str()); dmsg << lmsg.str() << std::endl; lmsg.str(_T(""));
#if defined(_MSC_VER) && defined(_DEBUG)
		lmsg << _T(">>> ファイル: ") << errinf->file <<
		_T(" 行: ") << errinf->line <<
		_T(" 関数名: ") << errinf->func;
		fatal(lmsg.str().c_str()); dmsg << lmsg.str() << std::endl; lmsg.str(_T(""));

		pSymbol = (PIMAGEHLP_SYMBOL)GlobalAlloc(GMEM_FIXED, 16384);
		pSymbol->SizeOfStruct = 16384; pSymbol->MaxNameLength = 16384 - sizeof(IMAGEHLP_SYMBOL);
		SymInitialize(GetCurrentProcess(), nullptr, TRUE);
		for (unsigned int i = 0; true; i++) {
			if (i >= ADDRBUF) break;
			if (errinf->traceBack[i] == 0) break;
			if (SymGetSymFromAddr(GetCurrentProcess(), errinf->traceBack[i], &disp, pSymbol))
				lmsg << std::hex << std::setw(8) << std::setfill(_T('0')) << errinf->traceBack[i] <<
				_T(" ") << pSymbol->Name << _T("() + ") <<
				std::setfill(_T('0')) << disp;
			else lmsg << std::hex << std::setw(8) << std::setfill(_T('0')) << errinf->traceBack[i] <<
				_T(" Unknown");
			debug(lmsg.str().c_str()); lmsg.str(_T(""));
		}
		SymCleanup(GetCurrentProcess());
		GlobalFree(pSymbol);
#endif
		break;
#ifdef _MSC_VER
	default:
		traceLog(ex->ContextRecord, nullptr, 0);
#endif
	}

#ifdef _MSC_VER
	terminate();
#else
	abort();
#endif
}

#if 0
void MJCore_Terminate_Handler() {
	try {throw;}
	catch (std::exception& e) { // 例外クラスのインスタンスだった場合
		const type_info& exceptionType = typeid(e);
		CodeConv::tostringstream dmsg, lmsg;
		dmsg << _T("ハンドルされない例外 ") << exceptionType.name() <<
			_T(" が発生したため強制終了されます。") << std::endl <<
			e.what();
		lmsg << _T("ハンドルされない例外 ") << exceptionType.name() <<
			_T(" が発生したため強制終了されます：") <<
			e.what();
		fatal(lmsg.str());
		/* スタックトレースとかの拡張はまた今度書く */
	}
	catch (...) { // それ以外だった場合
		fatal(_T("ハンドルされない例外が発生したため強制終了されます。追加の情報はありません。"));
	}
	abort();
}
#endif

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved) {
	switch (fdwReason) {
	case DLL_PROCESS_ATTACH:
		dllInst = hinstDLL;
		SetUnhandledExceptionFilter((LPTOP_LEVEL_EXCEPTION_FILTER)MJCore_Exception_Filter);
#ifdef _MSC_VER
		//set_terminate(MJCore_Terminate_Handler);
		_set_se_translator(translateException);
#endif
		break;
	case DLL_PROCESS_DETACH:
		dllInst = nullptr;
		break;
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
		break;
	}

	return TRUE;
}

#else /*_WIN32*/

#include "except.h"

ErrorInfo errorInfo;
const uintptr_t errorInfoPtr[1] = {(uintptr_t)(&errorInfo)};

#endif /*_WIN32*/
