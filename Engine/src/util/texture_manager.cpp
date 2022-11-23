#include "corepch.h"
#include "texture_manager.h"

#include "texture.h"
#include "renderer/opengl/opengl_texture.h"
#include "util/logger.h"

namespace zeus
{
	std::shared_ptr<TextureManager> TextureManager::s_Instance = nullptr;

	TextureManager::TextureManager()
	{
		if (s_Instance != nullptr)
		{
			LOG_ENGINE(Error, "Runtime Error: Trying to create already existing instance!");
		}
	}

	void TextureManager::PutTexture(const TextureInfo& info)
	{
		if (GetTextureMaxSlotsCount() <= m_TextureSlot + 1)
		{
			LOG_ENGINE(Error, "Runtime Error: You exceeding your maximum texture slots count!");
		}

		m_Textures[info.Name] = Texture::CreateTexture(info.Filepath);
		m_Textures[info.Name]->Bind(m_TextureSlot);
		m_TextureSlotIndices.push_back(m_TextureSlot);
		m_TextureSlot++;
	}

	void TextureManager::PutTexture(const std::string& name, std::shared_ptr<Texture> tex)
	{
		if (GetTextureMaxSlotsCount() <= m_TextureSlot + 1)
		{
			LOG_ENGINE(Error, "Runtime Error: You exceeding your maximum texture slots count!");
		}

		m_Textures[name] = tex;
		m_Textures[name]->Bind(m_TextureSlot);
		m_TextureSlotIndices.push_back(m_TextureSlot);
		m_TextureSlot++;
	}

	void TextureManager::PutSpritesheet(const TextureInfo& info)
	{
		if (GetTextureMaxSlotsCount() <= m_TextureSlot + 1)
		{
			LOG_ENGINE(Error, "Runtime Error: You are exceeding your maximum texture slots count!");
		}

		m_Textures[info.Name] = Texture::CreateTexture(info.Filepath);
		m_Textures[info.Name]->m_Spritesheet = true;
		m_Textures[info.Name]->m_Cellsize = info.Cellsize;
		m_Textures[info.Name]->m_Pixelated = info.Pixelated;
		m_Textures[info.Name]->Bind(m_TextureSlot);
		m_TextureSlotIndices.push_back(m_TextureSlot);
		m_TextureSlot++;
	}

	std::string nameBuffer(100, '0');
	std::shared_ptr<Texture> TextureManager::GetTexture(std::string_view name)
	{
		nameBuffer.resize(name.size());
		nameBuffer = name;
		if (m_Textures[nameBuffer] == nullptr)
		{
			LOG_ENGINE(Error, "Runtime Error: Accessing invalid texture!");
		}

		return m_Textures[nameBuffer];
	}

	std::shared_ptr<SubTexture> TextureManager::GetSubTexture(std::string_view name, uint32_t xCoord, uint32_t yCoord)
	{
		nameBuffer.resize(name.size());
		nameBuffer = name;
		if (m_Textures[nameBuffer] == nullptr)
		{
			LOG_ENGINE(Error, "Runtime Error: Accessing invalid texture!");
		}

		if (xCoord != -1 && yCoord != -1)
		{
			// Texture id system
			// Assumption: texture id is unique for each texture
			// e.g. we need texture with #: 305040
			// texID = 3, xCoord = 5, yCoord = 4
			// 300000 : texID  * 100000
			// 005000 : xCoord * 001000
			// 000040 : yCoord * 000010
			// This id system can be extended as you wish
			uint32_t texIdx = m_Textures[nameBuffer]->m_TextureID * 100000 + xCoord * 1000 + yCoord * 10;
			if (m_SubTextures.find(texIdx) != m_SubTextures.end())
			{
				const auto& res = m_SubTextures[texIdx];
				return res;
			}
			else
			{
				auto& texture = m_Textures[nameBuffer];
				m_SubTextures[texIdx] = SubTexture::CreateSubTexture(texture, xCoord, yCoord, texture->GetCellSize());
				return m_SubTextures[texIdx];
			}
		}

		return nullptr;
	}

	void TextureManager::RemoveTexture(std::string_view name)
	{
		if (m_Textures[name.data()] == nullptr)
		{
			LOG_ENGINE(Error, "Runtime Error: Trying to remove invalid texture!");
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

	uint32_t TextureManager::GetTextureSlotsUsed() const
	{
		return m_TextureSlot;
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
