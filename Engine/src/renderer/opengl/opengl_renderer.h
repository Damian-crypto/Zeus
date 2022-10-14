#pragma once

#include "renderer/render_command.h"

namespace zeus
{
	class OpenGLRenderer : public RenderCommand
	{
	private:
		VertexArray m_VertexArray;

		uint32_t m_VAO = 0;
		uint32_t m_VBO = 0;
		uint32_t m_EBO = 0;
		uint32_t m_Buffers = 0;

		bool m_IndexedRendering = true;

	public:
		OpenGLRenderer() = default;

		void Init(const VertexArray& vertexArray) override;
		void Draw(DrawingMode mode) override;
		void SetState(uint32_t flag) override;
		void SetLineWidth(float width) override;
		void ClearBuffers() const override;
		void ClearColor(float r, float g, float b, float a) const override;

	private:
		void ReplaceVertexData();
	};
}
