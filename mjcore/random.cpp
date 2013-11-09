#include "random.h"

#include <vector>
#include <algorithm>
#include <functional>
#include <cstdint>
#include <cmath>
#include <cassert>

std::mt19937 RndNum::engine;

void RndNum::init() {
	std::random_device rnd;
	std::vector<std::uint32_t> v(624);
	std::generate(v.begin(), v.end(), std::ref(rnd));
	std::seed_seq& seed(std::seed_seq(v.begin(), v.end()));
	engine = std::mt19937(seed);
}

/* 一様分布整乱数 */
unsigned int RndNum::rnd(unsigned int k) {
	assert(k > 0);
	std::uniform_int_distribution<unsigned int> distrib(0, k-1);
	return distrib(engine);
}

/* 一様分布浮動小数点乱数 */
double RndNum::rnd() {
	std::uniform_real_distribution<double> distrib(0.0, 1.0); // 半開区間 [0,1)
	return distrib(engine);
}

/* 正規分布乱数 */
double RndNum::rnd(double mean, double var) {
	std::normal_distribution<double> distrib(mean, std::sqrt(var));
	return distrib(engine);
}

/* サイコロ */
unsigned int RndNum::dice() {
	std::uniform_int_distribution<unsigned int> distrib(1u, 6u);
	return distrib(engine);
}
