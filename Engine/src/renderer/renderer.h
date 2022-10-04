#pragma once

#include "render_command.h"
#include "glm/glm.hpp"

#include <memory>

namespace zeus
{
	class Texture;
	class Camera;
	class TextureManager;

	struct RendererStatistics
	{
		uint32_t DrawCalls		= 0;
		uint32_t QuadsDrawn		= 0;
	};

	class Renderer
	{
	public:
		// Vertex (x, y, z, r, g, b, a, tx, ty, tid)
		static const uint32_t MaxQuads = 1000;
		static const uint32_t MaxVertices = MaxQuads * 4 * 10;
		static const uint32_t MaxIndices = MaxQuads * 6;
		static glm::vec4 s_BackgroundColor;

	private:
		static RenderCommand* s_RenderCommand;

	public:
		static void Init();
		static void Start(const std::shared_ptr<Camera> camera);
		static void DrawQuad(const glm::vec3& pos, const glm::vec3& size, float angle = 0.0f, const glm::vec4& color = glm::vec4(1.0f));
		static void DrawTexturedQuad(const glm::vec3& pos, const glm::vec3& size, const std::shared_ptr<Texture> tex, float angle = 0.0f, const glm::vec4& tint = glm::vec4(1.0f));
		static void Flush(const std::shared_ptr<TextureManager> texManager);
		static void SetPolygonMode(bool enabled);
		static void SetDepthTest(bool enabled);
		static void SetColorBlending(bool enabled);
		static void SetBackgroundColor(const glm::vec4& color);
		static RendererStatistics GetRendererStat();
	};
}
