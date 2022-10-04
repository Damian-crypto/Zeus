#pragma once

#include "../core.h"

namespace zeus
{
	class Texture;

	struct TextureInfo
	{
		std::string Name;
		std::string Filepath;
		uint32_t Cellsize = 16;
		bool Pixelated = false;
	};

	class TextureManager
	{
	private:
		std::unordered_map<std::string, std::shared_ptr<Texture>> m_Textures;
		uint32_t m_TextureSlot = 0;
		std::vector<int> m_TextureSlotIndices;

		static std::shared_ptr<TextureManager> s_Instance;

	public:
		TextureManager();

		void PutTexture(const TextureInfo& info);
		void PutSpritesheet(const TextureInfo& info);
		std::shared_ptr<Texture> GetTexture(const std::string& name);
		std::shared_ptr<Texture> GetSubTexture(const std::string& name, uint32_t xCoord = 0, uint32_t yCoord = 0);
		void RemoveTexture(const std::string& name);
		std::vector<int>& GetTextureSlotData();

		static std::shared_ptr<TextureManager> GetInstance();
		static uint32_t GetTextureMaxSlotsCount();
	};
}
