#include "opengl_texture.h"

#include "stb/stb_image.h"
#include "glad/gl.h"

namespace zeus
{
	OpenGLTexture::OpenGLTexture(const std::string& filepath)
	{
		m_Filepath = filepath;

		stbi_set_flip_vertically_on_load(1);

		glCreateTextures(GL_TEXTURE_2D, 1, &m_TextureID);
		glBindTexture(GL_TEXTURE_2D, m_TextureID);

		bool pixelated = false;
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, pixelated ? GL_NEAREST : GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, pixelated ? GL_NEAREST : GL_LINEAR);

		int width, height, channels;
		stbi_uc* data = stbi_load(filepath.c_str(), &width, &height, &channels, 0);

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
			// That are used when texture is far-away for example
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		else
		{
			throw std::runtime_error("Runtime Error: Failed to load image '" + m_Filepath + "'");
		}

		stbi_image_free(data);
	}

	void OpenGLTexture::Bind(uint32_t slot) const
	{
		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(GL_TEXTURE_2D, m_TextureID);
	}

	void OpenGLTexture::Unbind() const
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}
