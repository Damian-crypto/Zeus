#include "corepch.h"
#include "renderer.h"

#include "util/texture.h"
#include "shader.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "renderer_constants.h"
#include "opengl/opengl_renderer.h"
#include "util/camera.h"
#include "util/texture_manager.h"
#include "application.h"
#include "core.h"

#include <cstring>

namespace zeus
{
	RenderCommand* Renderer::s_QuadRenderCommand = new OpenGLRenderer;
	RenderCommand* Renderer::s_LineRenderCommand = new OpenGLRenderer;

	glm::vec4 Renderer::s_BackgroundColor = glm::vec4(1.0f);

	struct QuadVertex
	{
		glm::vec3 Position;
		glm::vec4 Color;
		glm::vec2 TextureCoords;

		int TextureSlot;
		float TilingFactor;
		int EntityID;
	};

	struct RenderData
	{
		glm::mat4 Model			{ 1.0f };
		glm::mat4 View			{ 1.0f };
		glm::mat4 Projection	{ 1.0f };

		////////////////////////////////////////////
		// Data for rendering quads ////////////////
		////////////////////////////////////////////
		QuadVertex* QuadVertexData	= nullptr;
		uint32_t* QuadIndexData		= nullptr;

		std::shared_ptr<Shader> QuadShader;
		
		std::vector<glm::vec4> QuadVertices;
		std::vector<glm::vec2> TextureCoordinates;

		uint32_t NextQuadVertex	= 0;

		////////////////////////////////////////////
		// Data for rendering lines ////////////////
		////////////////////////////////////////////
		float* LineVertexData = nullptr;
		float LineWidth = 1.0f;
		std::shared_ptr<Shader> LineShader;

		uint32_t NextLineVertex = 0;

		////////////////////////////////////////////
		// Renderer data ///////////////////////////
		////////////////////////////////////////////
		uint32_t RendererState = 0;

		RendererStatistics RendererStat;
	};

	static RenderData s_RenderData;

	void Renderer::Init()
	{
		////////////////////////////////////////////
		// Initializing quad renderer //////////////
		////////////////////////////////////////////
		s_RenderData.QuadVertices = {
			{ -1.0f, -1.0f, 0.0f, 1.0f },
			{  1.0f, -1.0f, 0.0f, 1.0f },
			{  1.0f,  1.0f, 0.0f, 1.0f },
			{ -1.0f,  1.0f, 0.0f, 1.0f }
		};

		s_RenderData.TextureCoordinates = std::vector<glm::vec2>(4, glm::vec2(1.0f));

		s_RenderData.QuadShader = zeus::Shader::CreateShader("zeus_quad_shader", "assets/shaders/quad_vertex_shader.glsl", "assets/shaders/quad_fragment_shader.glsl");

		s_RenderData.QuadIndexData = new uint32_t[MaxQuadIndices]{ 0 };

		uint32_t idx = 0;
		for (uint32_t i = 0; i < MaxQuadIndices / 4; i += 4)
		{
			// 1-st triangle
			s_RenderData.QuadIndexData[idx + 0] = i + 0;
			s_RenderData.QuadIndexData[idx + 1] = i + 1;
			s_RenderData.QuadIndexData[idx + 2] = i + 3;
			// 2-nd triangle
			s_RenderData.QuadIndexData[idx + 3] = i + 1;
			s_RenderData.QuadIndexData[idx + 4] = i + 2;
			s_RenderData.QuadIndexData[idx + 5] = i + 3;

			idx += 6;
		}

		s_RenderData.NextQuadVertex = 0;

		VertexBufferLayout quadVertexLayout = {
			DataType::Float3,
			DataType::Float4,
			DataType::Float2,
			DataType::Int,
			DataType::Float,
			DataType::Int
		};

		s_RenderData.QuadVertexData = new QuadVertex[MaxQuadVertices];

		VertexArray quadVertexArray(quadVertexLayout, MaxQuadVertices, s_RenderData.QuadVertexData, MaxQuadIndices, s_RenderData.QuadIndexData);

		s_QuadRenderCommand->Init(quadVertexArray);

		////////////////////////////////////////////
		// Initializing line renderer //////////////
		////////////////////////////////////////////
		s_RenderData.LineShader = zeus::Shader::CreateShader("zeus_line_shader", "assets/shaders/line_vertex_shader.glsl", "assets/shaders/line_fragment_shader.glsl");

		s_RenderData.NextLineVertex = 0;

		VertexBufferLayout lineVertexLayout = {
			DataType::Float3,
			DataType::Float4
		};

		s_RenderData.LineVertexData = new float[MaxLineVertices * lineVertexLayout.GetStrideSize()] {0.0f};

		VertexArray lineVertexArray(lineVertexLayout, MaxLineVertices, s_RenderData.LineVertexData, 0, nullptr);

		s_LineRenderCommand->Init(lineVertexArray);
	}

