#include "filesel.h"

std::vector<std::string> aiscript::FileSelector::files; // AI�̃X�N���v�g�t�@�C���ꗗ���t���p�X�Ŋi�[

/* �t�@�C�����X�g���� */
void aiscript::FileSelector::filelist() {
	std::string confPath = confpath::confPath();
	if (confPath.empty()) confPath = ".";
	std::string scriptPath = confPath + std::string("\\ai");
	std::string scriptFiles = scriptPath + std::string("\\*.lua");
	files.clear();
	WIN32_FIND_DATA finddat;
	info("AI�X�N���v�g���������܂�");

	/* �����J�n */
	HANDLE h = FindFirstFile(scriptFiles.c_str(), &finddat);
	if (h == INVALID_HANDLE_VALUE) {
		error("�t�@�C�������ł��܂���I�I"); return;
	}
	do { // ����
		if (!strcmp(finddat.cFileName + strlen(finddat.cFileName) - 4, ".lua")) {
			std::ostringstream o; o << "���o: " << finddat.cFileName; info(o.str().c_str());
			files.push_back(scriptPath + std::string("\\") + std::string(finddat.cFileName));
		}
	} while (FindNextFile(h, &finddat));
	/* ���������I */
	FindClose(h);
}

/* �t�@�C���������_���ɑI�� */
std::string aiscript::FileSelector::randomfile() {
	return files[RndNum::rnd(files.size())];
}
