#pragma once

#include "../core.h"

namespace zeus
{
	class Texture;
	class SubTexture;

	struct TextureInfo
	{
		const char* Name;
		std::string Filepath;
		uint32_t Cellsize = 16;
		bool Pixelated = false;
	};

	class TextureManager
	{
	private:
		std::unordered_map<const char*, std::shared_ptr<Texture>> m_Textures;
		std::unordered_map<uint32_t, std::shared_ptr<SubTexture>> m_SubTextures;
		uint32_t m_TextureSlot = 0;
		std::vector<int> m_TextureSlotIndices;

		static std::shared_ptr<TextureManager> s_Instance;

	public:
		TextureManager();

		void PutTexture(const TextureInfo& info);
		void PutSpritesheet(const TextureInfo& info);
		std::shared_ptr<Texture> GetTexture(std::string_view name);
		std::shared_ptr<SubTexture> GetSubTexture(std::string_view name, uint32_t xCoord = 0, uint32_t yCoord = 0);
		void RemoveTexture(std::string_view name);
		std::vector<int>& GetTextureSlotData();

		static std::shared_ptr<TextureManager> GetInstance();
		static uint32_t GetTextureMaxSlotsCount();
	};
}
