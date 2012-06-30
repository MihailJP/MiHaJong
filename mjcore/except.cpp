#include "except.h"

/* decompress_failure */
decompress_failure::decompress_failure(const std::string& _Message)
	: std::runtime_error(_Message)
{ // 圧縮データの伸長失敗時にスローする例外
	this->errc = 0;
}
decompress_failure::decompress_failure(const char* _Message)
	: std::runtime_error(_Message)
{ // 圧縮データの伸長失敗時にスローする例外
	this->errc = 0;
}
decompress_failure::decompress_failure(const std::string& _Message, int errcode)
	: std::runtime_error(_Message)
{ // 圧縮データの伸長失敗時にスローする例外
	this->errc = errcode;
}
decompress_failure::decompress_failure(const char* _Message, int errcode)
	: std::runtime_error(_Message)
{ // 圧縮データの伸長失敗時にスローする例外
	this->errc = errcode;
}
int decompress_failure::errorCode() { // 発生したエラーコード
	return this->errc;
}

/* hash_mismatch */
hash_mismatch::hash_mismatch(const std::string& _Message)
	: std::runtime_error(_Message)
{ // メッセージダイジェスト不一致時にスローする例外
	this->expected = MDVEC();
	this->actual = MDVEC();
}
hash_mismatch::hash_mismatch(const char* _Message)
	: std::runtime_error(_Message)
{ // メッセージダイジェスト不一致時にスローする例外
	this->expected = MDVEC();
	this->actual = MDVEC();
}
hash_mismatch::hash_mismatch(const std::string& _Message,
	MDVEC expected, MDVEC actual)
	: std::runtime_error(_Message)
{ // メッセージダイジェスト不一致時にスローする例外
	this->expected = MDVEC(expected.begin(), expected.end());
	this->actual = MDVEC(actual.begin(), actual.end());
}
hash_mismatch::hash_mismatch(const char* _Message,
	MDVEC expected, MDVEC actual)
	: std::runtime_error(_Message)
{ // メッセージダイジェスト不一致時にスローする例外
	this->expected = MDVEC(expected.begin(), expected.end());
	this->actual = MDVEC(actual.begin(), actual.end());
}
MDVEC hash_mismatch::expectedHash() { // 期待されるハッシュ値
	return this->expected;
}
MDVEC hash_mismatch::actualHash() { // 実際のハッシュ値
	return this->actual;
}
