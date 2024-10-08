#pragma once
#include <optional>
#include "Texture.h"

namespace LEO
{
	enum class FrameBufferMode : u8
	{
		ColorAttachment,
		Layered,
		Texture3D
	};

#define MAX_COLOR_ATTACHMENTS 8

	class FrameBuffer
	{
	public:
		// for 2D Texture
		FrameBuffer(u32 width, u32 height, u32 colorAttachmentCount = 1,
			TextureFormat format = TextureFormat::RGBA32F, FrameBufferMode fbt = FrameBufferMode::ColorAttachment,
			TextureMinFiltering min_filter = TextureMinFiltering::MIN_NEAREST,
			TextureMagFiltering mag_filter = TextureMagFiltering::MAG_NEAREST);

		// for 3D Texture
		FrameBuffer(u32 width, u32 height, u32 depth, u32 colorAttachmentCount = 1,
			TextureFormat format = TextureFormat::RGBA32F,
			TextureMinFiltering min_filter = TextureMinFiltering::MIN_NEAREST,
			TextureMagFiltering mag_filter = TextureMagFiltering::MAG_NEAREST);
	public:
		FrameBuffer(const FrameBuffer&) = delete;
		FrameBuffer& operator=(const FrameBuffer&) = delete;
		FrameBuffer(FrameBuffer&& other) noexcept;
		FrameBuffer& operator=(FrameBuffer&& other) noexcept;
	public:
		~FrameBuffer();
	public:
		void Bind() const;
		void UnBind() const;
	public:
		void BindColorTexture(u8 index, u32 slot) const;
		void BindDepthTexture(u32 slot) const;
		void UnBindColorTexture(u8 index) const;
		void UnBindDepthTexture() const;
	public:
		void Resize(u32 width, u32 height);
	public:
		inline u8  NumberOfColorAttachments() const { return m_num_color_attachments; };
		inline u32 Width()  const { return m_size.x; }
		inline u32 Height() const { return m_size.y; }
		inline u32 Depth()  const { return m_size.z; }
	private:
		void InitColorAttachmentMode(u32 width, u32 height, u32 colorAttachmentCount,
			TextureMinFiltering min_filter, TextureMagFiltering mag_filter, TextureFormat format);
		void InitLayered(u32 width, u32 height, u32 colorAttachmentCount,
			TextureMinFiltering min_filter, TextureMagFiltering mag_filter, TextureFormat format);
		void InitTexture3D(u32 width, u32 height, u32 colorAttachmentCount,
			TextureMinFiltering min_filter, TextureMagFiltering mag_filter, TextureFormat format);
	private:
		void InitColorAttachmentMode3D(u32 width, u32 height, u32 depth, u32 colorAttachmentCount,
			TextureMinFiltering min_filter, TextureMagFiltering mag_filter, TextureFormat format);
	private:
		u32 m_id = 0;
		glm::uvec3 m_size = glm::uvec3(0);
		u32 m_num_color_attachments = 0;
		Texture m_color_attachments[MAX_COLOR_ATTACHMENTS] = {};
		TextureFormat m_format = TextureFormat::RGBA32F;
		std::optional<Texture> m_depth_texture;
	};
}
