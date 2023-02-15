#pragma once

#include "util/texture.h"

namespace zeus
{
	class OpenGLTexture : public Texture
	{
	private:
		// m_IsStorage is true when this class used as a container
		// Otherwise this is used as a real texture(with image)
		bool m_IsStorage = false;

	public:
		OpenGLTexture();
		OpenGLTexture(const std::string& filepath);
		~OpenGLTexture();

		void Bind(uint32_t slot = 0) override;
		void Unbind() const override;
		void Activate() override;
		static uint32_t GetTextureSlotsCount();
	};
}