	ProjectionProperties projectionProps;
	void Renderer::Start(const std::shared_ptr<Camera> camera)
	{
		const auto& props = Application::GetInstance()->GetApplicationProperties();
		
		s_RenderData.Model	= glm::mat4(1.0f);
		s_RenderData.View	= camera->GetViewMatrix();

		projectionProps.Left	= 0.0f;
		projectionProps.Right	= (float)props.Width;
		projectionProps.Bottom	= 0.0f;
		projectionProps.Top		= (float)props.Height;
		projectionProps.Near	= -1.0f;
		projectionProps.Far		= 1.0f;
		s_RenderData.Projection = camera->GetProjectionMatrix(projectionProps);

		Refresh();
	}

	void Renderer::DrawQuad(const QuadData& quad)
	{
		bool rotated = quad.RotatedAngle != 0.0f;

		// Transformation order: scale -> rotate -> transform
		glm::mat4 scale		= glm::scale(s_RenderData.Model, quad.Size);
		glm::mat4 rotation	= glm::rotate(s_RenderData.Model, glm::radians(quad.RotatedAngle), glm::vec3(0.0f, 1.0f, 0.0f));
		glm::mat4 transform = glm::translate(s_RenderData.Model, quad.Position);
		for (int i = 0; i < 4; i++)
		{
			glm::mat4 factor = transform;
			factor *= scale;
			if (rotated)
				factor *= rotation;

			glm::vec4 position = factor * s_RenderData.QuadVertices[i];
			s_RenderData.QuadVertexData[s_RenderData.NextQuadVertex].Position = position;
			s_RenderData.QuadVertexData[s_RenderData.NextQuadVertex].Color = quad.Color;
			s_RenderData.QuadVertexData[s_RenderData.NextQuadVertex].TextureCoords = { 0.0f, 0.0f };
			s_RenderData.QuadVertexData[s_RenderData.NextQuadVertex].TextureSlot = -1.0f; // tex index -1 means it is a color
			s_RenderData.QuadVertexData[s_RenderData.NextQuadVertex].TilingFactor = quad.TilingFactor;
			s_RenderData.QuadVertexData[s_RenderData.NextQuadVertex].EntityID = quad.EntityID;
			s_RenderData.NextQuadVertex++;
		}

		s_RenderData.RendererStat.QuadsDrawn++;
	}

	float texSlot = 0;
	bool rotated = false;
	void Renderer::DrawTexturedQuad(const QuadData& quad)
	{
		rotated = quad.RotatedAngle != 0.0f;
		if (quad.qSubTexture != nullptr)
		{
			quad.qSubTexture->Activate();
			texSlot = (float)quad.qSubTexture->GetTextureSlot();
			s_RenderData.TextureCoordinates = quad.qSubTexture->GetTexCoords();
		}

		if (quad.qTexture != nullptr)
		{
			quad.qTexture->Activate();
			texSlot = (float)quad.qTexture->GetTextureSlot();
			s_RenderData.TextureCoordinates = quad.qTexture->GetTexCoords();
		}

		if (quad.qTexture == nullptr && quad.qSubTexture == nullptr)
		{
			texSlot = -1;
		}

		// Transformation order: scale -> rotate -> transform
		glm::mat4 scale = glm::scale(s_RenderData.Model, quad.Size);
		glm::mat4 rotation = glm::rotate(s_RenderData.Model, glm::radians(quad.RotatedAngle), glm::vec3(0.0f, 1.0f, 0.0f));
		glm::mat4 transform = glm::translate(s_RenderData.Model, quad.Position);
		for (int i = 0; i < 4; i++)
		{
			glm::mat4 factor = transform;
			factor *= scale;
			if (rotated)
				factor *= rotation;

			glm::vec4 position = factor * s_RenderData.QuadVertices[i];
			s_RenderData.QuadVertexData[s_RenderData.NextQuadVertex].Position = position;
			s_RenderData.QuadVertexData[s_RenderData.NextQuadVertex].Color = quad.Color;
			s_RenderData.QuadVertexData[s_RenderData.NextQuadVertex].TextureCoords = s_RenderData.TextureCoordinates[i];
			s_RenderData.QuadVertexData[s_RenderData.NextQuadVertex].TextureSlot = texSlot;
			s_RenderData.QuadVertexData[s_RenderData.NextQuadVertex].TilingFactor = quad.TilingFactor;
			s_RenderData.QuadVertexData[s_RenderData.NextQuadVertex].EntityID = quad.EntityID;
			s_RenderData.NextQuadVertex++;
		}

		s_RenderData.RendererStat.QuadsDrawn++;
	}

