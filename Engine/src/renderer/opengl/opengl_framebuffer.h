#pragma once

#include "renderer/framebuffer.h"

namespace zeus
{
	class OpenGLFramebuffer : public Framebuffer
	{
	public:
		OpenGLFramebuffer() {}
		OpenGLFramebuffer(const FramebufferInfo& info);
		~OpenGLFramebuffer();

		void Bind(const std::shared_ptr<TextureManager>& textureManager) override;
		void Unbind() const override;
		void Activate() const override;
		int ReadID(uint32_t attachment, float xPos, float yPos) const;

		static uint32_t GetMaxAttachments();
	};
}
