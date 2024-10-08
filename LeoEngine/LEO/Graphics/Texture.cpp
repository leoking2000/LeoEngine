#include <glad/glad.h>
#include "Texture.h"
#include "../Utilities/LeoAssert.h"

namespace LEO
{
    static inline u32 GetTypeOf(TextureDimensions dim)
    {
        switch (dim)
        {
        case LEO::TextureDimensions::DIM_1D:
            return GL_TEXTURE_1D;
        case LEO::TextureDimensions::DIM_2D:
            return GL_TEXTURE_2D;
        case LEO::TextureDimensions::DIM_3D:
            return GL_TEXTURE_3D;
        case LEO::TextureDimensions::DIM_2D_ARRAY:
            return GL_TEXTURE_2D_ARRAY;
        }
    }

#define TEXTURE_TYPE GetTypeOf(m_dimensions)

    Texture::Texture(u32 width, u32 height, TextureFormat format, u8* data)
        :
        Texture(TextureDimensions::DIM_2D, { width, height, 0 }, format,
            TextureMinFiltering::MIN_NEAREST, TextureMagFiltering::MAG_NEAREST,
            TextureWrapping::CLAMP_TO_EDGE, TextureWrapping::CLAMP_TO_EDGE, false,
            data)
    {}

    Texture::Texture(
        TextureDimensions dimensions, const glm::uvec3& size, TextureFormat format,
        TextureMinFiltering min_filter, TextureMagFiltering mag_filter,
        TextureWrapping S, TextureWrapping T, bool minimap, u8* data
    )
        :
        m_id(0),
        m_size(size),
        m_dimensions(dimensions),
        m_format(format)
    {
        glGenTextures(1, &m_id);

        IsTexSizeValid(m_size);

        SetFiltering(min_filter, mag_filter);
        SetWrapping(S, T);
        SetImageData(data, format, minimap);
    }

    Texture::Texture(Texture&& other) noexcept
        :
        m_id(other.m_id),
        m_size(other.m_size),
        m_dimensions(other.m_dimensions),
        m_format(other.m_format)
    {
        other.m_id = 0;
    }

    Texture& Texture::operator=(Texture&& other) noexcept
    {
        glDeleteTextures(1, &m_id);
        m_id = other.m_id;
        m_size = other.m_size;
        m_dimensions = other.m_dimensions;
        m_format = other.m_format;

        other.m_id = 0;
        return *this;
    }

    Texture::~Texture()
    {
        glDeleteTextures(1, &m_id);
    }

    void Texture::Bind(u32 slot) const
    {
        glActiveTexture(GL_TEXTURE0 + slot);
        glBindTexture(TEXTURE_TYPE, m_id);
    }

    void Texture::UnBind() const
    {
        glBindTexture(TEXTURE_TYPE, 0);
    }

