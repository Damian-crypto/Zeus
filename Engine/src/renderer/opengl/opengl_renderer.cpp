#include "opengl_renderer.h"

#include "glad/gl.h"

#include "core.h"
#include "opengl_shader.h"
#include "renderer/renderer_constants.h"

namespace zeus
{
	void OpenGLRenderer::Init(uint32_t vertexCount, const float* vertices, uint32_t indexCount, uint32_t* indices)
	{
		m_VertexCount = vertexCount;
		m_IndexCount = indexCount;
		m_VertexData = vertices;
		m_IndexData = indices;
		m_Buffers |= GL_COLOR_BUFFER_BIT;

		glGenVertexArrays(1, &m_VAO);
		glGenBuffers(1, &m_VBO);
		glGenBuffers(1, &m_EBO);

		glBindVertexArray(m_VAO);
		
		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
		glBufferData(GL_ARRAY_BUFFER, m_VertexCount * sizeof(float), m_VertexData, GL_STATIC_DRAW);
		
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_IndexCount * sizeof(uint32_t), m_IndexData, GL_STATIC_DRAW);
		
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 10 * sizeof(float), (const void*)0);
		
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 10 * sizeof(float), (const void*)(3 * sizeof(float)));

		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 10 * sizeof(float), (const void*)(7 * sizeof(float)));

		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, 10 * sizeof(float), (const void*)(9 * sizeof(float)));

		//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		//glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void OpenGLRenderer::Draw()
	{
		ReplaceVertexData(m_VertexCount, m_VertexData);

		glBindVertexArray(m_VAO);
		GLValidate(glDrawElements(GL_TRIANGLES, m_IndexCount, GL_UNSIGNED_INT, 0));
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void OpenGLRenderer::BindTextureUnit(uint32_t slot, uint32_t texId)
	{
		glBindTextureUnit(slot, texId);
	}

	void OpenGLRenderer::ReplaceVertexData(uint32_t count, const float* vertices)
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, count * sizeof(float), vertices);
	}

	void OpenGLRenderer::SetState(uint32_t flag)
	{
		if (flag & PolygonMode)
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		else
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		if (flag & DepthTest)
		{
			glEnable(GL_DEPTH_TEST);
			glDepthFunc(GL_LESS);
			m_Buffers |= GL_DEPTH_BUFFER_BIT;
		}
		else
		{
			glDisable(GL_DEPTH_TEST);
			m_Buffers &= ~GL_DEPTH_BUFFER_BIT;
		}

		if (flag & ColorBlend)
		{
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		}
		else
		{
			glDisable(GL_BLEND);
		}
	}

	void OpenGLRenderer::ClearBuffers() const
	{
		glClear(m_Buffers);
	}

	void OpenGLRenderer::ClearColor(float r, float g, float b, float a) const
	{
		glClearColor(r, g, b, a);
	}
}
