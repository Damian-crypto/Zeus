#include "corepch.h"
#include "framebuffer.h"

#include "opengl/opengl_framebuffer.h"

namespace zeus
{
	std::shared_ptr<Framebuffer> Framebuffer::CreateFramebuffer(const FramebufferInfo& info)
	{
		return std::make_shared<OpenGLFramebuffer>(info);
	}

	uint32_t Framebuffer::GetMaxAttachments()
	{
		return OpenGLFramebuffer::GetMaxAttachments();
	}
}
