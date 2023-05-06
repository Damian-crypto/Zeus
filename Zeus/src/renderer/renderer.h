#pragma once

#include "render_command.h"
#include "glm/glm.hpp"

#include <memory>
#include <sstream>

namespace zeus
{
	class Texture;
	class SubTexture;
	class TextureManager;
	class Camera;

	struct RendererStatistics
	{
		uint32_t DrawCalls		= 0;
		uint32_t QuadsDrawn		= 0;
		uint32_t LinesDrawn		= 0;
	};

	class QuadData
	{
	private:
		glm::vec3 Position{ 0.0f };
		glm::vec3 Size{ 1.0f };
		glm::vec4 Color{ 1.0f };
		
		float RotatedAngle{ 0.0f };

		std::shared_ptr<Texture> qTexture;
		std::shared_ptr<SubTexture> qSubTexture;

		float TilingFactor = 1;

		int EntityID = -1;

		friend class Renderer;

	public:
		void SetPosition(const glm::vec3& position) { this->Position = position; }
		void SetSize(const glm::vec3& size)			{ this->Size = size; }
		void SetColor(const glm::vec4& color)		{ this->Color = color; }
		void SetRotation(float angle)				{ this->RotatedAngle = angle; }
		void SetTilingFactor(float factor)			{ this->TilingFactor = factor; }

		void SetTexture(std::shared_ptr<zeus::Texture> texture)
		{
			this->qTexture = texture;
			this->qSubTexture = nullptr;
		}

		void SetSubTexture(std::shared_ptr<zeus::SubTexture> texture)
		{
			this->qSubTexture = texture;
			this->qTexture = nullptr;
		}

		void SetEntityID(int id)
		{
			this->EntityID = id;
		}
	};

	class LineData
	{
	private:
		glm::vec3 Point1;
		glm::vec3 Point2;
		glm::vec4 Color;

		float LineWidth;

		friend class Renderer;

	public:
		void SetPoint1(const glm::vec3& point1) { this->Point1 = point1; }
		void SetPoint2(const glm::vec3& point2) { this->Point2 = point2; }
		void SetColor(const glm::vec4& color)	{ this->Color = color; }
		void SetLineWidth(float lineWidth)		{ this->LineWidth = lineWidth; }
	};

	class Renderer
	{
	public:
		// Vertex (x, y, z, r, g, b, a, tx, ty, tid, tf)
		static const uint32_t MaxQuads = 1000;
		static const uint32_t MaxQuadVertices = MaxQuads * 4;
		static const uint32_t MaxQuadIndices = MaxQuads * 6;

		static const uint32_t MaxLines = 1000;
		static const uint32_t MaxLineVertices = MaxLines * 2;

		static glm::vec4 s_BackgroundColor;

	private:
		static RenderCommand* s_QuadRenderCommand;
		static RenderCommand* s_LineRenderCommand;

	public:
		static void Init();
		static void Start(const std::shared_ptr<Camera> camera);
		static void DrawQuad(const QuadData& quad);
		static void DrawTexturedQuad(const QuadData& quad);
		static void DrawLine(const LineData& line);
		static void Flush(const std::shared_ptr<TextureManager> texManager);

		static void SetPolygonMode(bool enabled);
		static void SetDepthTest(bool enabled);
		static void SetStencilTest(bool enabled);
		static void SetColorBlending(bool enabled);
		static void SetBackgroundColor(const glm::vec4& color);
		static void Refresh();

		static RendererStatistics GetRendererStat();
	};
}
