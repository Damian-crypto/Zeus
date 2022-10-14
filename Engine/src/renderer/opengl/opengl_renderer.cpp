#include "corepch.h"
#include "opengl_renderer.h"

#include "glad/gl.h"
#include "core.h"
#include "debugger.h"

#include "opengl_shader.h"
#include "renderer/renderer_constants.h"

namespace zeus
{
	unsigned int GetTypeOpenGL(DataType type)
	{
		switch (type)
		{
			case DataType::Int:
			case DataType::Int2:
			case DataType::Int3:
			case DataType::Int4:
				return GL_INT;
			case DataType::Float:
			case DataType::Float2:
			case DataType::Float3:
			case DataType::Float4:
				return GL_FLOAT;
		}

		throw std::runtime_error("Runtime Error: Unknown data type found!");

		return GL_FLOAT;
	}

	void OpenGLRenderer::Init(const VertexArray& vertexArray)
	{
		m_VertexArray = vertexArray;

		m_IndexedRendering = m_VertexArray.IndexCount > 0;

		m_Buffers |= GL_COLOR_BUFFER_BIT;

		glGenVertexArrays(1, &m_VAO);
		glGenBuffers(1, &m_VBO);

		if (m_IndexedRendering)
		{
			glGenBuffers(1, &m_EBO);
		}

		glBindVertexArray(m_VAO);
		
		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
		glBufferData(GL_ARRAY_BUFFER, m_VertexArray.VertexSize, m_VertexArray.VertexData, GL_STATIC_DRAW);
		
		if (m_IndexedRendering)
		{
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_VertexArray.IndexSize, m_VertexArray.IndexData, GL_STATIC_DRAW);
		}
		
		const auto& layout = m_VertexArray.VertexLayout;
		int strideSize = (int)layout.GetStrideSize();
		size_t offset = 0;
		const auto& attributes = m_VertexArray.VertexLayout.GetLayout();
		for (int i = 0; i < (int)layout.GetAttributesCount(); i++)
		{
			glEnableVertexAttribArray(i);
			int elements = VertexBufferLayout::GetElementsOfAttribute(attributes[i]);
			unsigned int type = GetTypeOpenGL(attributes[i]);
			glVertexAttribPointer(i, elements, type, GL_FALSE, strideSize, (const void*)offset);
			offset += VertexBufferLayout::GetTypeSize(attributes[i]);
		}
	}

	void OpenGLRenderer::Draw(DrawingMode mode)
	{
		ReplaceVertexData();

		glBindVertexArray(m_VAO);
		if (m_IndexedRendering)
		{
			glDrawElements(mode, m_VertexArray.IndexCount, GL_UNSIGNED_INT, 0);
		}
		else
		{
			glDrawArrays(mode, 0, m_VertexArray.VertexCount);
		}
		glBindVertexArray(0);
		GLVALIDATE;
	}

	void OpenGLRenderer::ReplaceVertexData()
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
		if (m_IndexedRendering)
		{
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
		}
		glBufferSubData(GL_ARRAY_BUFFER, 0, m_VertexArray.VertexSize, m_VertexArray.VertexData);
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

	void OpenGLRenderer::SetLineWidth(float width)
	{
		glLineWidth(width);
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
