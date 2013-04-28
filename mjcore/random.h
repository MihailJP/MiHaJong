#pragma once

#include <random>

class RndNum {
private:
	static std::seed_seq seed;
	static std::mt19937 engine;
public:
	static void init();
	static unsigned int rnd(unsigned int);
	static double rnd();
	static double rnd(double mean, double var);
	static void frnd(double* const);
	static unsigned int dice();
};
