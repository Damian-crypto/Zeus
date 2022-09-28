#pragma once

#include <cstdint>

// Renderer constants
static const uint32_t ColorBuffer	{ 0x00004000 };

// Renderer flags
static const uint32_t PolygonMode	{ 1 << 1 };
static const uint32_t DepthTest		{ 1 << 2 };
static const uint32_t ColorBlend	{ 1 << 3 };
