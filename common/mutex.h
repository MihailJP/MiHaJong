#pragma once

#include <Windows.h>
#include <functional>
#include "finally.h"

class MHJMutex { // ミューテックスのラッパーオブジェクト
private:
	CRITICAL_SECTION myCS;
public:
	MHJMutex() {InitializeCriticalSection(&myCS);}
	~MHJMutex() {DeleteCriticalSection(&myCS);}
	void acquire() {EnterCriticalSection(&myCS);} // ミューテックスを獲得
	void release() {LeaveCriticalSection(&myCS);} // ミューテックスを解放

	template <typename T> T syncDo(std::function<T (void)> f) { // 相互排他的に関数オブジェクトを実行
		DoFinally<T> myFunc(
			[f, this]() -> T    {this->acquire(); return f();},
			[   this]() -> void {this->release();} );
		return myFunc();
	}
};
