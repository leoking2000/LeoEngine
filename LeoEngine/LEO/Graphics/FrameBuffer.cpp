#include <glad/glad.h>
#include "../Utilities/Log.h"
#include "FrameBuffer.h"


static u32 CheckColorAttachmentNumber(u32 colorAttachmentCount);
static u32 CheckFramebufferStatus(u32 framebuffer_object);

namespace LEO
{
    FrameBuffer::FrameBuffer(u32 width, u32 height, u32 colorAttachmentCount, 
        TextureFormat format, FrameBufferMode fbt, 
        TextureMinFiltering min_filter, TextureMagFiltering mag_filter)
    {
        switch (fbt)
        {
        case LEO::FrameBufferMode::ColorAttachment:
            InitColorAttachmentMode(width, height, colorAttachmentCount, min_filter, mag_filter, format);
            break;
        case LEO::FrameBufferMode::Layered:
            InitLayered(width, height, colorAttachmentCount, min_filter, mag_filter, format);
            break;
        case LEO::FrameBufferMode::Texture3D:
            InitTexture3D(width, height, colorAttachmentCount, min_filter, mag_filter, format);
            break;
        }
    }

    FrameBuffer::FrameBuffer(u32 width, u32 height, u32 depth, u32 colorAttachmentCount, TextureFormat format, TextureMinFiltering min_filter, TextureMagFiltering mag_filter)
    {
        InitColorAttachmentMode3D(width, height, depth, colorAttachmentCount, min_filter, mag_filter, format);
    }

	FrameBuffer::FrameBuffer(FrameBuffer&& other) noexcept
		:
        m_id(other.m_id),
		m_size(other.m_size),
        m_num_color_attachments(other.m_num_color_attachments),
        m_format(other.m_format),
        m_depth_texture(std::move(other.m_depth_texture))
	{
		other.m_id = 0;

        for (i32 i = 0; i < other.m_num_color_attachments; i++)
        {
            m_color_attachments[i] = std::move(other.m_color_attachments[i]);
        }
	}

    FrameBuffer& FrameBuffer::operator=(FrameBuffer&& other) noexcept
    {
        for (i32 i = 0; i < m_num_color_attachments; i++)
        {
            m_color_attachments[i].~Texture();
        }
        if (m_depth_texture)
        {
            glDeleteTextures(1, &m_depth_texture->m_id);
            m_depth_texture->m_id = 0;
        }
        glDeleteFramebuffers(1, &m_id);

        m_id = other.m_id;
        m_size = other.m_size;
        m_format = other.m_format;
        m_depth_texture = std::move(other.m_depth_texture);
        other.m_id = 0;

        m_num_color_attachments = other.m_num_color_attachments;
        for (i32 i = 0; i < other.m_num_color_attachments; i++)
        {
            m_color_attachments[i] = std::move(other.m_color_attachments[i]);
        }

        return *this;
    }

	LEO::FrameBuffer::~FrameBuffer()
	{
        for (i32 i = 0; i < m_num_color_attachments; i++)
        {
            m_color_attachments[i].~Texture();
        }

		if (m_depth_texture)
		{
			glDeleteTextures(1, &m_depth_texture->m_id);
			m_depth_texture->m_id = 0;
		}

		glDeleteFramebuffers(1, &m_id);
	}

    void FrameBuffer::Bind() const
    {
        glBindFramebuffer(GL_FRAMEBUFFER, m_id);

        constexpr static GLenum drawbuffers[MAX_COLOR_ATTACHMENTS] = {
            GL_COLOR_ATTACHMENT0,
            GL_COLOR_ATTACHMENT1,
            GL_COLOR_ATTACHMENT2,
            GL_COLOR_ATTACHMENT3,
            GL_COLOR_ATTACHMENT4,
            GL_COLOR_ATTACHMENT5,
            GL_COLOR_ATTACHMENT6,
            GL_COLOR_ATTACHMENT7
        };

        glDrawBuffers((GLsizei)m_num_color_attachments, drawbuffers);
    }

    void FrameBuffer::UnBind() const
    {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void FrameBuffer::BindColorTexture(u8 index, u32 slot) const
    {
        m_color_attachments[index].Bind(slot);
    }

    void FrameBuffer::BindDepthTexture(u32 slot) const
    {
        if (m_depth_texture) { m_depth_texture->Bind(slot); }
    }

    void FrameBuffer::UnBindColorTexture(u8 index) const
    {
        m_color_attachments[index].Bind();
    }

    void FrameBuffer::UnBindDepthTexture() const
    {
        if (m_depth_texture) { m_depth_texture->UnBind(); }
    }

    void FrameBuffer::Resize(u32 width, u32 height)
    {
        m_size = glm::vec3(width, height, 0);

        if (m_depth_texture)
        {
            m_depth_texture->Resize(m_size, TextureFormat::DEPTH_COMPONENT32F);
        }

        for (i32 i = 0; i < m_num_color_attachments; i++)
        {
            m_color_attachments[i].Resize(m_size, m_format);
        }

        LEOASSERT(CheckFramebufferStatus(m_id) == GL_FRAMEBUFFER_COMPLETE, "Frame buffer error");
    }

    u8 FrameBuffer::NumberOfColorAttachments() const
    {
        return (u8)m_num_color_attachments;
    }

    void FrameBuffer::InitColorAttachmentMode(u32 width, u32 height, u32 colorAttachmentCount,
        TextureMinFiltering min_filter, TextureMagFiltering mag_filter, TextureFormat format)
    {
        glGenFramebuffers(1, &m_id);
        glBindFramebuffer(GL_FRAMEBUFFER, m_id);

        m_size = glm::vec3(width, height, 0);
        m_num_color_attachments = CheckColorAttachmentNumber(colorAttachmentCount);
        m_format = format;

        for (u8 i = 0; i < m_num_color_attachments; i++)
        {
            m_color_attachments[i] = Texture(TextureDimensions::DIM_2D, m_size, m_format, min_filter, mag_filter,
                TextureWrapping::CLAMP_TO_EDGE, TextureWrapping::CLAMP_TO_EDGE, false, nullptr);

            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, 
                m_color_attachments[i].GetType(), m_color_attachments[i].ID(), 0);
        }

        m_depth_texture = Texture(TextureDimensions::DIM_2D, m_size, TextureFormat::DEPTH_COMPONENT32F,
            TextureMinFiltering::MIN_NEAREST, TextureMagFiltering::MAG_NEAREST,
            TextureWrapping::CLAMP_TO_EDGE, TextureWrapping::CLAMP_TO_EDGE, false, 0);

        glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, m_depth_texture->ID(), 0);

