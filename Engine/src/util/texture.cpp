#include "texture.h"

#include "renderer/opengl/opengl_texture.h"
#include "texture_manager.h"

namespace zeus
{
	std::shared_ptr<Texture> Texture::CreateTexture(std::string_view filepath)
	{
		return std::make_shared<OpenGLTexture>(filepath.data());
	}

	std::shared_ptr<SubTexture> SubTexture::CreateSubTexture(std::shared_ptr<Texture> texture, uint32_t xCoord, uint32_t yCoord, uint32_t cellsize)
	{
		std::shared_ptr<SubTexture> subTex = std::make_shared<SubTexture>();
		subTex->m_Cellsize = cellsize;
		subTex->m_ParentTexture = texture;

		const auto& parentTex = subTex->m_ParentTexture;
		const auto& cellSize = parentTex->GetCellSize();
		float xPos = cellSize / (float)parentTex->GetWidth();
		float yPos = cellSize / (float)parentTex->GetHeight();

		float x = xCoord < 1.0f ? 1.0f : (float)xCoord;
		float y = yCoord < 1.0f ? 1.0f : (float)yCoord;

		auto& texCoords = subTex->m_TexCoords;
		texCoords[0] = { xPos * (x - 1),	yPos * (y - 1)	};	// bottom left
		texCoords[1] = { xPos * x,			yPos * (y - 1)	};	// bottom right
		texCoords[2] = { xPos * x,			yPos * y		};	// top right
		texCoords[3] = { xPos * (x - 1),	yPos * y		};	// top left

		return subTex;
	}
}
