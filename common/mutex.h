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

private:
	template <typename T, typename dummy_type=void> class trySyncDo_obj { // 相互排他的に関数オブジェクトを実行(ロックしない)
	private:
		MHJMutex* caller;
	public:
		trySyncDo_obj(MHJMutex* callerPtr) {caller = callerPtr;}
		bool operator()(T* ans, std::function<T (void)> f) {
			if (caller->tryAcquire()) {
				DoFinally<T> myFunc(
					[f, this]() -> T    {return f();},
					[   this]() -> void {caller->release();} );
				if (ans) *ans = myFunc();
				return true;
			} else {
				return false;
			}
		}
	};
	template <typename T2> class trySyncDo_obj<void, T2> { // 相互排他的に関数オブジェクトを実行(ロックしない)
	private:
		MHJMutex* caller;
	public:
		trySyncDo_obj(MHJMutex* callerPtr) {caller = callerPtr;}
		bool operator()(void*, std::function<void (void)> f) {
			if (caller->tryAcquire()) {
				DoFinally<void> myFunc(
					[f, this]() -> void {return f();},
					[   this]() -> void {caller->release();} );
				myFunc();
				return true;
			} else {
				return false;
			}
		}
	};
public:
	template <typename T> bool trySyncDo(T* ans, std::function<T (void)> f) { // 相互排他的に関数オブジェクトを実行(ロックしない)
		trySyncDo_obj<T> trySyncDo_func(this);
		return trySyncDo_func(ans, f);
	}
};
