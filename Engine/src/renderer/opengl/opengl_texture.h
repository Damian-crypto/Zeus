#pragma once

#include "utils/texture.h"

namespace zeus
{
	class OpenGLTexture : public Texture
	{
	public:
		OpenGLTexture(const std::string& filepath);

		void Bind(uint32_t slot = 0) const override;
		void Unbind() const override;
	};
}
