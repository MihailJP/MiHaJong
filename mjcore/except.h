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
	explicit decompress_failure(const std::string& _Message); // 圧縮データの伸長失敗時にスローする例外
	explicit decompress_failure(const char* _Message); // 圧縮データの伸長失敗時にスローする例外
	explicit decompress_failure(const std::string& _Message, int errcode);// 圧縮データの伸長失敗時にスローする例外
	explicit decompress_failure(const char* _Message, int errcode);// 圧縮データの伸長失敗時にスローする例外
	int errorCode(); // 発生したエラーコード
};

template class std::vector<uint8_t>;
typedef std::vector<uint8_t> MDVEC;
class hash_mismatch : public std::runtime_error {
protected:
	MDVEC expected, actual;
public:
	explicit hash_mismatch(const std::string& _Message); // メッセージダイジェスト不一致時にスローする例外
	explicit hash_mismatch(const char* _Message); // メッセージダイジェスト不一致時にスローする例外
	explicit hash_mismatch(const std::string& _Message,
		MDVEC expected, MDVEC actual); // メッセージダイジェスト不一致時にスローする例外
	explicit hash_mismatch(const char* _Message,
		MDVEC expected, MDVEC actual); // メッセージダイジェスト不一致時にスローする例外
	MDVEC expectedHash(); // 期待されるハッシュ値
	MDVEC actualHash(); // 実際のハッシュ値
};

#endif
