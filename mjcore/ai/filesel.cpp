#include "filesel.h"

#include <sstream>
#ifdef _WIN32
#include <windows.h>
#else  /*_WIN32*/
#include <dirent.h>
#endif /*_WIN32*/
#include "../func.h"
#include "../random.h"

std::vector<std::string> aiscript::FileSelector::files; // AI�̃X�N���v�g�t�@�C���ꗗ���t���p�X�Ŋi�[

/* �t�@�C�����X�g���� */
void aiscript::FileSelector::filelist() {
	std::string confPath = confpath::confPath();
	if (confPath.empty()) confPath = ".";
#ifdef _WIN32
#ifdef GUOBIAO
	std::string scriptPath = confPath + std::string("\\gbai");
#else /* GUOBIAO */
	std::string scriptPath = confPath + std::string("\\ai");
#endif /* GUOBIAO */
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
#ifdef GUOBIAO
	std::string scriptPath = confPath + std::string("/gbai");
#else /* GUOBIAO */
	std::string scriptPath = confPath + std::string("/ai");
#endif /* GUOBIAO */
	files.clear();
	info(_T("AI�X�N���v�g���������܂�"));

	/* �����J�n */
	DIR* dir = opendir(scriptPath.c_str());
	dirent* dirp;
	if (!dir) {
		error(_T("�t�@�C�������ł��܂���I�I")); return;
	}
	while (dirp = readdir(dir)) { // ����
		if (!strcmp(dirp->d_name + strlen(dirp->d_name) - 4, ".lua")) {
			CodeConv::tostringstream o; o << _T("���o: ") << (dirp->d_name); info(o.str().c_str());
			files.push_back(scriptPath + std::string("/") + std::string(dirp->d_name));
		}
	}
	/* ���������I */
	closedir(dir);
#endif /*_WIN32*/
}

/* �t�@�C���������_���ɑI�� */
std::string aiscript::FileSelector::randomfile() {
	return files[RndNum::rnd(files.size())];
}
