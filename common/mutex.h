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
	bool tryAcquire() {return (bool)TryEnterCriticalSection(&myCS);} // ミューテックスを獲得(ロックしない)
	void release() {LeaveCriticalSection(&myCS);} // ミューテックスを解放

	template <typename T> T syncDo(std::function<T (void)> f) { // 相互排他的に関数オブジェクトを実行
		DoFinally<T> myFunc(
			[f, this]() -> T    {this->acquire(); return f();},
			[   this]() -> void {this->release();} );
		return myFunc();
	}

	template <typename T> bool trySyncDo(T* ans, std::function<T (void)> f) { // 相互排他的に関数オブジェクトを実行(ロックしない)
		if (tryAcquire()) {
			DoFinally<T> myFunc(
				[f, this]() -> T    {return f();},
				[   this]() -> void {this->release();} );
			if (ans) *ans = myFunc();
			return true;
		} else {
			return false;
		}
	}
	template <> bool trySyncDo<void>(void*, std::function<void (void)> f) { // 相互排他的に関数オブジェクトを実行(ロックしない)
		if (tryAcquire()) {
			DoFinally<void> myFunc(
				[f, this]() -> void {return f();},
				[   this]() -> void {this->release();} );
			myFunc();
			return true;
		} else {
			return false;
		}
	}
};
