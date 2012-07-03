#include "dllmain.h"

HINSTANCE dllInst;
ErrorInfo errorInfo;
const ULONG_PTR errorInfoPtr[1] = {(ULONG_PTR)(&errorInfo)};

void translateException(unsigned int code, _EXCEPTION_POINTERS* ep) {
	std::ostringstream lmsg;
	lmsg << "構造化例外 " <<
	std::hex << std::setw(8) << std::setfill('0') << ep->ExceptionRecord->ExceptionCode <<
	" をC++例外に変換します。";
	info(lmsg.str().c_str());

	CONTEXT context; memcpy(&context, ep->ContextRecord, sizeof(CONTEXT));
	traceLog(ep->ContextRecord, NULL, 0);
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
	std::ostringstream lmsg;
	if (addrList != NULL) memset(addrList, 0, addrListSize);

	HANDLE hProcess = GetCurrentProcess();
	DWORD disp;
	HANDLE hThread = GetCurrentThread();

	PIMAGEHLP_SYMBOL pSymbol = (PIMAGEHLP_SYMBOL)GlobalAlloc(GMEM_FIXED, 16384);
	pSymbol->SizeOfStruct = 16384; pSymbol->MaxNameLength = 16384 - sizeof(IMAGEHLP_SYMBOL);
	SymInitialize(GetCurrentProcess(), NULL, TRUE);

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
			&stackFrame, &context, NULL, SymFunctionTableAccess, SymGetModuleBase, NULL)) break;
		if (stackFrame.AddrPC.Offset == 0) break;

		if (addrList != NULL) {
			addrList[i] = stackFrame.AddrPC.Offset;
			if (i >= (addrListSize / sizeof(int))) break;
		} else {
			if (SymGetSymFromAddr(hProcess, stackFrame.AddrPC.Offset, &disp, pSymbol))
				lmsg << std::hex << std::setw(8) << std::setfill('0') << stackFrame.AddrPC.Offset <<
				" " << pSymbol->Name << "() + " <<
				std::setfill('0') << disp;
			else lmsg << std::hex << std::setw(8) << std::setfill('0') << stackFrame.AddrPC.Offset <<
				" Unknown";
			debug(lmsg.str().c_str()); lmsg.str("");
		}
	}
	
	SymCleanup(hProcess);
	GlobalFree(pSymbol);
#endif
#endif
}

LONG CALLBACK MJCore_Exception_Filter(_EXCEPTION_POINTERS *ex) {
	std::ostringstream dmsg, lmsg;
	PIMAGEHLP_SYMBOL pSymbol; DWORD disp;
	ErrorInfo *errinf = NULL;

	lmsg << "ハンドルされていない例外 " <<
	std::hex << std::setw(8) << std::setfill('0') << ex->ExceptionRecord->ExceptionCode <<
	" が発生したため、強制終了されます。";
	fatal(lmsg.str().c_str()); dmsg << lmsg.str() << std::endl; lmsg.str("");

	CONTEXT context; memcpy(&context, ex->ContextRecord, sizeof(CONTEXT));
	switch (ex->ExceptionRecord->ExceptionCode) {
	case EXCEPTION_MJCORE_SUBSCRIPT_OUT_OF_RANGE:
	case EXCEPTION_MJCORE_INVALID_ARGUMENT:
	case EXCEPTION_MJCORE_INVALID_DATA:
	case EXCEPTION_MJCORE_OVERFLOW:
	case EXCEPTION_MJCORE_DECOMPRESSION_FAILURE:
	case EXCEPTION_MJCORE_HASH_MISMATCH:
		errinf = (ErrorInfo *)(ex->ExceptionRecord->ExceptionInformation[0]);
		lmsg << ">>> " << errinf->msg;
		fatal(lmsg.str().c_str()); dmsg << lmsg.str() << std::endl; lmsg.str("");
#ifdef _DEBUG
		lmsg << ">>> ファイル: " << errinf->file <<
		" 行: " << errinf->line <<
		" 関数名: " << errinf->func;
		fatal(lmsg.str().c_str()); dmsg << lmsg.str() << std::endl; lmsg.str("");

		pSymbol = (PIMAGEHLP_SYMBOL)GlobalAlloc(GMEM_FIXED, 16384);
		pSymbol->SizeOfStruct = 16384; pSymbol->MaxNameLength = 16384 - sizeof(IMAGEHLP_SYMBOL);
		SymInitialize(GetCurrentProcess(), NULL, TRUE);
		for (unsigned int i = 0; true; i++) {
			if (i >= ADDRBUF) break;
			if (errinf->traceBack[i] == 0) break;
			if (SymGetSymFromAddr(GetCurrentProcess(), errinf->traceBack[i], &disp, pSymbol))
				lmsg << std::hex << std::setw(8) << std::setfill('0') << errinf->traceBack[i] <<
				" " << pSymbol->Name << "() + " <<
				std::setfill('0') << disp;
			else lmsg << std::hex << std::setw(8) << std::setfill('0') << errinf->traceBack[i] <<
				" Unknown";
			debug(lmsg.str().c_str()); lmsg.str("");
		}
		SymCleanup(GetCurrentProcess());
		GlobalFree(pSymbol);
#endif
		break;
	default:
		traceLog(ex->ContextRecord, NULL, 0);
	}

	terminate();
}

#if 0
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
#endif

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved) {
	switch (fdwReason) {
	case DLL_PROCESS_ATTACH:
		dllInst = hinstDLL;
		SetUnhandledExceptionFilter((LPTOP_LEVEL_EXCEPTION_FILTER)MJCore_Exception_Filter);
		//set_terminate(MJCore_Terminate_Handler);
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
