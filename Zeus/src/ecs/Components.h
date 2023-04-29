#pragma once

#include "glm/glm.hpp"

#include <string>

namespace zeus
{
	struct SpriteComponent
	{
		std::string TextureName{ "default_texture"};
		glm::ivec2 Coordinates{ 0, 0};
	};

	struct TransformComponent
	{
		glm::vec3 Position{ 0.0f };
		glm::vec3 Scale{ 32.0f, 32.0f, 0.0f };
		float Rotation = 0.0f;
	};
}
