#pragma once

#include <chrono>
#include <random>

static std::mt19937 createMersenneTwisterEngine()
{
	std::random_device rd;
	std::mt19937::result_type seed = rd() ^ (
	(std::mt19937::result_type)
	std::chrono::duration_cast<std::chrono::seconds>(
		std::chrono::system_clock::now().time_since_epoch()
		).count() +
		(std::mt19937::result_type)
		std::chrono::duration_cast<std::chrono::microseconds>(
			std::chrono::high_resolution_clock::now().time_since_epoch()
			).count());

	return std::mt19937{ seed };
}
