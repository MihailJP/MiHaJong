#include "decomp.h"

using std::min;

/* 面子データ初期化 */
namespace Compressed {

void Data::decompress(int FileID_) {
	DWORD size = 0;
	const uint8_t* compressedBuf = nullptr;
	int result;
	LoadFileInResource(FileID_, LZMA_STREAM, size, compressedBuf);
	assert(size > 13);
	uint8_t* compressedData = (uint8_t *)malloc(size+1);
	memcpy(compressedData, compressedBuf, size);
	compressedData[size] = 0;
	decompressedSize = *((size_t *)(compressedData+5));
	DecompressedData = (uint8_t *)malloc(decompressedSize);
	result = LzmaUncompress(DecompressedData, &decompressedSize,
		(const uint8_t *)(compressedData+13),
		(SizeT *)&size, (const uint8_t *)compressedData, 5);
	free(compressedData); compressedData = nullptr;
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
	std::ostringstream o;
	o.setf(std::ios::right); o.fill('0'); o.width(2);
	for (unsigned int i = 0; i < byteStr.size(); i++) o << byteStr[i];
	return o.str();
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
			CodeConv::EnsureTStr(bytesToHexString(std::vector<uint8_t>(expectedDigest_[0], expectedDigest_[31]))) <<
			_T("実際のハッシュ値: ") <<
			CodeConv::EnsureTStr(bytesToHexString(std::vector<uint8_t>(actualDigest[0], actualDigest[31])));
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
	catch (_EXCEPTION_POINTERS* e) {
		ErrorInfo *errStat = nullptr;
		switch (e->ExceptionRecord->ExceptionCode) {
		case EXCEPTION_MJCORE_DECOMPRESSION_FAILURE:
			errStat = (ErrorInfo *)(e->ExceptionRecord->ExceptionInformation[0]);
			MessageBox(nullptr, CodeConv::EnsureTStr(errStat->msg).c_str(), _T("LZMA decompression error"),
				MB_OK | MB_ICONERROR | MB_SETFOREGROUND);
			terminate();
		case EXCEPTION_MJCORE_HASH_MISMATCH:
			errStat = (ErrorInfo *)(e->ExceptionRecord->ExceptionInformation[0]);
			MessageBox(nullptr, CodeConv::EnsureTStr(errStat->msg).c_str(), _T("SHA256 verification error"),
				MB_OK | MB_ICONERROR | MB_SETFOREGROUND);
			terminate();
		default:
			throw;
		}
	}
}

Data::~Data() {
	free(DecompressedData); DecompressedData = nullptr;
}

LPCTSTR Data::Description = nullptr;
const uint8_t Data::expectedDigest[32] = {0,};

LPCTSTR file_mentz_dat::Description = _T("面子構成データベース");
const uint8_t file_mentz_dat::expectedDigest[32] = {
	0x2d, 0x10, 0x7e, 0x88, 0x85, 0xad, 0xd7, 0xe0,
	0x1f, 0xec, 0x65, 0xfa, 0x69, 0x06, 0x33, 0x7a,
	0xba, 0xe9, 0xf7, 0x6c, 0xfb, 0x6f, 0xc2, 0xe8,
	0x98, 0xca, 0xfe, 0x17, 0xaa, 0x7b, 0x51, 0xc1,
};

}
