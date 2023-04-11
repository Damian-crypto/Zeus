#pragma once

#include <unordered_map>
#include <string_view>

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
		std::unordered_map<std::string, std::shared_ptr<Texture>> m_Textures;
		std::unordered_map<uint32_t, std::shared_ptr<SubTexture>> m_SubTextures;
		uint32_t m_TextureSlot = 0;
		std::vector<int> m_TextureSlotIndices;

		static std::shared_ptr<TextureManager> s_Instance;

	public:
		TextureManager();

		void PutTexture(const TextureInfo& info);
		void PutTexture(const std::string& name, std::shared_ptr<Texture> tex);
		void PutSpritesheet(const TextureInfo& info);
		
		const std::unordered_map<std::string, std::shared_ptr<Texture>>& GetTextureMap() const { return m_Textures; }
		std::shared_ptr<Texture> GetTexture(std::string_view name);
		std::shared_ptr<SubTexture> GetSubTexture(std::string_view name, uint32_t xCoord = 0, uint32_t yCoord = 0);
		std::vector<int>& GetTextureSlotData();
		uint32_t GetTextureSlotsUsed() const;

		void RemoveTexture(std::string_view name);

		static std::shared_ptr<TextureManager> GetInstance();
		static uint32_t GetTextureMaxSlotsCount();
	};
}
