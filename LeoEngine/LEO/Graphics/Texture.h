#pragma once
#include <glm/glm.hpp>
#include "../Utilities/Types.h"

namespace LEO
{
    enum TextureDimensions
    {
        DIM_1D = 0,
        DIM_2D = 1,
        DIM_3D = 2,
        DIM_2D_ARRAY = 3
    };

    enum class TextureFormat
    {
        RGBA8UB,
        RGBA16F,
        RGBA32F,
        RGBA32UI,
        R32UI,
        R32F,
        DEPTH_COMPONENT32F
    };

    enum class TextureWrapping
    {
        REPEAT,
        MIRRORED_REPEAT,
        CLAMP_TO_EDGE,
        CLAMP_TO_BORDER
    };

    enum class TextureMinFiltering
    {
        MIN_NEAREST,
        MIN_LINEAR,
        MIN_NEAREST_MIPMAP_NEAREST,
        MIN_LINEAR_MIPMAP_NEAREST,
        MIN_NEAREST_MIPMAP_LINEAR,
        MIN_LINEAR_MIPMAP_LINEAR
    };

    enum class TextureMagFiltering
    {
        MAG_NEAREST,
        MAG_LINEAR
    };

    class Texture
    {
    public:
        Texture(u32 width, u32 height, TextureFormat format = TextureFormat::RGBA8UB, u8* data = nullptr);

        Texture(TextureDimensions dimensions, glm::uvec3 size, TextureFormat format,
            TextureMinFiltering min_filter, TextureMagFiltering mag_filter,
            TextureWrapping S, TextureWrapping T, u8* data
        );

        Texture(const Texture& other) = delete;
        Texture& operator=(const Texture& other) = delete;

        Texture(Texture&& other);
        Texture& operator=(Texture&& other);

        ~Texture();
    public:
        inline u32 ID() const { return m_id; };
        inline TextureDimensions Dimensions() const { return m_params.dimensions; }
        inline glm::uvec3 Size() const { return m_params.size; }
    public:
        void Bind(u32 slot = 0) const;
        void UnBind() const;
    public:
        void SetFiltering(TextureMinFiltering min_filter, TextureMagFiltering mag_filter);
        void SetWrapping(TextureWrapping S, TextureWrapping T);
        void SetImageData(u8* data, TextureFormat format);
        void Resize(const glm::uvec3& new_size);
    private:
        bool IsTexSizeValid(const glm::uvec3& new_size) const;
    private:
        struct TextureParameters
        {
            TextureParameters(TextureDimensions dimensions, glm::uvec3 size,
                TextureFormat format,
                TextureMinFiltering min_filter, TextureMagFiltering mag_filter,
                TextureWrapping S, TextureWrapping T
            )
                :
                dimensions(dimensions),
                size(size),
                format(format),
                min_filter(min_filter),
                mag_filter(mag_filter),
                wrapping_s(S),
                wrapping_t(T)
            {}

            TextureDimensions dimensions;
            glm::uvec3 size;
            TextureFormat format;
            TextureMinFiltering min_filter;
            TextureMagFiltering mag_filter;
            TextureWrapping wrapping_s;
            TextureWrapping wrapping_t;
        };
    private:
        u32 m_id = 0;
        bool m_minimap = false;
        TextureParameters m_params;
    private:
        friend class FrameBuffer;
        static u32 TYPE[4];
    };
}