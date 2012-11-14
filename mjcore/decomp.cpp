#include "decomp.h"

using std::min;

/* 面子データ初期化 */
namespace Compressed {

void Data::decompress() {
	DWORD size = 0;
	const uint8_t* compressedBuf = nullptr;
	int result;
	LoadFileInResource(FileID, LZMA_STREAM, size, compressedBuf);
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
		std::ostringstream o;
		o << "LZMAストリームのデコードに失敗しました。ファイルが壊れている虞があります。" <<
			"エラーコード: " << result;
		Raise(EXCEPTION_MJCORE_DECOMPRESSION_FAILURE, o.str().c_str());
	}
	else {
		info("LZMAストリームをデコードしました。");
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

void Data::verify() {
	memset(actualDigest, 0, sizeof(actualDigest)); bool mdUnmatch = false;
	calcSHA256();
	for (int i = 0; i < 32; i++) {
		if (expectedDigest[i] != actualDigest[i]) mdUnmatch = true;
	}
	if (mdUnmatch) {
		std::ostringstream o;
		o << Description << "のSHA256ハッシュ値が一致しませんでした。" <<
			"ファイルが壊れている虞があります。" << std::endl <<
			"期待されるハッシュ値: " <<
			bytesToHexString(std::vector<uint8_t>(expectedDigest[0], expectedDigest[31])) <<
			"実際のハッシュ値: " <<
			bytesToHexString(std::vector<uint8_t>(actualDigest[0], actualDigest[31]));
		Raise(EXCEPTION_MJCORE_HASH_MISMATCH, o.str().c_str());
	}
	else {
		std::ostringstream o;
		o << Description << "のSHA256ハッシュ値の照合に成功しました。";
		info(o.str().c_str());
	}
}

Data::Data() { // 初期化
	try {
		decompress();
		verify();
	}
	catch (_EXCEPTION_POINTERS* e) {
		ErrorInfo *errStat = nullptr;
		switch (e->ExceptionRecord->ExceptionCode) {
		case EXCEPTION_MJCORE_DECOMPRESSION_FAILURE:
			errStat = (ErrorInfo *)(e->ExceptionRecord->ExceptionInformation[0]);
			MessageBox(nullptr, errStat->msg, (LPCSTR)"LZMA decompression error",
				MB_OK | MB_ICONERROR | MB_SETFOREGROUND);
			terminate();
		case EXCEPTION_MJCORE_HASH_MISMATCH:
			errStat = (ErrorInfo *)(e->ExceptionRecord->ExceptionInformation[0]);
			MessageBox(nullptr, errStat->msg, (LPCSTR)"SHA256 verification error",
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

const char* file_mentz_dat::Description = "面子構成データベース";
const uint8_t file_mentz_dat::expectedDigest[32] = {
	0x2d, 0x10, 0x7e, 0x88, 0x85, 0xad, 0xd7, 0xe0,
	0x1f, 0xec, 0x65, 0xfa, 0x69, 0x06, 0x33, 0x7a,
	0xba, 0xe9, 0xf7, 0x6c, 0xfb, 0x6f, 0xc2, 0xe8,
	0x98, 0xca, 0xfe, 0x17, 0xaa, 0x7b, 0x51, 0xc1,
};

}
