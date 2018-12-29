#pragma once

#include <cstdint>
#include <vector>
#include "resource.h"
#include "../common/strcode.h"

namespace Compressed {

/* Compressed file object */
class Data {
protected:
	static LPCTSTR Description;
	static const int FileID = 0;
	static const uint8_t expectedDigest[32];
	uint8_t* DecompressedData;
	size_t decompressedSize;
	uint8_t actualDigest[32];
	void decompress(int FileID_);
	void calcSHA256();
	static std::string bytesToHexString(std::vector<uint8_t> byteStr);
	void verify(LPCTSTR Description_, const uint8_t* const expectedDigest_);
	Data(LPCTSTR Description_, int FileID_, const uint8_t* const expectedDigest_); // Decompress within constructor
public:
	Data(const Data&) = delete; // Delete unexpected copy constructor
	Data& operator= (const Data&) = delete; // Delete unexpected assign operator
	virtual ~Data() = 0;
	constexpr uint8_t* getData() {return DecompressedData;}
	size_t getDataSize() {return decompressedSize;}
};
/* Compressed file object for mentz.dat */
class file_mentz_dat : public Data {
private:
	static LPCTSTR Description;
	static const int FileID = IDR_LZMA_STREA1;
	static const uint8_t expectedDigest[32];
public:
	file_mentz_dat() : Data(Description, FileID, expectedDigest) {}
	file_mentz_dat(const file_mentz_dat&) = delete; // Delete unexpected copy constructor
	file_mentz_dat& operator= (const file_mentz_dat&) = delete; // Delete unexpected assign operator
	~file_mentz_dat() {}
};

}

/* Other compressed resources */
#define FILEID_CONFITEM_CSV IDR_LZMA_STREA2
#define FILEID_CONFITEM_INI IDR_LZMA_STREA3
#define FILEID_RANKVAL_CSV IDR_LZMA_STREA4
#define FILEDESC_CONFITEM_CSV _T("ルール定義ファイル")
#define FILEDESC_CONFITEM_INI _T("ルールタグ翻訳ファイル")
#define FILEDESC_RANKVAL_CSV _T("ウマ定義ファイル")
#include "data/confitem_csv.h"
#include "data/confitem_ini.h"
#include "data/rankval_csv.h"
