#include "filesel.h"

#include <sstream>
#ifdef _WIN32
#include <windows.h>
#endif /*_WIN32*/
#include "../func.h"
#include "../random.h"

std::vector<std::string> aiscript::FileSelector::files; // AI�̃X�N���v�g�t�@�C���ꗗ���t���p�X�Ŋi�[

/* �t�@�C�����X�g���� */
void aiscript::FileSelector::filelist() {
#ifdef _WIN32
	std::string confPath = confpath::confPath();
	if (confPath.empty()) confPath = ".";
	std::string scriptPath = confPath + std::string("\\ai");
	std::string scriptFiles = scriptPath + std::string("\\*.lua");
	files.clear();
	WIN32_FIND_DATAA finddat;
	info(_T("AI�X�N���v�g���������܂�"));

	/* �����J�n */
	HANDLE h = FindFirstFileA(scriptFiles.c_str(), &finddat);
	if (h == INVALID_HANDLE_VALUE) {
		error(_T("�t�@�C�������ł��܂���I�I")); return;
	}
	do { // ����
		if (!strcmp(finddat.cFileName + strlen(finddat.cFileName) - 4, ".lua")) {
			CodeConv::tostringstream o; o << _T("���o: ") << finddat.cFileName; info(o.str().c_str());
			files.push_back(scriptPath + std::string("\\") + std::string(finddat.cFileName));
		}
	} while (FindNextFileA(h, &finddat));
	/* ���������I */
	FindClose(h);
#else /*_WIN32*/
	/* TODO: �������ӏ� */
#endif /*_WIN32*/
}

/* �t�@�C���������_���ɑI�� */
std::string aiscript::FileSelector::randomfile() {
	return files[RndNum::rnd(files.size())];
}
