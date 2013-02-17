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

	template <typename T> bool trySyncDo(T* ans, std::function<T (void)> f) { // ���ݔr���I�Ɋ֐��I�u�W�F�N�g�����s(���b�N���Ȃ�)
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
	template <> bool trySyncDo<void>(void*, std::function<void (void)> f) { // ���ݔr���I�Ɋ֐��I�u�W�F�N�g�����s(���b�N���Ȃ�)
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
