#ifndef FILESEL_H
#define FILESEL_H

#include <string>
#include <set>
#include <sstream>
#include <Windows.h>
#include "class.h"
#include "../func.h"

class aiscript::FileSelector {
private:
	static std::set<std::string> files; // AI�̃X�N���v�g�t�@�C���ꗗ���t���p�X�Ŋi�[
public:
	static void filelist(); // �t�@�C���ꗗ��p�ӁI
};

#endif
