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
	{ // 圧縮データの伸長失敗時にスローする例外
		this->errc = 0;
	}
	explicit decompress_failure(const char* _Message)
		: std::runtime_error(_Message)
	{ // 圧縮データの伸長失敗時にスローする例外
		this->errc = 0;
	}
	explicit decompress_failure(const std::string& _Message, int errcode)
		: std::runtime_error(_Message)
	{ // 圧縮データの伸長失敗時にスローする例外
		this->errc = errcode;
	}
	explicit decompress_failure(const char* _Message, int errcode)
		: std::runtime_error(_Message)
	{ // 圧縮データの伸長失敗時にスローする例外
		this->errc = errcode;
	}
	int errorCode() { // 発生したエラーコード
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
	{ // メッセージダイジェスト不一致時にスローする例外
		this->expected = MDVEC();
		this->actual = MDVEC();
	}
	explicit hash_mismatch(const char* _Message)
		: std::runtime_error(_Message)
	{ // メッセージダイジェスト不一致時にスローする例外
		this->expected = MDVEC();
		this->actual = MDVEC();
	}
	explicit hash_mismatch(const std::string& _Message,
		MDVEC expected, MDVEC actual)
		: std::runtime_error(_Message)
	{ // メッセージダイジェスト不一致時にスローする例外
		this->expected = MDVEC(expected.begin(), expected.end());
		this->actual = MDVEC(actual.begin(), actual.end());
	}
	explicit hash_mismatch(const char* _Message,
		MDVEC expected, MDVEC actual)
		: std::runtime_error(_Message)
	{ // メッセージダイジェスト不一致時にスローする例外
		this->expected = MDVEC(expected.begin(), expected.end());
		this->actual = MDVEC(actual.begin(), actual.end());
	}
	MDVEC expectedHash() { // 期待されるハッシュ値
		return this->expected;
	}
	MDVEC actualHash() { // 実際のハッシュ値
		return this->actual;
	}
};

#endif
