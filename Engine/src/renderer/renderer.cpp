#include "renderer.h"

#include "util/texture.h"
#include "core.h"
#include "shader.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "renderer_constants.h"
#include "opengl/opengl_renderer.h"
#include "util/camera.h"
#include "application.h"

const char* vertexSource = R"(
#version 450

layout (location = 0) in vec3 a_Position;
layout (location = 1) in vec4 a_Color;
layout (location = 2) in vec2 a_TexCoord;
layout (location = 3) in float a_TexId;

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Projection;

out vec4 f_Color;
out vec2 f_TexCoord;
out float f_TexId;

void main()
{
	f_Color = a_Color;
	f_TexCoord = a_TexCoord;
	f_TexId = a_TexId;
	gl_Position = u_Projection * u_View * u_Model * vec4(a_Position, 1.0);
}
)";

const char* fragmentSource = R"(
#version 450

layout (location = 0) out vec4 aColor;

in vec4 f_Color;
in vec2 f_TexCoord;
in float f_TexId;

uniform sampler2D u_Textures[2];

void main()
{
	int texIndex = int(f_TexId);
	aColor = texture(u_Textures[texIndex], f_TexCoord);
	//aColor = vec4(vec3(gl_FragCoord.z), 1.0f);
	//aColor = f_Color;
}
)";

namespace zeus
{
	RenderCommand* Renderer::s_RenderCommand = new OpenGLRenderer;
	glm::vec4 Renderer::s_BackgroundColor = glm::vec4(1.0f);

	struct RenderData
	{
		float* Vertices		= nullptr;
		uint32_t* Indices	= nullptr;

		glm::mat4 Model			{ 1.0f };
		glm::mat4 View			{ 1.0f };
		glm::mat4 Projection	{ 1.0f };

		std::shared_ptr<Shader> SquareShader;

		std::vector<glm::vec4> QuadVertices;
		std::vector<glm::vec2> TextureIndices;

		uint32_t NextVertex		= 0;
		uint32_t RendererState	= 0;
		std::vector<int> TextureSlots;
	};

	static RenderData s_RenderData;

	void Renderer::Init()
	{
		s_RenderData.QuadVertices = {
			{ -1.0f, -1.0f, 0.0f, 1.0f },
			{  1.0f, -1.0f, 0.0f, 1.0f },
			{  1.0f,  1.0f, 0.0f, 1.0f },
			{ -1.0f,  1.0f, 0.0f, 1.0f }
		};

		s_RenderData.TextureIndices = {
			{ 0.0f, 0.0f },
			{ 1.0f, 0.0f },
			{ 1.0f, 1.0f },
			{ 0.0f, 1.0f }
		};

		s_RenderData.Indices = new uint32_t[MaxIndices]{ 0 };

		uint32_t idx = 0;
		for (uint32_t i = 0; i < MaxIndices / 4; i += 4)
		{
			// 1-st triangle
			s_RenderData.Indices[idx + 0] = i + 0;
			s_RenderData.Indices[idx + 1] = i + 1;
			s_RenderData.Indices[idx + 2] = i + 3;
			// 2-nd triangle
			s_RenderData.Indices[idx + 3] = i + 1;
			s_RenderData.Indices[idx + 4] = i + 2;
			s_RenderData.Indices[idx + 5] = i + 3;

			idx += 6;
		}

		s_RenderData.NextVertex = 0;

		s_RenderData.Vertices = new float[MaxVertices]{ 0.0f };

		s_RenderData.SquareShader = zeus::Shader::CreateShader("zeus_square_shader", vertexSource, fragmentSource);

		s_RenderCommand->Init(MaxVertices, s_RenderData.Vertices, MaxIndices, s_RenderData.Indices);
	}

	void Renderer::Start(const std::shared_ptr<Camera> camera)
	{
		const auto& props = Application::GetInstance()->GetApplicationProperties();

		s_RenderData.Model = glm::mat4(1.0f);
		s_RenderData.View = glm::mat4(1.0f);
		s_RenderData.View = camera->GetViewMatrix();

		ProjectionProperties projectionProps;
		projectionProps.Left = 0.0f;
		projectionProps.Right = (float)props.Width;
		projectionProps.Bottom = 0.0f;
		projectionProps.Top = (float)props.Height;
		projectionProps.Near = -1.0f;
		projectionProps.Far = 1.0f;
		s_RenderData.Projection = camera->GetProjectionMatrix(projectionProps);

		s_RenderCommand->ClearColor(s_BackgroundColor.r, s_BackgroundColor.g, s_BackgroundColor.b, s_BackgroundColor.a);
		s_RenderCommand->ClearBuffers();
	}

