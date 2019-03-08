#pragma once

#include <random>

class RndNum {
private:
	static std::mt19937 engine;
public:
	static void init();
	static unsigned int rnd(unsigned int);
	static double rnd();
	static double rnd(double mean, double var);
	static unsigned int dice();
	template <class container> static void shuffle(container& obj) {
		std::shuffle(obj.begin(), obj.end(), engine);
	}
public: /* Monostate class: cannot be instantiated */
	RndNum() = delete;
	RndNum(const RndNum&) = delete;
	RndNum& operator= (const RndNum&) = delete;
	~RndNum() = delete;
};
