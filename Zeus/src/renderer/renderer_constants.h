#pragma once

#include <cstdint>

// Renderer constants opengl specific
static const uint32_t ColorBuffer	{ 0x00004000 };
static const uint32_t Lines			{ 0x0001 };
static const uint32_t Triangles		{ 0x0004 };
static const uint32_t RGB			{ 0x1907 };
static const uint32_t RGB8			{ 0x8051 };
static const uint32_t RGBA			{ 0x1908 };
static const uint32_t RGBA8			{ 0x8058 };
static const uint32_t REDINT		{ 0x8D94 };
static const uint32_t REDINT32		{ 0x8235 };

// Renderer flags
static const uint32_t PolygonMode	{ 1 << 1 };
static const uint32_t DepthTest		{ 1 << 2 };
static const uint32_t ColorBlend	{ 1 << 3 };
static const uint32_t StencilTest	{ 1 << 4 };
