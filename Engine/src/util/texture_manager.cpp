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

	std::shared_ptr<Texture> TextureManager::GetTexture(std::string_view name)
	{
		if (m_Textures[name.data()] == nullptr)
		{
			throw std::runtime_error("Runtime Error: Accessing invalid texture!");
		}

#if 0
		const auto& texture = m_Textures[name];
		texture->m_TexCoords[0] = { 0.0f, 0.0f }; // bottom left
		texture->m_TexCoords[1] = { 1.0f, 0.0f }; // bottom right
		texture->m_TexCoords[2] = { 1.0f, 1.0f }; // top right
		texture->m_TexCoords[3] = { 0.0f, 1.0f }; // top left
#endif

		return m_Textures[name.data()];
	}

	std::shared_ptr<SubTexture> TextureManager::GetSubTexture(std::string_view name, uint32_t xCoord, uint32_t yCoord)
	{
		if (m_Textures[name.data()] == nullptr)
		{
			throw std::runtime_error("Runtime Error: Accessing invalid texture!");
		}

		std::shared_ptr<SubTexture> subTexture = nullptr;

		uint32_t texIdx = m_Textures[name.data()]->m_TextureID * 10000 + xCoord * 1000 + yCoord * 100;
		if (m_SubTextures[texIdx] != nullptr)
		{
			return m_SubTextures[texIdx];
		}
		else
		{
			auto& texture = m_Textures[name.data()];
			m_SubTextures[texIdx] = SubTexture::CreateSubTexture(texture, xCoord, yCoord, texture->GetCellSize());
			subTexture = m_SubTextures[texIdx];
		}

		return subTexture;
	}

	void TextureManager::RemoveTexture(std::string_view name)
	{
		if (m_Textures[name.data()] == nullptr)
		{
			throw std::runtime_error("Runtime Error: Trying to remove invalid texture!");
		}

		m_TextureSlotIndices.erase(m_TextureSlotIndices.begin() + m_Textures[name.data()]->GetTextureSlot());
		uint32_t texId = m_Textures[name.data()]->m_TextureID;
		m_Textures[name.data()]->Unbind();
		m_Textures[name.data()].reset();
		m_Textures.erase(name.data());
		m_TextureSlot--;

		for (auto it = m_SubTextures.begin(); it != m_SubTextures.end(); it++)
		{
			if (it->first / 10000 == texId)
			{
				m_SubTextures.erase(it->first);
			}
		}
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
