#pragma once

#include "bodies/body.h"
#include "glm/glm.hpp"

#include <set>
#include <vector>
#include <unordered_map>
#include <memory>

#define Engine3D 0

struct Comparator
{
	constexpr bool operator()(const glm::vec3* left, const glm::vec3* right) const
	{
#if Engine3D
		return left->x < right->x && left->y < right->y && left->z <= right->z;
#else
		return left->x < right->x && left->y < right->y;
#endif
	}
};

namespace zeus
{
	struct PhyzicsStat
	{
		size_t StaticBodies{ 0 };
		size_t DynamicBodies{ 0 };
	};

	class Phyzics
	{
	private:
		std::vector<std::shared_ptr<PhyzicalBody>> m_StaticBodies;
		std::vector<std::shared_ptr<PhyzicalBody>> m_DynamicBodies;

		float m_G = 10.0f;

		static PhyzicsStat s_Stat;

	public:
		Phyzics() {}

		void AddStaticBody(std::shared_ptr<PhyzicalBody> body);
		void AddDynamicBody(std::shared_ptr<PhyzicalBody> body);
		void Step(float dt);

		static const PhyzicsStat& GetStatistics();
	};
}
