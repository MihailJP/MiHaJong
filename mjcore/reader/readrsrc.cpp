#include "readrsrc.h"

#include <cassert>
#include "../dllmain.h"
#ifndef _WIN32
#include "../filenum.h"
#include <map>
#include <vector>
#include <fstream>
#include <iostream>
#endif /*_WIN32*/

#ifdef _WIN32
void LoadFileInResource(int name, int type, DWORD& size, const uint8_t*& data) {
	HRSRC rc = ::FindResource(dllInst, MAKEINTRESOURCE(name), MAKEINTRESOURCE(type));
	DWORD err = GetLastError();
	assert(rc != nullptr);
	HGLOBAL rcData = ::LoadResource(dllInst, rc);
	assert(rcData != nullptr);
	size = ::SizeofResource(dllInst, rc);
	assert(size != 0);
	data = static_cast<const uint8_t*>(::LockResource(rcData));
}
#else /*_WIN32*/
namespace {
	std::map<int, std::vector<char> > filedat;
}

void LoadFileInResource(int name, int type, size_t& size, const uint8_t*& data) {
	if (filedat.find(name) != filedat.end()) { // 既に読み込まれていた場合
		size = filedat[name].size();
		data = reinterpret_cast<const uint8_t*>(&filedat[name][0]);
	} else { // まだ読み込まれていない場合
		std::string dataFile(dataFileName(name));
		std::ifstream ifs(dataFile.c_str(), std::ios::in | std::ios::binary);
		ifs.seekg(0, ifs.end);
		size = ifs.tellg();
		ifs.seekg(0, ifs.beg);
		assert(size > 0);
		filedat.insert(std::make_pair(name, std::vector<char>()));
		filedat[name].resize(size, '\0');
		ifs.read(&filedat[name][0], size);
		data = reinterpret_cast<const uint8_t*>(&filedat[name][0]);
	}
}
#endif /*_WIN32*/
