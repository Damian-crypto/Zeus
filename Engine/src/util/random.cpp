#include "corepch.h"
#include "random.h"

namespace zeus
{
	float Random::GetFloat(float low, float high)
	{
		static std::random_device rd;
		static std::mt19937 gen(rd());
		static std::uniform_real_distribution<float> dist(low, high);

		return dist(gen);
	}

	int Random::GetInt(int low, int high)
	{
		static std::random_device rd;
		static std::mt19937 gen(rd());
		static std::uniform_int_distribution<int> dist(low, high);

		return dist(gen);
	}

	uint32_t Random::GetUint(uint32_t low, uint32_t high)
	{
		static std::random_device rd;
		static std::mt19937 gen(rd());
		static std::uniform_int_distribution<uint32_t> dist(low, high);

		return dist(gen);
	}
}
