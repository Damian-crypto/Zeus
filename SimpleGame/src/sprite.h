#include "glm/glm.hpp"

#include <string>

struct Sprite
{
	glm::vec3 Coords = glm::vec3(0.0f);
	glm::vec3 Size;
	std::string Name;
};
