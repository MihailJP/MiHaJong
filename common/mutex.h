#pragma once

#include <Windows.h>
#include <functional>
#include "finally.h"

class MHJMutex { // �~���[�e�b�N�X�̃��b�p�[�I�u�W�F�N�g
private:
	CRITICAL_SECTION myCS;
public:
	MHJMutex() {InitializeCriticalSection(&myCS);}
	~MHJMutex() {DeleteCriticalSection(&myCS);}
	void acquire() {EnterCriticalSection(&myCS);} // �~���[�e�b�N�X���l��
	bool tryAcquire() {return (bool)TryEnterCriticalSection(&myCS);} // �~���[�e�b�N�X���l��(���b�N���Ȃ�)
	void release() {LeaveCriticalSection(&myCS);} // �~���[�e�b�N�X�����

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
