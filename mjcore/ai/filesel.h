#pragma once

#include <string>
#include <vector>
#include <sstream>
#include <Windows.h>
#include "class.h"
#include "../func.h"
#include "../random.h"

class aiscript::FileSelector {
private:
	static std::vector<std::string> files; // AI�̃X�N���v�g�t�@�C���ꗗ���t���p�X�Ŋi�[
public:
	static void filelist(); // �t�@�C���ꗗ��p�ӁI
	static std::string randomfile();
};
