#pragma once
#include "../Utilities/Types.h"

namespace LEO
{
	namespace PhysicsConstants
	{
		const inline f32 MinBodyArea = 0.01f * 0.01f;
		const inline f32 MaxBodyArea = 64.0f * 64.0f;

		// Note: The Density of styrofoam is about 0.03-0.12 g/cm^3 
		const inline f32 MinBodyDensity = 0.1f;
		// Note: The Density of platinum is about 21.4 g/cm^3
		const inline f32 MaxBodyDensity = 21.4f;
	}
}

