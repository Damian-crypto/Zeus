#include "opengl_texture.h"

#include "core.h"

#include "stb/stb_image.h"
#include "glad/gl.h"
#include "renderer/renderer.h"

namespace zeus
{
	OpenGLTexture::OpenGLTexture(const std::string& filepath)
	{
		m_Filepath = filepath;
	}

	OpenGLTexture::~OpenGLTexture()
	{
		glDeleteTextures(1, &m_TextureID);
	}

	void OpenGLTexture::Bind(uint32_t slot)
	{
		uint32_t slotsCount = GetTextureSlotsCount();
		if (slotsCount <= slot)
		{
			throw std::runtime_error("Runtime Error: Your gpu has only " + std::to_string(slotsCount) + " texture slots!");
		}

		m_TextureSlot = slot;

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

		int gpuFormat = GL_RED;
		int imageFormat = GL_RED;
		switch (channels)
		{
		case 3:
			gpuFormat = GL_RGB;
			imageFormat = GL_RGB;
			break;
		case 4:
			gpuFormat = GL_RGBA8;
			imageFormat = GL_RGBA;
			break;
		}

		uint32_t mipLevel = 0; // 0 - largest, 1 - half of the largest
		if (data != nullptr)
		{
			glTexImage2D(GL_TEXTURE_2D, mipLevel, gpuFormat, width, height, 0, imageFormat, GL_UNSIGNED_BYTE, data);
			// Mipmaps are smaller resolution images of the same image
			// That are used when textures are far-away from camera
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		else
		{
			throw std::runtime_error("Runtime Error: Failed to load image '" + m_Filepath + "'");
		}

		stbi_image_free(data);
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
