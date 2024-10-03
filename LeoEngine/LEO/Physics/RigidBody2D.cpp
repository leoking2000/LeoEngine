#include "RigidBody2D.h"
#include "../Utilities/Log.h"

namespace LEO
{
	RigidBody2D RigidBody2D::CreateCircle(const glm::vec3& pos, f32 radius, bool isStatic, f32 bounciness, f32 density)
	{
		f32 area = radius * radius * glm::pi<f32>();

		LEOASSERT(area <= PhysicsConstants::MaxBodyArea, "Circle area is too big");
		LEOASSERT(area >= PhysicsConstants::MinBodyArea, "Circle area is too small");
		LEOASSERT(density <= PhysicsConstants::MaxBodyDensity, "Circle is too dense");
		LEOASSERT(density >= PhysicsConstants::MinBodyDensity, "Circle is too sparse");

		return RigidBody2D(pos, ShapeType2D::CIRCLE, radius, radius, isStatic, area * density, glm::clamp(bounciness, 0.0f, 1.0f));
	}

	RigidBody2D RigidBody2D::CreateBox(const glm::vec3& pos, f32 width, f32 height, bool isStatic, f32 bounciness, f32 density)
	{
		f32 area = width * height;

		LEOASSERT(area <= PhysicsConstants::MaxBodyArea, "Box area is too big");
		LEOASSERT(area >= PhysicsConstants::MinBodyArea, "Box area is too small");
		LEOASSERT(density <= PhysicsConstants::MaxBodyDensity, "Box is too dense");
		LEOASSERT(density >= PhysicsConstants::MinBodyDensity, "Box is too sparse");

		return RigidBody2D(pos, ShapeType2D::BOX, width, height, isStatic, area * density, glm::clamp(bounciness, 0.0f, 1.0f));
	}

	RigidBody2D::RigidBody2D(const glm::vec3& pos,
		ShapeType2D shape, f32 data0, f32 data1,
		bool isStatic, f32 mass, f32 bounciness)
		:
		Position(pos),
		m_type(shape),
		m_data{data0, data1},
		m_isStatic(isStatic),
		m_mass(mass),
		m_bounciness(bounciness)
	{}
}