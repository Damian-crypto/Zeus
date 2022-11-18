#pragma once

namespace zeus
{
	enum class ColorAttachment;
	class Texture;
	class TextureManager;

	struct FramebufferInfo
	{
		std::vector<ColorAttachment> Attachments{};
	};

	class Framebuffer
	{
	protected:
		uint32_t m_FramebufferID = 0;
		std::vector<std::shared_ptr<Texture>> m_Attachments;
		FramebufferInfo m_FramebufferInfo{};

	public:
		static std::shared_ptr<Framebuffer> CreateFramebuffer(const FramebufferInfo& info);

		virtual void Bind(const std::shared_ptr<TextureManager>& textureManager) = 0;
		virtual void Unbind() const = 0;
		virtual void Activate() const = 0;
		virtual int ReadID(uint32_t attachment, float xPos, float yPos) const = 0;
		const std::vector<std::shared_ptr<Texture>>& GetAttachedTextures() const { return m_Attachments; }

		static uint32_t GetMaxAttachments();
	};
}
