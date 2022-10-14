#pragma once

#include <vector>
#include <stdexcept>

namespace zeus
{
	typedef uint32_t DrawingMode;

	enum class DataType
	{
		Int, Int2, Int3, Int4,

		Float, Float2, Float3, Float4,

		UInt, UInt2, UInt3, UInt4
	};

	class VertexBufferLayout
	{
	private:
		std::vector<DataType> m_Layout;

	public:
		VertexBufferLayout() = default;

		VertexBufferLayout(std::initializer_list<DataType> initializer)
			: m_Layout(initializer)
		{
		}

		const std::vector<DataType>& GetLayout() const
		{
			return m_Layout;
		}

		// Stride size = 1 vertex size
		size_t GetStrideSize() const
		{
			size_t bufferSize = 0;
			for (const DataType& type : m_Layout)
			{
				bufferSize += GetTypeSize(type);
			}

			return bufferSize;
		}

		uint32_t GetAttributesCount() const
		{
			return (uint32_t)m_Layout.size();
		}

		static uint32_t GetElementsOfAttribute(DataType type)
		{
			switch (type)
			{
			case DataType::Int:
			case DataType::UInt:
			case DataType::Float:
				return 1;
			case DataType::Int2:
			case DataType::UInt2:
			case DataType::Float2:
				return 2;
			case DataType::Int3:
			case DataType::UInt3:
			case DataType::Float3:
				return 3;
			case DataType::Int4:
			case DataType::UInt4:
			case DataType::Float4:
				return 4;
			}

			return 0;
		}

		static size_t GetTypeSize(DataType type)
		{
			if (type == DataType::Int)
				return sizeof(int);
			else if (type == DataType::Int2)
				return sizeof(int) * 2;
			else if (type == DataType::Int3)
				return sizeof(int) * 3;
			else if (type == DataType::Int4)
				return sizeof(int) * 4;
			else if (type == DataType::Float)
				return sizeof(float);
			else if (type == DataType::Float2)
				return sizeof(float) * 2;
			else if (type == DataType::Float3)
				return sizeof(float) * 3;
			else if (type == DataType::Float4)
				return sizeof(float) * 4;

			throw std::runtime_error("Runtime Error: Unknown data type found!");

			return 0;
		}
	};

	struct VertexArray
	{
		VertexBufferLayout VertexLayout;
		uint32_t VertexCount		= 0;
		size_t VertexSize			= 0;
		const void* VertexData		= nullptr;
		uint32_t IndexCount			= 0;
		size_t IndexSize			= 0;
		uint32_t* IndexData			= nullptr;

		VertexArray() = default;

		VertexArray(const VertexBufferLayout& layout, uint32_t vertexCount, const void* vertices, uint32_t indexCount, uint32_t* indices)
			: VertexLayout(layout),
			VertexCount(vertexCount),
			VertexData(vertices),
			IndexCount(indexCount),
			IndexData(indices)
		{
			VertexSize = VertexCount * VertexLayout.GetStrideSize();
			IndexSize = IndexCount * sizeof(uint32_t);
		}
	};

	class RenderCommand
	{
	public:
		virtual void Init(const VertexArray& vertexArray) = 0;
		virtual void Draw(DrawingMode mode) = 0;
		virtual void ClearBuffers() const = 0;
		virtual void ClearColor(float r, float g, float b, float a) const = 0;

		virtual void SetState(uint32_t flag) = 0;
		virtual void SetLineWidth(float width) = 0;
	};
}