        //LOGDEBUG("FrameBuffer({}): Created with size ({},{}) and {} color attachments", m_id, m_width, m_height, colorAttachmentCount);
        LEOASSERT(CheckFramebufferStatus(m_id) == GL_FRAMEBUFFER_COMPLETE, "Frame buffer error");
    }

    void FrameBuffer::InitLayered(u32 width, u32 height, u32 colorAttachmentCount,
        TextureMinFiltering min_filter, TextureMagFiltering mag_filter, TextureFormat format)
    {
        glGenFramebuffers(1, &m_id);
        glBindFramebuffer(GL_FRAMEBUFFER, m_id);

        m_size = glm::vec3(width, height, CheckColorAttachmentNumber(colorAttachmentCount));
        m_num_color_attachments = 1;
        m_format = format;

        m_color_attachments[0] = Texture(TextureDimensions::DIM_2D_ARRAY, m_size, m_format,
            min_filter, mag_filter, TextureWrapping::CLAMP_TO_EDGE, TextureWrapping::CLAMP_TO_EDGE, 
            false, (u8*)0u);

        glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, m_color_attachments[0].ID(), 0);

        //LOGDEBUG("FrameBuffer({}): Created with size ({},{}) and {} color attachments Layered", m_id, m_width, m_height, colorAttachmentCount);
        LEOASSERT(CheckFramebufferStatus(m_id) == GL_FRAMEBUFFER_COMPLETE, "Frame buffer error");
    }

    void FrameBuffer::InitTexture3D(u32 width, u32 height, u32 colorAttachmentCount,
        TextureMinFiltering min_filter, TextureMagFiltering mag_filter, TextureFormat format)
    {
        glGenFramebuffers(1, &m_id);
        glBindFramebuffer(GL_FRAMEBUFFER, m_id);

        m_size = glm::vec3(width, height, colorAttachmentCount);
        m_num_color_attachments = 1;
        m_format = format;

        m_color_attachments[0] = Texture(TextureDimensions::DIM_3D, m_size, m_format, min_filter, mag_filter,
            TextureWrapping::CLAMP_TO_EDGE, TextureWrapping::CLAMP_TO_EDGE, false, (u8*)0u);

        glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, m_color_attachments[0].ID(), 0);

        //LOGDEBUG("FrameBuffer({}): Created with size ({},{},{})", m_id, m_width, m_height, m_depth);
        LEOASSERT(CheckFramebufferStatus(m_id) == GL_FRAMEBUFFER_COMPLETE, "Frame buffer error");
    }

    void FrameBuffer::InitColorAttachmentMode3D(u32 width, u32 height, u32 depth, u32 colorAttachmentCount,
        TextureMinFiltering min_filter, TextureMagFiltering mag_filter, TextureFormat format)
    {
        glGenFramebuffers(1, &m_id);
        glBindFramebuffer(GL_FRAMEBUFFER, m_id);

        m_size = glm::vec3(width, height, depth);
        m_num_color_attachments = CheckColorAttachmentNumber(colorAttachmentCount);
        m_format = format;

        for (u8 i = 0; i < m_num_color_attachments; i++)
        {
            m_color_attachments[i] = Texture(TextureDimensions::DIM_3D, m_size, m_format, min_filter, mag_filter,
                TextureWrapping::CLAMP_TO_EDGE, TextureWrapping::CLAMP_TO_EDGE, false, (u8*)0u
            );

            glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, m_color_attachments[i].ID(), 0);
        }

        //LOGDEBUG("FrameBuffer({}): Created with size ({},{},{}) and {} color attachments", m_id, m_width, m_height, m_depth, colorAttachmentCount);
        LEOASSERT(CheckFramebufferStatus(m_id) == GL_FRAMEBUFFER_COMPLETE, "Frame buffer error");
    }
}

u32 CheckColorAttachmentNumber(u32 colorAttachmentCount)
{
    if (colorAttachmentCount < MAX_COLOR_ATTACHMENTS)
    {
        return colorAttachmentCount;
    }

    return MAX_COLOR_ATTACHMENTS;
}

u32 CheckFramebufferStatus(u32 framebuffer_object)
{
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer_object);
    u32 status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

    if (status != GL_FRAMEBUFFER_COMPLETE)
    {
        LOGERROR("FrameBuffer({}): glCheckFramebufferStatus: error {}", framebuffer_object, status);
        switch (status)
        {
        case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
            LOGERROR("Incomplete attatchement");
            break;
        case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
            LOGERROR("Incomplete missing attachment");
            break;
        case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
            LOGERROR("Incomplete draw buffer");
            break;
        case GL_FRAMEBUFFER_UNSUPPORTED:
            LOGERROR("Unsupported");
            break;
        case GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS:
            LOGERROR("Incomplete layer targets");
            break;
        default:
            LOGERROR("Default error");
            break;
        }
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    return status;
}


