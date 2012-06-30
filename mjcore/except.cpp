#include "except.h"

/* decompress_failure */
decompress_failure::decompress_failure(const std::string& _Message)
	: std::runtime_error(_Message)
{ // ���k�f�[�^�̐L�����s���ɃX���[�����O
	this->errc = 0;
}
decompress_failure::decompress_failure(const char* _Message)
	: std::runtime_error(_Message)
{ // ���k�f�[�^�̐L�����s���ɃX���[�����O
	this->errc = 0;
}
decompress_failure::decompress_failure(const std::string& _Message, int errcode)
	: std::runtime_error(_Message)
{ // ���k�f�[�^�̐L�����s���ɃX���[�����O
	this->errc = errcode;
}
decompress_failure::decompress_failure(const char* _Message, int errcode)
	: std::runtime_error(_Message)
{ // ���k�f�[�^�̐L�����s���ɃX���[�����O
	this->errc = errcode;
}
int decompress_failure::errorCode() { // ���������G���[�R�[�h
	return this->errc;
}

/* hash_mismatch */
hash_mismatch::hash_mismatch(const std::string& _Message)
	: std::runtime_error(_Message)
{ // ���b�Z�[�W�_�C�W�F�X�g�s��v���ɃX���[�����O
	this->expected = MDVEC();
	this->actual = MDVEC();
}
hash_mismatch::hash_mismatch(const char* _Message)
	: std::runtime_error(_Message)
{ // ���b�Z�[�W�_�C�W�F�X�g�s��v���ɃX���[�����O
	this->expected = MDVEC();
	this->actual = MDVEC();
}
hash_mismatch::hash_mismatch(const std::string& _Message,
	MDVEC expected, MDVEC actual)
	: std::runtime_error(_Message)
{ // ���b�Z�[�W�_�C�W�F�X�g�s��v���ɃX���[�����O
	this->expected = MDVEC(expected.begin(), expected.end());
	this->actual = MDVEC(actual.begin(), actual.end());
}
hash_mismatch::hash_mismatch(const char* _Message,
	MDVEC expected, MDVEC actual)
	: std::runtime_error(_Message)
{ // ���b�Z�[�W�_�C�W�F�X�g�s��v���ɃX���[�����O
	this->expected = MDVEC(expected.begin(), expected.end());
	this->actual = MDVEC(actual.begin(), actual.end());
}
MDVEC hash_mismatch::expectedHash() { // ���҂����n�b�V���l
	return this->expected;
}
MDVEC hash_mismatch::actualHash() { // ���ۂ̃n�b�V���l
	return this->actual;
}
