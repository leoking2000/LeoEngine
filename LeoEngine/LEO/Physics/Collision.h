#pragma once
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include "../Utilities/Types.h"

namespace LEO
{
	struct CollisionData2D
	{
		bool hit;
		glm::vec2 normal;
		f32 depth;

		operator bool() const { return hit; }
	};

	CollisionData2D IntersectCircles(const glm::vec2& posA, f32 rA, const glm::vec2& posB, f32 rB);
}

