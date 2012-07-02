#ifndef EXCEPT_H
#define EXCEPT_H

#ifdef MJCORE_EXPORTS
#include <cstdint>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <type_traits>
#endif
#include "mjexport.h"

/*
 *  std::exception ��POD����Ȃ��̂�DLL�Ŏg���ɂ͕s�K���ł���B
 *  C++����throw�ŉ������悤���\��Ȃ��̂ŁA�ʂ�POD�ȗ�O�N���X������Ă��܂��܂��傤�B
 */

/*MJCoreException MJCoreException::raise(const char* msg) {
	char tmpmsg[1024]; memset(tmpmsg, 0, 1024); strcpy_s(tmpmsg, 1023, msg);
	MJCoreException ex;
	memset(ex.msg, 0, 1024); strcpy_s(ex.msg, 1023, msg);
	return ex;
}*/

class decompress_failure : public std::runtime_error {
protected:
	int errc;
public:
	explicit decompress_failure(const std::string& _Message); // ���k�f�[�^�̐L�����s���ɃX���[�����O
	explicit decompress_failure(const char* _Message); // ���k�f�[�^�̐L�����s���ɃX���[�����O
	explicit decompress_failure(const std::string& _Message, int errcode);// ���k�f�[�^�̐L�����s���ɃX���[�����O
	explicit decompress_failure(const char* _Message, int errcode);// ���k�f�[�^�̐L�����s���ɃX���[�����O
	int errorCode(); // ���������G���[�R�[�h
};

template class std::vector<uint8_t>;
typedef std::vector<uint8_t> MDVEC;
class hash_mismatch : public std::runtime_error {
protected:
	MDVEC expected, actual;
public:
	explicit hash_mismatch(const std::string& _Message); // ���b�Z�[�W�_�C�W�F�X�g�s��v���ɃX���[�����O
	explicit hash_mismatch(const char* _Message); // ���b�Z�[�W�_�C�W�F�X�g�s��v���ɃX���[�����O
	explicit hash_mismatch(const std::string& _Message,
		MDVEC expected, MDVEC actual); // ���b�Z�[�W�_�C�W�F�X�g�s��v���ɃX���[�����O
	explicit hash_mismatch(const char* _Message,
		MDVEC expected, MDVEC actual); // ���b�Z�[�W�_�C�W�F�X�g�s��v���ɃX���[�����O
	MDVEC expectedHash(); // ���҂����n�b�V���l
	MDVEC actualHash(); // ���ۂ̃n�b�V���l
};

#endif
