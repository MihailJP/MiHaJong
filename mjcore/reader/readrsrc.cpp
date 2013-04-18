#include "readrsrc.h"

#include <cassert>
#include "../dllmain.h"

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
void LoadFileInResource(int name, int type, size_t& size, const uint8_t*& data) {
	/* TODO: ñ¢é¿ëïâ”èä */
}
#endif /*_WIN32*/
