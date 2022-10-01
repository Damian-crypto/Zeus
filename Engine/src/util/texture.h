#pragma once

#include "..\core.h"

namespace zeus
{
	class Texture
	{
	protected:
		uint32_t m_TextureID = 0;
		uint32_t m_Width = 1280;
		uint32_t m_Height = 720;
		uint32_t m_TextureSlot = 0;

		std::string m_Filepath{};

	public:
		static std::shared_ptr<Texture> CreateTexture(const std::string& filepath);

		virtual void Bind(uint32_t slot = 0) = 0;
		virtual void Unbind() const = 0;
		virtual uint32_t GetTextureSlotsCount() const = 0;

		uint32_t GetWidth() const { return m_Width; }
		uint32_t GetHeight() const { return m_Height; }
		uint32_t GetTextureID() const { return m_TextureID; }
		uint32_t GetTextureSlot() const { return m_TextureSlot; }
		const std::string& GetFilePath() { return m_Filepath; }
	};
}
