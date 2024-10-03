#include "Collision.h"

namespace LEO
{
	CollisionData2D IntersectCircles(const glm::vec2& posA, f32 rA, const glm::vec2& posB, f32 rB)
	{
		CollisionData2D output;

		f32 distance = glm::distance(posA, posB);
		f32 r = rA + rB;

		if (distance >= r)
		{
			output.hit = false;
			output.normal = glm::vec2(0.0f);
			output.depth = 0.0f;
			return output;
		}

		output.hit = true;
		output.normal = glm::normalize(posB - posA);
		output.depth = r - distance;

		return output;
	}
}