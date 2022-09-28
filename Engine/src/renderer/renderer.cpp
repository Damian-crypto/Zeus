#include "renderer.h"

#include "utils/texture.h"
#include "core.h"
#include "shader.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "renderer_constants.h"
#include "opengl/opengl_renderer.h"

const char* vertexSource = R"(
layout (location = 0) in vec3 a_Position;
layout (location = 1) in vec4 a_Color;
layout (location = 2) in vec2 a_TexCoord;

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Projection;

out vec4 f_Color;
out vec2 f_TexCoord;

void main()
{
	f_Color = a_Color;
	f_TexCoord = a_TexCoord;
	gl_Position = u_Projection * u_View * u_Model * vec4(a_Position, 1.0);
}
)";

const char* fragmentSource = R"(
layout (location = 0) out vec4 aColor;

in vec4 f_Color;
in vec2 f_TexCoord;

uniform sampler2D u_Texture;

void main()
{
	aColor = texture(u_Texture, f_TexCoord);
}
)";

namespace zeus
{
	RenderCommand* Renderer::s_RenderCommand = new OpenGLRenderer;
	glm::vec4 Renderer::s_BackgroundColor = glm::vec4(1.0f);

	struct RenderData
	{
		float* Vertices;
		uint32_t* Indices;

		glm::mat4 Model;
		glm::mat4 View;
		glm::mat4 Projection;

		std::shared_ptr<Shader> SquareShader;

		std::vector<glm::vec4> QuadVertices;
		std::vector<glm::vec2> TextureIndices;

		uint32_t NextVertex;

		uint32_t RendererState = 0;
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

	void Renderer::Start()
	{
		s_RenderData.SquareShader->Bind();

		s_RenderData.Model = glm::mat4(1.0f);
		s_RenderData.View = glm::mat4(1.0f);
		s_RenderData.Projection = glm::mat4(1.0f);

		s_RenderData.Projection = glm::ortho(0.0f, 640.0f, 0.0f, 480.0f, 0.0f, 1.0f);

		s_RenderData.SquareShader->UploadUniformMat4("u_Model", s_RenderData.Model);
		s_RenderData.SquareShader->UploadUniformMat4("u_View", s_RenderData.View);
		s_RenderData.SquareShader->UploadUniformMat4("u_Projection", s_RenderData.Projection);
	
		s_RenderCommand->ClearColor(s_BackgroundColor.r, s_BackgroundColor.g, s_BackgroundColor.b, s_BackgroundColor.a);
		s_RenderCommand->ClearBuffers();
	}

	void Renderer::DrawQuad(const glm::vec3& pos, const glm::vec3& size, const glm::vec4& color)
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos);
		glm::mat4 scale = glm::scale(glm::mat4(1.0f), size);
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

			s_RenderData.NextVertex += 2;
		}
	}

	void Renderer::DrawTexturedQuad(const glm::vec3& pos, const glm::vec3& size, const std::shared_ptr<Texture> tex, const glm::vec4& tint)
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos);
		glm::mat4 scale = glm::scale(glm::mat4(1.0f), size);
		for (int i = 0; i < 4; i++)
		{
			glm::vec4 vertex = transform * scale * s_RenderData.QuadVertices[i];
			s_RenderData.Vertices[s_RenderData.NextVertex++] = vertex.x;
			s_RenderData.Vertices[s_RenderData.NextVertex++] = vertex.y;
			s_RenderData.Vertices[s_RenderData.NextVertex++] = vertex.z;

			s_RenderData.Vertices[s_RenderData.NextVertex++] = tint.r;
			s_RenderData.Vertices[s_RenderData.NextVertex++] = tint.g;
			s_RenderData.Vertices[s_RenderData.NextVertex++] = tint.b;
			s_RenderData.Vertices[s_RenderData.NextVertex++] = tint.a;

			s_RenderData.Vertices[s_RenderData.NextVertex++] = s_RenderData.TextureIndices[i].x;
			s_RenderData.Vertices[s_RenderData.NextVertex++] = s_RenderData.TextureIndices[i].y;
		}
	}

	void Renderer::Flush()
	{
		s_RenderCommand->Draw();
		s_RenderData.SquareShader->Unbind();
		memset(s_RenderData.Vertices, 0, s_RenderData.NextVertex * sizeof(float));
		s_RenderData.NextVertex = 0;
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

	void Renderer::SetBackgroundColor(const glm::vec4& color)
	{
		s_BackgroundColor = color;
	}
}
