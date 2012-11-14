#ifndef DECOMP_H
#define DECOMP_H

#include <cstdint>
#include <vector>
#ifdef MJCORE_EXPORTS
#include "resource.h"
#include "lzma/LzmaLib.h"
#include "lzma/Sha256.h"
#include "logging.h"
#include "reader/readrsrc.h"
#endif

namespace Compressed {

/* Compressed file object */
class Data {
protected:
	static const char* Description;
	static const int FileID = 0;
	static const uint8_t expectedDigest[32];
	uint8_t* DecompressedData;
	size_t decompressedSize;
	uint8_t actualDigest[32];
	void decompress();
	void calcSHA256();
	std::string bytesToHexString(std::vector<uint8_t> byteStr);
	void verify();
public:
	Data(); // Decompress within constructor
	virtual ~Data();
	const uint8_t* getData() {return DecompressedData;}
	size_t getDataSize() {return decompressedSize;}
};
/* Compressed file object for mentz.dat */
class file_mentz_dat : public Data {
private:
	static const int FileID = IDR_LZMA_STREA1;
};

}

#endif
