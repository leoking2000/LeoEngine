#include <random>
#include <glm/glm.hpp>
#include "Types.h"

namespace LEO
{
    class Random
    {
    public:
        static u32 UInt();
        static u32 UInt(u32 min, u32 max);

        static f32 Float();
        static f32 Float(f32 min, f32 max);

        static glm::vec2 Vec2();
        static glm::vec2 Vec2(f32 min, f32 max);
    private:
        Random();
        static Random& Get();
    private:
        std::mt19937 m_randomEngine;
		std::uniform_int_distribution<std::mt19937::result_type> m_distribution;
    };
}