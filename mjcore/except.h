#ifndef EXCEPT_H
#define EXCEPT_H

#include <exception>
#include <stdexcept>
#include <cstdint>
#include <vector>

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
