#pragma once

#include <memory>
#include <vector>
#include <string>

#include "glm/glm.hpp"

namespace zeus
{
	class Texture
	{
	protected:
		uint32_t m_TextureID = 0;
		uint32_t m_Width = 1280;
		uint32_t m_Height = 720;
		uint32_t m_TextureSlot = 0;
		uint32_t m_Cellsize = 16;
		bool m_Spritesheet = false;
		bool m_Pixelated = false;

		std::vector<glm::vec2> m_TexCoords{ 4, { 1.0f, 1.0f } };

		std::string m_Filepath{};

		friend class TextureManager;

	public:
		static std::shared_ptr<Texture> CreateTexture(const std::string& filepath);

		virtual void Bind(uint32_t slot = 0) = 0;
		virtual void Unbind() const = 0;
		virtual void Activate() = 0;

		uint32_t GetWidth() const { return m_Width; }
		uint32_t GetHeight() const { return m_Height; }
		const std::vector<glm::vec2>& GetTexCoords() const { return m_TexCoords; }
		uint32_t GetTextureID() const { return m_TextureID; }
		uint32_t GetTextureSlot() const { return m_TextureSlot; }
		const std::string& GetFilePath() { return m_Filepath; }
	};
}
