#pragma once

#include "renderer/render_command.h"

namespace zeus
{
	class OpenGLRenderer : public RenderCommand
	{
	private:
		const float* m_VertexData;
		const uint32_t* m_IndexData;
		uint32_t m_VertexCount = 0;
		uint32_t m_IndexCount = 0;
		uint32_t m_VAO = 0;
		uint32_t m_VBO = 0;
		uint32_t m_EBO = 0;
		uint32_t m_Buffers = 0;

	public:
		OpenGLRenderer() = default;

		void Init(uint32_t vertexCount, const float* vertices, uint32_t indexCount, uint32_t* indices) override;
		void Draw() override;
		void SetState(uint32_t flag) override;
		void ClearBuffers() override;
		void ClearColor(float r, float g, float b, float a) override;

	private:
		void ReplaceVertexData(uint32_t count, const float* vertices);
	};
}
