#pragma once

#include <random>

namespace zeus
{
	class Random
	{
	public:
		Random() = delete;

		static float GetFloat(float low, float high);
		static int GetInt(int low, int high);
		static uint32_t GetUint(uint32_t low, uint32_t high);
	};
}
