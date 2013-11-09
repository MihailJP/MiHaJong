#pragma once

#ifdef _WIN32
#include <windows.h>
#else /* _WIN32 */
#include <pthread.h>
#endif /* _WIN32 */
#include <functional>
#include "finally.h"

class MHJMutex { // ミューテックスのラッパーオブジェクト
private:
#ifdef _WIN32
	CRITICAL_SECTION myCS;
#else /* _WIN32 */
	pthread_mutex_t myCS;
	pthread_mutexattr_t attr;
#endif /* _WIN32 */
public:
#ifdef _WIN32
	MHJMutex() {InitializeCriticalSection(&myCS);}
	~MHJMutex() {DeleteCriticalSection(&myCS);}
	void acquire() {EnterCriticalSection(&myCS);} // ミューテックスを獲得
	bool tryAcquire() {return (bool)TryEnterCriticalSection(&myCS);} // ミューテックスを獲得(ロックしない)
	void release() {LeaveCriticalSection(&myCS);} // ミューテックスを解放
#else /* _WIN32 */
	MHJMutex() {
		pthread_mutexattr_init(&attr);
		pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE_NP);
		pthread_mutex_init(&myCS, &attr);
	}
	~MHJMutex() {
		pthread_mutex_destroy(&myCS);
		pthread_mutexattr_destroy(&attr);
	}
	void acquire() {pthread_mutex_lock(&myCS);} // ミューテックスを獲得
	bool tryAcquire() {return pthread_mutex_trylock(&myCS) == 0;} // ミューテックスを獲得(ロックしない)
	void release() {pthread_mutex_unlock(&myCS);} // ミューテックスを解放
	pthread_mutex_t* getMutex() {return &myCS;} // pthread_mutex_t* の直接使用が必要なAPIに使うアクセサ
#endif /* _WIN32 */

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
