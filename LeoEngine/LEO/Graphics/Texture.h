#pragma once
#include <glm/glm.hpp>
#include "../Utilities/Types.h"

namespace LEO
{
    enum class TextureDimensions : u8
    {
        DIM_1D = 0,
        DIM_2D = 1,
        DIM_3D = 2,
        DIM_2D_ARRAY = 3
    };

    enum class TextureFormat : u8
    {
        RGBA8UB,
        RGBA16F,
        RGBA32F,
        RGBA32UI,
        R32UI,
        R32F,
        DEPTH_COMPONENT32F
    };

    enum class TextureWrapping : u8
    {
        REPEAT,
        MIRRORED_REPEAT,
        CLAMP_TO_EDGE,
        CLAMP_TO_BORDER
    };

    enum class TextureMinFiltering : u8
    {
        MIN_NEAREST,
        MIN_LINEAR,
        MIN_NEAREST_MIPMAP_NEAREST,
        MIN_LINEAR_MIPMAP_NEAREST,
        MIN_NEAREST_MIPMAP_LINEAR,
        MIN_LINEAR_MIPMAP_LINEAR
    };

    enum class TextureMagFiltering : u8
    {
        MAG_NEAREST,
        MAG_LINEAR
    };

    class Texture
    {
    public:
        Texture(u32 width, u32 height, TextureFormat format = TextureFormat::RGBA8UB, u8* data = nullptr);

        Texture(TextureDimensions dimensions,const glm::uvec3& size, TextureFormat format,
            TextureMinFiltering min_filter, TextureMagFiltering mag_filter,
            TextureWrapping S, TextureWrapping T, bool minimap, u8* data
        );

        Texture(const Texture& other) = delete;
        Texture& operator=(const Texture& other) = delete;

        Texture(Texture&& other) noexcept;
        Texture& operator=(Texture&& other) noexcept;

        ~Texture();
    public:
        inline u32 ID() const { return m_id; };
        inline glm::uvec3 Size() const { return m_size; }
        inline TextureDimensions Dimensions() const { return m_dimensions; }
        inline TextureFormat Format() const { return m_format; }
    public:
        void Bind(u32 slot = 0) const;
        void UnBind() const;
    public:
        void SetFiltering(TextureMinFiltering min_filter, TextureMagFiltering mag_filter);
        void SetWrapping(TextureWrapping S, TextureWrapping T);
        void SetImageData(u8* data, TextureFormat format, bool minimap = false);
        void Resize(const glm::uvec3& new_size, TextureFormat format);
    private:
        bool IsTexSizeValid(const glm::uvec3& new_size) const;
    private:
        u32 m_id = 0u; 
        glm::uvec3 m_size = glm::uvec3(0u);
        TextureDimensions m_dimensions = TextureDimensions::DIM_1D;
        TextureFormat m_format = TextureFormat::RGBA8UB;
    private:
        Texture() = default;
        friend class FrameBuffer;
        friend class AssetManager;
    private:
        u32 GetType();
    };
}