	void Renderer::DrawLine(const LineData& line)
	{
		if (line.LineWidth != s_RenderData.LineWidth)
		{
			s_RenderData.LineWidth = line.LineWidth;
			s_LineRenderCommand->SetLineWidth(s_RenderData.LineWidth);
		}

		s_RenderData.LineVertexData[s_RenderData.NextLineVertex++] = line.Point1.x;
		s_RenderData.LineVertexData[s_RenderData.NextLineVertex++] = line.Point1.y;
		s_RenderData.LineVertexData[s_RenderData.NextLineVertex++] = line.Point1.z;

		s_RenderData.LineVertexData[s_RenderData.NextLineVertex++] = line.Color.r;
		s_RenderData.LineVertexData[s_RenderData.NextLineVertex++] = line.Color.g;
		s_RenderData.LineVertexData[s_RenderData.NextLineVertex++] = line.Color.b;
		s_RenderData.LineVertexData[s_RenderData.NextLineVertex++] = line.Color.a;

		s_RenderData.LineVertexData[s_RenderData.NextLineVertex++] = line.Point2.x;
		s_RenderData.LineVertexData[s_RenderData.NextLineVertex++] = line.Point2.y;
		s_RenderData.LineVertexData[s_RenderData.NextLineVertex++] = line.Point2.z;

		s_RenderData.LineVertexData[s_RenderData.NextLineVertex++] = line.Color.r;
		s_RenderData.LineVertexData[s_RenderData.NextLineVertex++] = line.Color.g;
		s_RenderData.LineVertexData[s_RenderData.NextLineVertex++] = line.Color.b;
		s_RenderData.LineVertexData[s_RenderData.NextLineVertex++] = line.Color.a;

		s_RenderData.RendererStat.LinesDrawn++;
	}

	void Renderer::Flush(const std::shared_ptr<TextureManager> texManager)
	{
		if (s_RenderData.NextLineVertex > 0)
		{
			s_RenderData.LineShader->Bind();
			s_RenderData.LineShader->UploadUniformMat4("u_Model", s_RenderData.Model);
			s_RenderData.LineShader->UploadUniformMat4("u_View", s_RenderData.View);
			s_RenderData.LineShader->UploadUniformMat4("u_Projection", s_RenderData.Projection);

			s_LineRenderCommand->Draw(Lines);
			s_RenderData.LineShader->Unbind();
			memset(s_RenderData.LineVertexData, 0, s_RenderData.NextLineVertex * sizeof(float));
			s_RenderData.NextLineVertex = 0;

			s_RenderData.RendererStat.DrawCalls++;
		}

		if (s_RenderData.NextQuadVertex > 0)
		{
			s_RenderData.QuadShader->Bind();
			s_RenderData.QuadShader->UploadUniformMat4("u_Model", s_RenderData.Model);
			s_RenderData.QuadShader->UploadUniformMat4("u_View", s_RenderData.View);
			s_RenderData.QuadShader->UploadUniformMat4("u_Projection", s_RenderData.Projection);

			s_RenderData.QuadShader->UploadUniformIntArray("u_Textures", texManager->GetTextureSlotData().data(), (uint32_t)texManager->GetTextureSlotData().size());
			s_QuadRenderCommand->Draw(Triangles);
			s_RenderData.QuadShader->Unbind();
			memset(s_RenderData.QuadVertexData, 0, s_RenderData.NextQuadVertex * sizeof(QuadVertex));
			s_RenderData.NextQuadVertex = 0;

			s_RenderData.RendererStat.DrawCalls++;
		}
	}

	void Renderer::SetPolygonMode(bool enabled)
	{
		if (enabled)
			s_RenderData.RendererState |= PolygonMode;
		else
			s_RenderData.RendererState &= ~PolygonMode;

		s_QuadRenderCommand->SetState(s_RenderData.RendererState);
	}

	void Renderer::SetDepthTest(bool enabled)
	{
		if (enabled)
			s_RenderData.RendererState |= DepthTest;
		else
			s_RenderData.RendererState &= ~DepthTest;

		s_QuadRenderCommand->SetState(s_RenderData.RendererState);
	}

	void Renderer::SetStencilTest(bool enabled)
	{
		if (enabled)
			s_RenderData.RendererState |= StencilTest;
		else
			s_RenderData.RendererState &= ~StencilTest;

		s_QuadRenderCommand->SetState(s_RenderData.RendererState);
	}

	void Renderer::SetColorBlending(bool enabled)
	{
		if (enabled)
			s_RenderData.RendererState |= ColorBlend;
		else
			s_RenderData.RendererState &= ~ColorBlend;

		s_QuadRenderCommand->SetState(s_RenderData.RendererState);
	}

	void Renderer::SetBackgroundColor(const glm::vec4& color)
	{
		s_BackgroundColor = color;
	}

	RendererStatistics Renderer::GetRendererStat()
	{
		RendererStatistics tmp = s_RenderData.RendererStat;
		memset(&s_RenderData.RendererStat, 0, sizeof(RendererStatistics));
		return tmp;
	}

	void Renderer::Refresh()
	{
		s_QuadRenderCommand->ClearColor(s_BackgroundColor.r, s_BackgroundColor.g, s_BackgroundColor.b, s_BackgroundColor.a);
		s_QuadRenderCommand->ClearBuffers();
	}
}
