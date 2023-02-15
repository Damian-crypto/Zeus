#include "corepch.h"
#include "opengl_texture.h"

#include "stb/stb_image.h"
#include "glad/gl.h"
#include "renderer/renderer.h"
#include "util/logger.h"

namespace zeus
{
	OpenGLTexture::OpenGLTexture()
		: m_IsStorage(true)
	{
		m_Filepath = "[this is a texture storage]";
	}

	OpenGLTexture::OpenGLTexture(const std::string& filepath)
		: m_IsStorage(false)
	{
		m_Filepath = filepath;
	}

	OpenGLTexture::~OpenGLTexture()
	{
		// glDeleteTextures(1, &m_TextureID);
	}

	void OpenGLTexture::Bind(uint32_t slot)
	{
		uint32_t slotsCount = GetTextureSlotsCount();
		if (slotsCount <= slot)
		{
			std::string errorMsg = "Runtime Error: Your gpu has only " + std::to_string(slotsCount) + " texture slots!";
			LOG(Error, errorMsg.c_str());
		}

		m_TextureSlot = slot;

		if (m_IsStorage)
		{
			glCreateTextures(GL_TEXTURE_2D, 1, &m_TextureID);
			glBindTexture(GL_TEXTURE_2D, m_TextureID);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, m_Pixelated ? GL_NEAREST : GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, m_Pixelated ? GL_NEAREST : GL_LINEAR);

			glTexImage2D(GL_TEXTURE_2D, 0, m_InternalFormat, m_Width, m_Height, 0, m_PixelFormat, GL_UNSIGNED_BYTE, nullptr);
		}
		else
		{
			stbi_set_flip_vertically_on_load(1);

			int width, height, channels;
			stbi_uc* data = stbi_load(m_Filepath.c_str(), &width, &height, &channels, 0);

			glCreateTextures(GL_TEXTURE_2D, 1, &m_TextureID);
			glBindTexture(GL_TEXTURE_2D, m_TextureID);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, m_Pixelated ? GL_NEAREST : GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, m_Pixelated ? GL_NEAREST : GL_LINEAR);

			m_Width = (uint32_t)width;
			m_Height = (uint32_t)height;

			m_InternalFormat = GL_RED;	// defines the format that OpenGL should use to store the data internally
			m_PixelFormat = GL_RED;		// describes the data you pass in as the last argument
			switch (channels)
			{
			case 3:
				m_InternalFormat = GL_RGB;
				m_PixelFormat = GL_RGB;
				break;
			case 4:
				m_InternalFormat = GL_RGBA8;
				m_PixelFormat = GL_RGBA;
				break;
			}

			uint32_t mipLevel = 0; // 0 - largest, 1 - half of the largest, 2 - half of half
			if (data != nullptr)
			{
				glTexImage2D(GL_TEXTURE_2D, mipLevel, m_InternalFormat, m_Width, m_Height, 0, m_PixelFormat, GL_UNSIGNED_BYTE, data);
				// Mipmaps are smaller resolution images of the same image
				// That are used when textures are far-away from camera
				glGenerateMipmap(GL_TEXTURE_2D);
			}
			else
			{
				std::string errorMsg = "Runtime Error: Failed to load image '" + m_Filepath + "'";
				LOG_ENGINE(Error, errorMsg.c_str());
			}

			stbi_image_free(data);
		}
	}

	void OpenGLTexture::Unbind() const
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	void OpenGLTexture::Activate()
	{
		glActiveTexture(GL_TEXTURE0 + m_TextureSlot);	// activate the texture unit before use it
		glBindTexture(GL_TEXTURE_2D, m_TextureID);		// bind the texture to that unit
	}

	uint32_t OpenGLTexture::GetTextureSlotsCount()
	{
		int texSlots = 0;
		glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &texSlots);

		return (uint32_t)texSlots;
	}
}
