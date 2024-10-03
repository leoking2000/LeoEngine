#include <glad/glad.h>
#include "TestScene.h"


void TestScene::OnCreate()
{
	sphere = GetAssets().AddMesh(LEO::DefaultMesh::SPHERE);

	SetUpScene();
}

void TestScene::OnUpdate(f32 dt)
{
	float move_speed = 15.0f;
	glm::vec2 move_dir = glm::vec2(0.0f);

	if (up)
	{
		move_dir.y = 1.0f;
	}
	if (down)
	{
		move_dir.y = -1.0f;
	}
	if (right)
	{
		move_dir.x = 1.0f;
	}
	if (left)
	{
		move_dir.x = -1.0f;
	}

	if (move_dir.x != 0.0f || move_dir.y != 0)
	{
		glm::vec3 new_pos = glm::vec3(m_bodies[0].Position + glm::normalize(move_dir) * move_speed * dt, 0.0f);
		m_bodies[0].Position = new_pos;
		m_drawables[0].trasform().pos = new_pos;
	}

	for (i32 i = 0; i < m_bodies.size() - 1; i++)
	{
		LEO::RigidBody2D& bodyA   = m_bodies[i];
		LEO::Drawable& sphereA    = m_drawables[i];

		for (i32 j = i + 1; j < m_bodies.size(); j++)
		{
			LEO::RigidBody2D& bodyB = m_bodies[j];
			LEO::Drawable& sphereB  = m_drawables[j];

			if (auto data = LEO::IntersectCircles(bodyA.Position, bodyA.Radius(), bodyB.Position, bodyB.Radius()))
			{
				glm::vec2 move_amount = data.normal * (data.depth / 2.0f);

				bodyA.Position -= move_amount;
				sphereA.trasform().pos -= glm::vec3(move_amount, 0.0f);

				bodyB.Position += move_amount;
				sphereB.trasform().pos += glm::vec3(move_amount, 0.0f);
			}
		}
	}

	GetCamera().OnUpdate(dt);
}

void TestScene::OnRender()
{
	GetRenderer().Render(m_drawables);
}

void TestScene::OnEvent(const LEO::Event& e)
{
	GetRenderer().OnEvent(e);
	//GetRenderer().GetCamera().OnEvent(e);

	if (e.GetEventType() == LEO::EventType::KeyEvent)
	{
		i32 keycode = EVENT_CAST(e, LEO::KeyEvent).keyCode;
		i32 action = EVENT_CAST(e, LEO::KeyEvent).action;

		if (keycode == KEY_W)
		{
			up = !(action == KEY_RELEASE);
		}

		if (keycode == KEY_S)
		{
			down = !(action == KEY_RELEASE);
		}

		if (keycode == KEY_A)
		{
			left = !(action == KEY_RELEASE);
		}

		if (keycode == KEY_D)
		{
			right = !(action == KEY_RELEASE);
		}

		if (keycode == KEY_ESCAPE && action == KEY_PRESS)
		{
			CloseApplication();
		}

		if (keycode == KEY_K && action == KEY_PRESS)
		{
			ToggleVsync();
		}
	}
}


void TestScene::SetUpScene()
{
	for (i32 i = 0; i < 20; i++)
	{
		CreateSphere(glm::vec3(LEO::Random::Vec2(5.0f, 15.0f), 0.0f));
	}
	

	// set up camera
	GetCamera().pos = glm::vec3(3.0f, 3.0f, 35.0f);
}

void TestScene::CreateSphere(const glm::vec3 pos)
{
	glm::vec3 color = m_drawables.size() == 0 ? glm::vec3(1.0f, 0.0f, 0.0f) : glm::vec3(1.0f);
	f32 radius = 0.5f;

	m_drawables.emplace_back(GetAssets().AddModel(sphere), GetAssets());
	m_drawables.back().trasform().pos = pos;
	m_drawables.back().trasform().scale = glm::vec3(radius);
	m_drawables.back().material().Albedo = color;

	m_bodies.emplace_back(LEO::RigidBody2D::CreateCircle(pos, radius, false));
}

