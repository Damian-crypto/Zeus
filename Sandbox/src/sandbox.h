#pragma once

#include "zeus.h"
#include "phyzicsall.h"
#include "zerializer/zerializer.hpp"
#include "util/string_manipulation.h"

#include "imgui.h"
#include "glm/gtc/type_ptr.hpp"

#include <random>
#include <iostream>
#include <random>
#include <stdexcept>
#include <fstream>

static constexpr int WIDTH = 1280;
static constexpr int HEIGHT = 720;

const glm::vec4 RED = { 1.0f, 0.0f, 0.0f, 1.0f };
const glm::vec4 BLUE = { 0.0f, 0.0f, 1.0f, 1.0f };
const glm::vec4 GREEN = { 0.0f, 1.0f, 0.0f, 1.0f };
const glm::vec4 GREY = { 0.3f, 0.3f, 0.3f, 1.0f };

static const struct DefaultOrthographicCamera
{
	glm::vec3 Position = { 0.0f, 0.0f, 0.0f };
	glm::vec3 Direction = { 0.0f, 0.0f, -1.0f };
	glm::vec3 Right = { 0.0f, 0.0f, 0.0f };
	float MovementSpeed = 30.0f;
} defaultOrthoCamera;

static const struct DefaultPerspectiveCamera
{
	glm::vec3 Position = { 0.0f, 0.0f, 600.0f };
	glm::vec3 Direction = { 0.0f, 0.0f, -1.0f };
	glm::vec3 Right = { 0.0f, 0.0f, 0.0f };
	float MovementSpeed = 0.3f;
} defaultPerspectiveCamera;

struct TileEntity
{
	bool IsActive = false;

	zeus::SpriteComponent Sprite;
	zeus::TransformComponent Transform;
};

glm::vec3 spritePos	{ 0.0f, 0.0f, 0.1f };
glm::vec3 scaler	{ 50.0f, 50.0f, 0.0f };
glm::bvec3 mouseButtons{ false };
float zVal = 0.0f;
float angle = 0.0f;
std::shared_ptr<zeus::TextureManager> textureManager;
