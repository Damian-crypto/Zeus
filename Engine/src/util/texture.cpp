#include "texture.h"

#include "renderer/opengl/opengl_texture.h"
#include "texture_manager.h"

namespace zeus
{
	std::shared_ptr<Texture> Texture::CreateTexture(const std::string& filepath)
	{
		return std::make_shared<OpenGLTexture>(filepath);
	}
}
