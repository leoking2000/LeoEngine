#include "Random.h"

namespace LEO
{
    Random::Random()
    {
        m_randomEngine.seed(std::random_device()());
    }

    Random& Random::Get()
    {
        static Random r;
        return r;
    }

    u32 Random::UInt()
    {
        Random& r = Get();
        return r.m_distribution(r.m_randomEngine);
    }

    u32 Random::UInt(u32 min, u32 max)
    {
        return min + (Random::UInt() % (max - min + 1));
    }

    f32 Random::Float()
    {
        return (f32)Random::UInt() / (f32)std::numeric_limits<uint32_t>::max();
    }

    f32 Random::Float(float min, float max)
    {
        return Float() * (max - min) + min;
    }

    glm::vec2 Random::Vec2()
    {
        return glm::vec2(2.0f * Float() - 1.0f, 2.0f * Float() - 1.0f);
    }

    glm::vec2 Random::Vec2(float min, float max)
    {
        return Float(min, max) * Vec2();
    }

    glm::vec3 Random::Vec3()
    {
        return glm::vec3(2.0f * Float() - 1.0f, 2.0f * Float() - 1.0f, 2.0f * Float() - 1.0f);
    }

    glm::vec3 Random::Vec3(f32 min, f32 max)
    {
        return Float(min, max) * Vec3();
    }
}
