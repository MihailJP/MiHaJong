#pragma once

#ifdef _WIN32
#include <windows.h>
#else /* _WIN32 */
#include <pthread.h>
#endif /* _WIN32 */
#include <functional>
#include "finally.h"

class MHJMutex { // �~���[�e�b�N�X�̃��b�p�[�I�u�W�F�N�g
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
	void acquire() {EnterCriticalSection(&myCS);} // �~���[�e�b�N�X���l��
	bool tryAcquire() {return (bool)TryEnterCriticalSection(&myCS);} // �~���[�e�b�N�X���l��(���b�N���Ȃ�)
	void release() {LeaveCriticalSection(&myCS);} // �~���[�e�b�N�X�����
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
	void acquire() {pthread_mutex_lock(&myCS);} // �~���[�e�b�N�X���l��
	bool tryAcquire() {return pthread_mutex_trylock(&myCS) == 0;} // �~���[�e�b�N�X���l��(���b�N���Ȃ�)
	void release() {pthread_mutex_unlock(&myCS);} // �~���[�e�b�N�X�����
	pthread_mutex_t* getMutex() {return &myCS;} // pthread_mutex_t* �̒��ڎg�p���K�v��API�Ɏg���A�N�Z�T
#endif /* _WIN32 */

	template <typename T> T syncDo(std::function<T (void)> f) { // ���ݔr���I�Ɋ֐��I�u�W�F�N�g�����s
		DoFinally<T> myFunc(
			[f, this]() -> T    {this->acquire(); return f();},
			[   this]() -> void {this->release();} );
		return myFunc();
	}

private:
	template <typename T, typename dummy_type=void> class trySyncDo_obj { // ���ݔr���I�Ɋ֐��I�u�W�F�N�g�����s(���b�N���Ȃ�)
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
	template <typename T2> class trySyncDo_obj<void, T2> { // ���ݔr���I�Ɋ֐��I�u�W�F�N�g�����s(���b�N���Ȃ�)
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
	template <typename T> bool trySyncDo(T* ans, std::function<T (void)> f) { // ���ݔr���I�Ɋ֐��I�u�W�F�N�g�����s(���b�N���Ȃ�)
		trySyncDo_obj<T> trySyncDo_func(this);
		return trySyncDo_func(ans, f);
	}
};
