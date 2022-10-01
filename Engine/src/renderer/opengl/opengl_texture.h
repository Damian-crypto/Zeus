#pragma once

#include "util/texture.h"

namespace zeus
{
	class OpenGLTexture : public Texture
	{
	public:
		OpenGLTexture(const std::string& filepath);

		void Bind(uint32_t slot = 0) override;
		void Unbind() const override;
		uint32_t GetTextureSlotsCount() const override;
	};
}
