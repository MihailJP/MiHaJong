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
	void release() {LeaveCriticalSection(&myCS);} // �~���[�e�b�N�X�����

	template <typename T> T syncDo(std::function<T (void)> f) { // ���ݔr���I�Ɋ֐��I�u�W�F�N�g�����s
		DoFinally<T> myFunc(
			[f, this]() -> T    {this->acquire(); return f();},
			[   this]() -> void {this->release();} );
		return myFunc();
	}
};
