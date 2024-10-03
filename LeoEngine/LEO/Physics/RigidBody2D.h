#pragma once
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include "PhysicsConstants.h"
#include "Collision.h"

namespace LEO
{
	enum class ShapeType2D
	{
		BOX,
		CIRCLE
	};

	class RigidBody2D
	{
	public:
		static RigidBody2D CreateCircle(const glm::vec3& pos, f32 radius, bool isStatic, 
			f32 bounciness = 0.5f, f32 density = 1.0f);

		static RigidBody2D CreateBox(const glm::vec3& pos, f32 width, f32 height, bool isStatic, 
			f32 bounciness = 0.5f, f32 density = 1.0f);
	public:
		inline ShapeType2D Shape()      const { return m_type;       }
		inline f32         Width()      const { return m_data[0];    } 
		inline f32         Height()     const { return m_data[1];    }
		inline f32         Radius()     const { return m_data[0];    }
	public:
		inline bool        isStatic()   const { return m_isStatic;   }
		inline f32         Mass()       const { return m_mass;       }
		inline f32         Bounciness() const { return m_bounciness; }
	public:
		glm::vec2 Position = glm::vec2(0.0f);
		glm::vec2 Velocity = glm::vec2(0.0f);
		f32 Rotation = 0.0f;
		f32 RotationalVelocity = 0.0f;
	private:
		ShapeType2D m_type;
		f32 m_data[2]; // m_data[0] = width  m_data[1] = height for Box, m_data[0] = radious for Circle
	private:
		bool m_isStatic;
		f32 m_mass;
		f32 m_bounciness;
	private:
		RigidBody2D(const glm::vec3& pos,
			ShapeType2D shape, f32 data0, f32 data1,
			bool isStatic, f32 mass, f32 bounciness);
	};
}

