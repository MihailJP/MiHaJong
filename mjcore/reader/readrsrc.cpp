#include "readrsrc.h"

void LoadFileInResource(int name, int type, DWORD& size, const uint8_t*& data) {
	HRSRC rc = ::FindResource(dllInst, MAKEINTRESOURCE(name), MAKEINTRESOURCE(type));
	assert(rc != NULL);
	HGLOBAL rcData = ::LoadResource(dllInst, rc);
	assert(rcData != NULL);
	size = ::SizeofResource(dllInst, rc);
	assert(size != 0);
	data = static_cast<const uint8_t*>(::LockResource(rcData));
}