    void Texture::SetFiltering(TextureMinFiltering min_filter, TextureMagFiltering mag_filter)
    {
        glBindTexture(TEXTURE_TYPE, m_id);

        switch (min_filter)
        {
        case TextureMinFiltering::MIN_NEAREST:
            glTexParameterf(TEXTURE_TYPE, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            break;
        case TextureMinFiltering::MIN_LINEAR:
            glTexParameterf(TEXTURE_TYPE, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            break;
        case TextureMinFiltering::MIN_NEAREST_MIPMAP_NEAREST:
            glTexParameterf(TEXTURE_TYPE, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
            break;
        case TextureMinFiltering::MIN_LINEAR_MIPMAP_NEAREST:
            glTexParameterf(TEXTURE_TYPE, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
            break;
        case TextureMinFiltering::MIN_NEAREST_MIPMAP_LINEAR:
            glTexParameterf(TEXTURE_TYPE, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
            break;
        case TextureMinFiltering::MIN_LINEAR_MIPMAP_LINEAR:
            glTexParameterf(TEXTURE_TYPE, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            break;
        }

        switch (mag_filter)
        {
        case TextureMagFiltering::MAG_NEAREST:
            glTexParameteri(TEXTURE_TYPE, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            break;
        case TextureMagFiltering::MAG_LINEAR:
            glTexParameteri(TEXTURE_TYPE, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            break;
        }

        glBindTexture(TEXTURE_TYPE, 0);
    }

    void Texture::SetWrapping(TextureWrapping S, TextureWrapping T)
    {
        glBindTexture(TEXTURE_TYPE, m_id);

        switch (T)
        {
        case TextureWrapping::REPEAT:
            glTexParameteri(TEXTURE_TYPE, GL_TEXTURE_WRAP_S, GL_REPEAT);
            break;
        case TextureWrapping::MIRRORED_REPEAT:
            glTexParameteri(TEXTURE_TYPE, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
            break;
        case TextureWrapping::CLAMP_TO_EDGE:
            glTexParameteri(TEXTURE_TYPE, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            break;
        case TextureWrapping::CLAMP_TO_BORDER:
            glTexParameteri(TEXTURE_TYPE, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
            break;
        }

        switch (T)
        {
        case TextureWrapping::REPEAT:
            glTexParameteri(TEXTURE_TYPE, GL_TEXTURE_WRAP_T, GL_REPEAT);
            break;
        case TextureWrapping::MIRRORED_REPEAT:
            glTexParameteri(TEXTURE_TYPE, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
            break;
        case TextureWrapping::CLAMP_TO_EDGE:
            glTexParameteri(TEXTURE_TYPE, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            break;
        case TextureWrapping::CLAMP_TO_BORDER:
            glTexParameteri(TEXTURE_TYPE, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
            break;
        }

        glBindTexture(TEXTURE_TYPE, 0);
    }

    void Texture::SetImageData(u8* data, TextureFormat format, bool minimap)
    {
        glBindTexture(TEXTURE_TYPE, m_id);

        GLint internalFormat = 0;
        GLenum color_format  = 0;
        GLenum type = 0;

        switch (format)
        {
        case TextureFormat::RGBA8UB:
            internalFormat = GL_RGBA8;
            color_format = GL_RGBA;
            type = GL_UNSIGNED_BYTE;
            break;
        case TextureFormat::RGBA16F:
            internalFormat = GL_RGBA16F;
            color_format = GL_RGBA;
            type = GL_FLOAT;
            break;
        case TextureFormat::RGBA32F:
            internalFormat = GL_RGBA32F;
            color_format = GL_RGBA;
            type = GL_FLOAT;
            break;
        case TextureFormat::RGBA32UI:
            internalFormat = GL_RGBA32UI;
            color_format = GL_RGBA_INTEGER;
            type = GL_UNSIGNED_INT;
            break;
        case TextureFormat::R32UI:
            internalFormat = GL_R32UI;
            color_format = GL_RGB;
            type = GL_UNSIGNED_INT;
            break;
        case TextureFormat::R32F:
            internalFormat = GL_RED;
            color_format = GL_RGB;
            type = GL_FLOAT;
            break;
        case TextureFormat::DEPTH_COMPONENT32F:
            internalFormat = GL_DEPTH_COMPONENT32F;
            color_format = GL_DEPTH_COMPONENT;
            type = GL_FLOAT;
            break;
        }

        switch (m_dimensions)
        {
        case TextureDimensions::DIM_1D:
            glTexImage1D(TEXTURE_TYPE, 0,
                internalFormat, m_size.x, 0, color_format, type, data);
            break;
        case TextureDimensions::DIM_2D:
            glTexImage2D(TEXTURE_TYPE, 0,
                internalFormat, m_size.x, m_size.y, 0, color_format, type, data);
            break;
        case TextureDimensions::DIM_3D:
            glTexImage3D(TEXTURE_TYPE, 0,
                internalFormat, m_size.x, m_size.y, m_size.z, 0, color_format, type, data);
            break;
        case TextureDimensions::DIM_2D_ARRAY:
            glTexImage3D(TEXTURE_TYPE, 0,
                internalFormat, m_size.x, m_size.y, m_size.z, 0, color_format, type, data);
            break;
        }

        if (minimap)
        {
            glGenerateMipmap(TEXTURE_TYPE);

            //GLfloat anisoSetting = 0.0f; 
            //glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &anisoSetting); 
            //glTexParameterf(TYPE[m_params.dimensions], GL_TEXTURE_MAX_ANISOTROPY_EXT, anisoSetting);
        }

        glBindTexture(TEXTURE_TYPE, 0);
    }

    void Texture::Resize(const glm::uvec3& new_size, TextureFormat format)
    {
        if (IsTexSizeValid(new_size) == false)
        {
            return;
        }

        m_size = new_size;
        SetImageData(nullptr, format, false);
    }

    bool Texture::IsTexSizeValid(const glm::uvec3& new_size) const
    {
        bool isValid = false;

        if (m_dimensions == TextureDimensions::DIM_1D)
        {
            isValid = new_size.x != 0 && new_size.y == 0 && new_size.z == 0;
        }

        if (m_dimensions == TextureDimensions::DIM_2D)
        {
            isValid = new_size.x != 0 && new_size.y != 0 && new_size.z == 0;
        }

        if (m_dimensions == TextureDimensions::DIM_3D || m_dimensions == TextureDimensions::DIM_2D_ARRAY)
        {
            isValid = new_size.x != 0 && new_size.y != 0 && new_size.z != 0;
        }

        LEOCHECK(isValid, "Texture size is invalid").watch(static_cast<u8>(m_dimensions) + 1, "Dimensions")
            LEOWATCH(new_size.x)LEOWATCH(new_size.y)LEOWATCH(new_size.z);

        return isValid;
    }

    u32 Texture::GetType()
    {
        return GetTypeOf(m_dimensions);
    }
}