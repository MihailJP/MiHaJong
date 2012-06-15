#ifndef YKCLASS_H
#define YKCLASS_H

class Yaku {
private:
	int yakuHan;
	std::function<bool ()> yakuProc ();
public:
	bool checkYaku() {return this.yakuProc();}
}

#endif
