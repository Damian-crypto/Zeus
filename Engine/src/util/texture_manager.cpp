#include "texture_manager.h"

#include "texture.h"
#include "renderer/opengl/opengl_texture.h"

namespace zeus
{
	std::shared_ptr<TextureManager> TextureManager::s_Instance = nullptr;

	TextureManager::TextureManager()
	{
		if (s_Instance != nullptr)
		{
			throw std::runtime_error("Runtime Error: Trying to create already existing instance!");
		}
	}

	void TextureManager::PutTexture(const TextureInfo& info)
	{
		if (GetTextureMaxSlotsCount() <= m_TextureSlot + 1)
		{
			throw std::runtime_error("Runtime Error: You exceeding your maximum texture slots count!");
		}

		m_Textures[info.Name] = Texture::CreateTexture(info.Filepath);
		m_Textures[info.Name]->Bind(m_TextureSlot);
		m_TextureSlotIndices.push_back(m_TextureSlot);
		m_TextureSlot++;
	}

	void TextureManager::PutSpritesheet(const TextureInfo& info)
	{
		if (GetTextureMaxSlotsCount() <= m_TextureSlot + 1)
		{
			throw std::runtime_error("Runtime Error: You exceeding your maximum texture slots count!");
		}

		m_Textures[info.Name] = Texture::CreateTexture(info.Filepath);
		m_Textures[info.Name]->m_Spritesheet = true;
		m_Textures[info.Name]->m_Cellsize = info.Cellsize;
		m_Textures[info.Name]->m_Pixelated = info.Pixelated;
		m_Textures[info.Name]->Bind(m_TextureSlot);
		m_TextureSlotIndices.push_back(m_TextureSlot);
		m_TextureSlot++;
	}

	std::shared_ptr<Texture> TextureManager::GetTexture(const std::string& name)
	{
		if (m_Textures.find(name) == m_Textures.end())
		{
			throw std::runtime_error("Runtime Error: Accessing invalid texture!");
		}

		const auto& texture = m_Textures[name];
		texture->m_TexCoords[0] = { 0.0f, 0.0f }; // bottom left
		texture->m_TexCoords[1] = { 1.0f, 0.0f }; // bottom right
		texture->m_TexCoords[2] = { 1.0f, 1.0f }; // top right
		texture->m_TexCoords[3] = { 0.0f, 1.0f }; // top left

		return texture;
	}

	std::shared_ptr<Texture> TextureManager::GetSubTexture(const std::string& name, uint32_t xCoord, uint32_t yCoord)
	{
		if (m_Textures.find(name) == m_Textures.end())
		{
			throw std::runtime_error("Runtime Error: Accessing invalid texture!");
		}

		const auto& texture = m_Textures[name];
		// relative tex coord = 1.0 / cellcount
		// cellcount = width / cellsize
		float xPos = texture->m_Cellsize / (float)texture->m_Width;
		float yPos = texture->m_Cellsize / (float)texture->m_Height;

		float x = xCoord < 1.0f ? 1.0f : (float)xCoord;
		float y = yCoord < 1.0f ? 1.0f : (float)yCoord;

		texture->m_TexCoords[0] = { xPos * (x - 1), yPos * (y - 1)	};	// bottom left
		texture->m_TexCoords[1] = { xPos * x,		yPos * (y - 1)	};	// bottom right
		texture->m_TexCoords[2] = { xPos * x,		yPos * y		};	// top right
		texture->m_TexCoords[3] = { xPos * (x - 1), yPos * y		};	// top left

		return texture;
	}

	void TextureManager::RemoveTexture(const std::string& name)
	{
		if (m_Textures.find(name) == m_Textures.end())
		{
			throw std::runtime_error("Runtime Error: Trying to remove invalid texture!");
		}

		m_TextureSlotIndices.erase(m_TextureSlotIndices.begin() + m_Textures[name]->GetTextureSlot());
		m_Textures[name]->Unbind();
		m_Textures[name].reset();
		m_Textures.erase(name);
		m_TextureSlot--;
	}

	std::vector<int>& TextureManager::GetTextureSlotData()
	{
		return m_TextureSlotIndices;
	}

	std::shared_ptr<TextureManager> TextureManager::GetInstance()
	{
		if (s_Instance == nullptr)
		{
			s_Instance = std::make_shared<TextureManager>();
		}

		return s_Instance;
	}

	uint32_t TextureManager::GetTextureMaxSlotsCount()
	{
		return OpenGLTexture::GetTextureSlotsCount();
	}
}
