#pragma once

#include <vector>

namespace zeus
{
	class RenderCommand
	{
	public:
		virtual void Init(uint32_t vertexCount, const float* vertices, uint32_t indexCount, uint32_t* indices) = 0;
		virtual void Draw() = 0;
		virtual void ClearBuffers() const = 0;
		virtual void ClearColor(float r, float g, float b, float a) const = 0;
		virtual void BindTextureUnit(uint32_t slot, uint32_t texId) = 0;

		virtual void SetState(uint32_t flag) = 0;
	};
}
