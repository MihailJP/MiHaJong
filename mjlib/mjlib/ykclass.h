#ifndef YKCLASS_H
#define YKCLASS_H

#include <string>
#include <functional>

using namespace std;

typedef function<bool ()> YAKUFUNC;

class Yaku {
private:
	int yakuHan; // ����𖞂����|
	int incompleteHan; // ����𖞂����Ȃ��|
	string yakuName; // ���̖��O�i������j
	YAKUFUNC yakuProc; // ���̔�����@
	Yaku() {} // Default constructor
public:
	bool checkYaku() {return this->yakuProc();} // ���𔻒肷��
	string getName() {return this->yakuName;} // ���̖��O���擾����
	// Constructor
	Yaku(string name, int han, int icHan, YAKUFUNC f) {
		this->yakuName = string(name.begin(), name.end());
		this->yakuHan = han;
		this->incompleteHan = icHan;
		this->yakuProc = YAKUFUNC(f);
	}
};

#endif
