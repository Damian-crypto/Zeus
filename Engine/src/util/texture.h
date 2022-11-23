#pragma once

#include <memory>
#include <vector>
#include <string>

#include "glm/glm.hpp"

namespace zeus
{
	enum class ColorAttachment
	{
		RGB = 0,
		RGBA,
		INT
	};

	class Texture
	{
	protected:
		uint32_t m_TextureID	= 0;
		uint32_t m_Width		= 1280;
		uint32_t m_Height		= 720;
		uint32_t m_TextureSlot	= 0;
		uint32_t m_Cellsize		= 16;
		int m_InternalFormat	= 0;
		int m_PixelFormat		= 0;
		bool m_Spritesheet		= false;
		bool m_Pixelated		= false;

		std::vector<glm::vec2> m_TexCoords = {
			{ 0.0f, 0.0f },
			{ 1.0f, 0.0f },
			{ 1.0f, 1.0f },
			{ 0.0f, 1.0f }
		};

		std::string m_Filepath{};

		friend class TextureManager;

	public:
		static std::shared_ptr<Texture> CreateTexture(std::string_view filepath);
		static std::shared_ptr<Texture> CreateTexture();

		virtual void Bind(uint32_t slot = 0) = 0;
		virtual void Unbind() const = 0;
		virtual void Activate() = 0;

		uint32_t GetWidth() const { return m_Width; }
		uint32_t GetHeight() const { return m_Height; }
		const std::vector<glm::vec2>& GetTexCoords() const { return m_TexCoords; }
		uint32_t GetTextureID() const { return m_TextureID; }
		uint32_t GetTextureSlot() const { return m_TextureSlot; }
		uint32_t GetCellSize() const { return m_Cellsize; }
		const std::string& GetFilePath() const { return m_Filepath; }

		void SetInternalFormat(ColorAttachment colorFormat);
	};

	class SubTexture
	{
	private:
		std::shared_ptr<Texture> m_ParentTexture;
		std::vector<glm::vec2> m_TexCoords{ 4, { 1.0f, 1.0f } };
		uint32_t m_Cellsize{ 0u };

	public:
		static std::shared_ptr<SubTexture> CreateSubTexture(std::shared_ptr<Texture> texture, uint32_t xCoord, uint32_t yCoord, uint32_t cellsize = 16);

		void Bind(uint32_t slot = 0) { m_ParentTexture->Bind(slot); }
		void Unbind() const { m_ParentTexture->Unbind(); }
		void Activate() { m_ParentTexture->Activate(); }

		const std::vector<glm::vec2>& GetTexCoords() const { return m_TexCoords; }
		uint32_t GetTextureID() const { return m_ParentTexture->GetTextureID(); }
		uint32_t GetTextureSlot() const { return  m_ParentTexture->GetTextureSlot(); }
		uint32_t GetCellSize() const { return m_Cellsize; }
		const std::string& GetFilePath() const { return  m_ParentTexture->GetFilePath(); }
	};
}