	void Renderer::DrawQuad(const glm::vec3& pos, const glm::vec3& size, float angle, const glm::vec4& color)
	{
		glm::mat4 transform = glm::translate(s_RenderData.Model, pos);
		glm::mat4 scale = glm::scale(s_RenderData.Model, size);
		for (int i = 0; i < 4; i++)
		{
			glm::vec4 vertex = transform * scale * s_RenderData.QuadVertices[i];
			s_RenderData.Vertices[s_RenderData.NextVertex++] = vertex.x;
			s_RenderData.Vertices[s_RenderData.NextVertex++] = vertex.y;
			s_RenderData.Vertices[s_RenderData.NextVertex++] = vertex.z;

			s_RenderData.Vertices[s_RenderData.NextVertex++] = color.r;
			s_RenderData.Vertices[s_RenderData.NextVertex++] = color.g;
			s_RenderData.Vertices[s_RenderData.NextVertex++] = color.b;
			s_RenderData.Vertices[s_RenderData.NextVertex++] = color.a;

			s_RenderData.NextVertex += 3;
		}
	}

	void Renderer::DrawTexturedQuad(const glm::vec3& pos, const glm::vec3& size, const std::shared_ptr<Texture> tex, float angle, const glm::vec4& tint)
	{
		s_RenderData.TextureSlots.push_back((int)tex->GetTextureSlot());
		s_RenderCommand->BindTextureUnit(tex->GetTextureSlot(), tex->GetTextureID());
		glm::mat4 transform = glm::translate(s_RenderData.Model, pos);
		glm::mat4 scale = glm::scale(s_RenderData.Model, size);
		glm::mat4 rotation = glm::rotate(s_RenderData.Model, glm::radians(angle), glm::vec3(0.0f, 1.0f, 0.0f));
		for (int i = 0; i < 4; i++)
		{
			glm::vec4 vertex = transform * rotation * scale * s_RenderData.QuadVertices[i];
			s_RenderData.Vertices[s_RenderData.NextVertex++] = vertex.x;
			s_RenderData.Vertices[s_RenderData.NextVertex++] = vertex.y;
			s_RenderData.Vertices[s_RenderData.NextVertex++] = vertex.z;

			s_RenderData.Vertices[s_RenderData.NextVertex++] = tint.r;
			s_RenderData.Vertices[s_RenderData.NextVertex++] = tint.g;
			s_RenderData.Vertices[s_RenderData.NextVertex++] = tint.b;
			s_RenderData.Vertices[s_RenderData.NextVertex++] = tint.a;

			s_RenderData.Vertices[s_RenderData.NextVertex++] = s_RenderData.TextureIndices[i].x;
			s_RenderData.Vertices[s_RenderData.NextVertex++] = s_RenderData.TextureIndices[i].y;
			s_RenderData.Vertices[s_RenderData.NextVertex++] = (float)tex->GetTextureSlot();
		}
	}

	void Renderer::Flush()
	{
		s_RenderData.SquareShader->Bind();
		s_RenderData.SquareShader->UploadUniformMat4("u_Model", s_RenderData.Model);
		s_RenderData.SquareShader->UploadUniformMat4("u_View", s_RenderData.View);
		s_RenderData.SquareShader->UploadUniformMat4("u_Projection", s_RenderData.Projection);

		s_RenderData.SquareShader->UploadUniformIntArray("u_Textures", s_RenderData.TextureSlots.data(), (uint32_t)s_RenderData.TextureSlots.size());
		s_RenderCommand->Draw();
		s_RenderData.SquareShader->Unbind();
		memset(s_RenderData.Vertices, 0, s_RenderData.NextVertex * sizeof(float));
		s_RenderData.NextVertex = 0;
		s_RenderData.TextureSlots.clear();
	}

	void Renderer::SetPolygonMode(bool enabled)
	{
		if (enabled)
			s_RenderData.RendererState |= PolygonMode;
		else
			s_RenderData.RendererState &= ~PolygonMode;

		s_RenderCommand->SetState(s_RenderData.RendererState);
	}

	void Renderer::SetDepthTest(bool enabled)
	{
		if (enabled)
			s_RenderData.RendererState |= DepthTest;
		else
			s_RenderData.RendererState &= ~DepthTest;

		s_RenderCommand->SetState(s_RenderData.RendererState);
	}

	void Renderer::SetColorBlending(bool enabled)
	{
		if (enabled)
			s_RenderData.RendererState |= ColorBlend;
		else
			s_RenderData.RendererState &= ~ColorBlend;

		s_RenderCommand->SetState(s_RenderData.RendererState);
	}

	void Renderer::SetBackgroundColor(const glm::vec4& color)
	{
		s_BackgroundColor = color;
	}
}
