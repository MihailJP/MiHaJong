#include "decomp.h"

#include <cassert>
#include "lzma/LzmaLib.h"
#include "lzma/Sha256.h"
#include "logging.h"
#include "reader/readrsrc.h"
#include "../common/strcode.h"
#include "except.h"
#ifndef _MSC_VER
#include <cstdlib>
#endif
#include <algorithm>

using std::min;

/* 面子データ初期化 */
namespace Compressed {

void Data::decompress(int FileID_) {
#ifdef _WIN32
	DWORD size = 0;
#else /*_WIN32*/
	size_t size = 0;
#endif /*_WIN32*/
	const uint8_t* compressedBuf = nullptr;
	int result;
	LoadFileInResource(FileID_, LZMA_STREAM, size, compressedBuf);
	assert(size > 13);
	uint8_t* compressedData = new uint8_t[size+1];
	memcpy(compressedData, compressedBuf, size);
	compressedData[size] = 0;
	decompressedSize = *((size_t *)(compressedData+5));
	DecompressedData = new uint8_t[decompressedSize];
	result = LzmaUncompress(DecompressedData, &decompressedSize,
		(const uint8_t *)(compressedData+13),
		(SizeT *)&size, (const uint8_t *)compressedData, 5);
	delete[] compressedData; compressedData = nullptr;
	if (result != SZ_OK) {
		CodeConv::tostringstream o;
		o << _T("LZMAストリームのデコードに失敗しました。ファイルが壊れている虞があります。") <<
			_T("エラーコード: ") << result;
		Raise(EXCEPTION_MJCORE_DECOMPRESSION_FAILURE, o.str().c_str());
	}
	else {
		info(_T("LZMAストリームをデコードしました。"));
	}
	return;
}

void Data::calcSHA256() {
	CSha256 p;
	Sha256_Init(&p);
	Sha256_Update(&p, DecompressedData, decompressedSize);
	Sha256_Final(&p, actualDigest);
}

std::string Data::bytesToHexString(std::vector<uint8_t> byteStr) {
	std::string hx = std::string();
	for (unsigned int i = 0; i < byteStr.size(); i++) {
		std::ostringstream o;
		o.setf(std::ios::right); o.fill('0'); o.width(2);
		o << std::hex << (int)byteStr[i];
		hx += o.str();
	}
	return hx;
}

void Data::verify(LPCTSTR Description_, const uint8_t* const expectedDigest_) {
	memset(actualDigest, 0, sizeof(actualDigest)); bool mdUnmatch = false;
	calcSHA256();
	for (int i = 0; i < 32; i++) {
		if (expectedDigest_[i] != actualDigest[i]) mdUnmatch = true;
	}
	if (mdUnmatch) {
		CodeConv::tostringstream o;
		o << Description_ << _T("のSHA256ハッシュ値が一致しませんでした。") <<
			_T("ファイルが壊れている虞があります。") << std::endl <<
			_T("期待されるハッシュ値: ") <<
			CodeConv::EnsureTStr(bytesToHexString(std::vector<uint8_t>(expectedDigest_, expectedDigest_ + 32))) << std::endl <<
			_T("実際のハッシュ値: ") <<
			CodeConv::EnsureTStr(bytesToHexString(std::vector<uint8_t>(actualDigest, actualDigest + 32)));
		Raise(EXCEPTION_MJCORE_HASH_MISMATCH, o.str().c_str());
	}
	else {
		CodeConv::tostringstream o;
		o << Description_ << _T("のSHA256ハッシュ値の照合に成功しました。");
		info(o.str().c_str());
	}
}

Data::Data(LPCTSTR Description_, int FileID_, const uint8_t* const expectedDigest_) { // 初期化
	try {
		decompress(FileID_);
		verify(Description_, expectedDigest_);
	}
#ifdef _WIN32
	catch (_EXCEPTION_POINTERS* e) {
		ErrorInfo *errStat = nullptr;
		switch (e->ExceptionRecord->ExceptionCode) {
		case EXCEPTION_MJCORE_DECOMPRESSION_FAILURE:
			errStat = (ErrorInfo *)(e->ExceptionRecord->ExceptionInformation[0]);
			MessageBox(nullptr, CodeConv::EnsureTStr(errStat->msg).c_str(), _T("LZMA decompression error"),
				MB_OK | MB_ICONERROR | MB_SETFOREGROUND);
#ifdef _MSC_VER
			terminate();
#else
			abort();
#endif
		case EXCEPTION_MJCORE_HASH_MISMATCH:
			errStat = (ErrorInfo *)(e->ExceptionRecord->ExceptionInformation[0]);
			MessageBox(nullptr, CodeConv::EnsureTStr(errStat->msg).c_str(), _T("SHA256 verification error"),
				MB_OK | MB_ICONERROR | MB_SETFOREGROUND);
#ifdef _MSC_VER
			terminate();
#else
			abort();
#endif
		default:
			throw;
		}
	}
#else /*_WIN32*/
	catch (...) {
		/* TODO: 未実装箇所 */
		throw;
	}
#endif /*_WIN32*/
}

Data::~Data() {
	delete[] DecompressedData; DecompressedData = nullptr;
}

LPCTSTR Data::Description = nullptr;
constexpr uint8_t Data::expectedDigest[32] = {0,};

LPCTSTR file_mentz_dat::Description = _T("面子構成データベース");
constexpr uint8_t file_mentz_dat::expectedDigest[32] = {
	0x38, 0x27, 0x3a, 0x13, 0x49, 0x94, 0xd3, 0x77,
	0x0e, 0x09, 0x05, 0xd4, 0xf5, 0xf7, 0xbb, 0x30,
	0x81, 0x0a, 0x9f, 0x8d, 0xd4, 0x4d, 0xe8, 0x24,
	0xbb, 0x66, 0x1f, 0x71, 0x40, 0xcd, 0x2c, 0x0b,
};

}
