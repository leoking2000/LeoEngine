#pragma once
#include <glm/glm.hpp>

namespace LEO
{
    class Transform
    {
    public:
        glm::vec3 pos = { 0.0f, 0.0f, 0.0f };
        glm::vec3 scale = { 1.0f, 1.0f, 1.0f };
        glm::vec3 eulerRot = { 0.0f, 0.0f, 0.0f };

        glm::mat4 ModelMatrix() const;
    };
}
