#include "corepch.h"
#include "opengl_framebuffer.h"

#include "debugger.h"
#include "glad/gl.h"
#include "util/texture_manager.h"
#include "util/texture.h"

namespace zeus
{
	OpenGLFramebuffer::OpenGLFramebuffer(const FramebufferInfo& info)
	{
		if (info.Attachments.size() > GetMaxAttachments())
		{
			throw std::runtime_error("Runtime Error: You are exceeding your maximum color attachments count!");
		}

		m_FramebufferInfo = info;
	}

	OpenGLFramebuffer::~OpenGLFramebuffer()
	{
		glDeleteFramebuffers(1, &m_FramebufferID);
	}

	void OpenGLFramebuffer::Bind(const std::shared_ptr<TextureManager>& textureManager)
	{
		glGenFramebuffers(1, &m_FramebufferID);
		glBindFramebuffer(GL_FRAMEBUFFER, m_FramebufferID);

		uint32_t idx = 0;
		for (const auto& attachment : m_FramebufferInfo.Attachments)
		{
			std::string name = "_framebuffer_" + std::to_string(idx);
			std::shared_ptr<Texture> texture = Texture::CreateTexture();
			texture->SetInternalFormat(attachment);
			textureManager->PutTexture(name, texture);
			m_Attachments.push_back(texture);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + idx, GL_TEXTURE_2D, (int)texture->GetTextureID(), 0);
			idx++;
		}

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		{
			throw std::runtime_error("Runtime Error: Framebuffer is not complete!");
		}

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void OpenGLFramebuffer::Unbind() const
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	GLenum attachments[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
	void OpenGLFramebuffer::Activate() const
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_FramebufferID);

		for (const auto& texture : m_Attachments)
		{
			texture->Activate();
		}

		glDrawBuffers((int)m_Attachments.size(), attachments);
	}

	uint32_t OpenGLFramebuffer::GetMaxAttachments()
	{
		int value;
		glGetIntegerv(GL_MAX_COLOR_ATTACHMENTS, &value);
		return (uint32_t)value;
	}

	int OpenGLFramebuffer::ReadID(uint32_t attachment, float xPos, float yPos) const
	{
		int value;
		glBindFramebuffer(GL_FRAMEBUFFER, m_FramebufferID);
		glReadBuffer(GL_COLOR_ATTACHMENT0 + attachment);
		glReadPixels(xPos, yPos, 1, 1, GL_RED_INTEGER, GL_INT, &value);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		return value;
	}
}
