#include "TestUtilities.h"

int test_util::generate_random_int_range(int i_start, int i_end) {
	std::random_device random_device;
	std::mt19937 gen(random_device());
	std::uniform_int_distribution<> distr(i_start, i_end);

	return distr(gen);
}

double test_util::generate_random_double_range(double d_start, double d_end) {
	std::random_device random_device;
	std::mt19937 gen(random_device());
	std::uniform_real_distribution<> distr(d_start, d_end);

	return distr(gen);
}