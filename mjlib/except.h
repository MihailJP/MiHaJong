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
	explicit decompress_failure(const std::string& _Message)
		: std::runtime_error(_Message)
	{ // ���k�f�[�^�̐L�����s���ɃX���[�����O
		this->errc = 0;
	}
	explicit decompress_failure(const char* _Message)
		: std::runtime_error(_Message)
	{ // ���k�f�[�^�̐L�����s���ɃX���[�����O
		this->errc = 0;
	}
	explicit decompress_failure(const std::string& _Message, int errcode)
		: std::runtime_error(_Message)
	{ // ���k�f�[�^�̐L�����s���ɃX���[�����O
		this->errc = errcode;
	}
	explicit decompress_failure(const char* _Message, int errcode)
		: std::runtime_error(_Message)
	{ // ���k�f�[�^�̐L�����s���ɃX���[�����O
		this->errc = errcode;
	}
	int errorCode() { // ���������G���[�R�[�h
		return this->errc;
	}
};

typedef std::vector<uint8_t> MDVEC;
class hash_mismatch : public std::runtime_error {
protected:
	MDVEC expected, actual;
public:
	explicit hash_mismatch(const std::string& _Message)
		: std::runtime_error(_Message)
	{ // ���b�Z�[�W�_�C�W�F�X�g�s��v���ɃX���[�����O
		this->expected = MDVEC();
		this->actual = MDVEC();
	}
	explicit hash_mismatch(const char* _Message)
		: std::runtime_error(_Message)
	{ // ���b�Z�[�W�_�C�W�F�X�g�s��v���ɃX���[�����O
		this->expected = MDVEC();
		this->actual = MDVEC();
	}
	explicit hash_mismatch(const std::string& _Message,
		MDVEC expected, MDVEC actual)
		: std::runtime_error(_Message)
	{ // ���b�Z�[�W�_�C�W�F�X�g�s��v���ɃX���[�����O
		this->expected = MDVEC(expected.begin(), expected.end());
		this->actual = MDVEC(actual.begin(), actual.end());
	}
	explicit hash_mismatch(const char* _Message,
		MDVEC expected, MDVEC actual)
		: std::runtime_error(_Message)
	{ // ���b�Z�[�W�_�C�W�F�X�g�s��v���ɃX���[�����O
		this->expected = MDVEC(expected.begin(), expected.end());
		this->actual = MDVEC(actual.begin(), actual.end());
	}
	MDVEC expectedHash() { // ���҂����n�b�V���l
		return this->expected;
	}
	MDVEC actualHash() { // ���ۂ̃n�b�V���l
		return this->actual;
	}
};

#endif
