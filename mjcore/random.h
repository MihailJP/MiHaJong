#pragma once

#include <random>
#include <vector>
#include <algorithm>
#include <functional>
#include <cstdint>
#include <cmath>

class RndNum {
private:
	static std::seed_seq seed;
	static std::mt19937 engine;
public:
	__declspec(dllexport) static void init();
	__declspec(dllexport) static unsigned int rnd(unsigned int);
	static double rnd();
	static double rnd(double mean, double var);
	__declspec(dllexport) static void frnd(double* const);
	__declspec(dllexport) static unsigned int dice();
};
