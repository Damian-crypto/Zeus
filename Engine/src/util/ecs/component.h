#pragma once

#include "ecs.h"
#include "glm/glm.hpp"

namespace zeus
{
	struct SpriteComponent : public Component
	{
		std::string Name{ "" };
		glm::vec2 Coords{ 0.0f };

		SpriteComponent()
		{
		}
	};

	struct TransformComponent : public Component
	{
		glm::vec3 Position{ 0.0f };
		glm::vec3 Scale{ 0.0f };

		TransformComponent()
		{
		}
	};
}
