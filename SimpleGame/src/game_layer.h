#pragma once

#include "zeus.h"

#include <string>

static const int WIDTH	= 1024;
static const int HEIGHT = 640;

const glm::vec4 RED		= { 1.0f, 0.0f, 0.0f, 1.0f };
const glm::vec4 BLUE	= { 0.0f, 0.0f, 1.0f, 1.0f };
const glm::vec4 GREEN	= { 0.0f, 1.0f, 0.0f, 1.0f };
const glm::vec4 GREY	= { 0.3f, 0.3f, 0.3f, 1.0f };
const glm::vec4 CYAN	= { 0.0f, 1.0f, 1.0f, 1.0f };

struct vec2i
{
	int x, y;
};

class GameLayer : public zeus::Layer
{
private:
	std::string m_Name;
	std::shared_ptr<zeus::Camera> m_Camera;
	std::shared_ptr<zeus::TextureManager> m_TexManager;

	bool m_Keys[65536] = { false };

	// coord - x, y, z
	// direction - w
	//    0 - look left
	//    1 - look backward
	//    2 - look forward
	//    3 - look right
	glm::vec3 m_CharacterSprite;
	glm::vec3 m_CharacterPos;

public:
	GameLayer(const char* name);

	void OnUpdate(float dt) override;
	void OnEvent(zeus::Event& evt) override;
	void OnRender() override;
};
