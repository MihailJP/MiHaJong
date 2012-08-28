#ifndef RANDOM_H
#define RANDOM_H

#include <random>
#include <vector>
#include <algorithm>
#include <functional>
#include <cstdint>

class RndNum {
private:
	static std::seed_seq seed;
	static std::mt19937 engine;
public:
	__declspec(dllexport) static void init();
	__declspec(dllexport) static unsigned int rnd(unsigned int);
	static double rnd();
	__declspec(dllexport) static void frnd(double* const);
	__declspec(dllexport) static unsigned int dice();
};

#endif
