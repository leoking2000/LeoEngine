#include <glad/glad.h>
#include "TestScene.h"


void TestScene::OnCreate()
{
	SetUpScene();
}

void TestScene::OnUpdate(f32 dt)
{
	GetCamera().OnUpdate(dt);
}

void TestScene::OnRender()
{
	GetRenderer().Render(m_drawables);
}

void TestScene::OnEvent(const LEO::Event& e)
{
	GetRenderer().OnEvent(e);

	if (e.GetEventType() == LEO::EventType::KeyEvent)
	{
		const LEO::KeyEvent key = EVENT_CAST(e, LEO::KeyEvent);

		if (key.keyCode == KEY_ESCAPE && key.action == KEY_PRESS)
		{
			CloseApplication();
		}

		if (key.keyCode == KEY_K && key.action == KEY_PRESS)
		{
			ToggleVsync();
		}
	}
}

#define FLOOR_SIZE 20

void TestScene::SetUpScene()
{
	u32 cube = GetAssets().AddMesh(LEO::DefaultMesh::CUBE);
	u32 sphere = GetAssets().AddMesh(LEO::DefaultMesh::SPHERE);
	u32 tex = GetAssets().AddTextureFromFile(RESOURCES_PATH"sandbox/chessBoard.png");

	// set Up FLOOR
	u32 count = FLOOR_SIZE * FLOOR_SIZE;
	glm::mat4* model_arr = new glm::mat4[count];

	LEO::Transform t;
	for (i32 x = 0; x < FLOOR_SIZE; x++)
	{
		for (i32 z = 0; z < FLOOR_SIZE; z++)
		{
			t.pos = glm::vec3(x, -1.0f, z);
			model_arr[z + x * FLOOR_SIZE] = t.ModelMatrix();
		}
	}
	
	GetAssets().GetMesh(cube).MakeInstancedArray(model_arr, count);
	delete[] model_arr;

	m_drawables.emplace_back(GetAssets().AddModel(cube), GetAssets());
	m_drawables[0].trasform().pos = glm::vec3(-10.0f, -1.0f, -15.0f);
	m_drawables[0].material().AlbedoMap = tex;

	// set up sphere
	m_drawables.emplace_back(GetAssets().AddModel(sphere), GetAssets());
	m_drawables[1].trasform().pos = glm::vec3(5.0f, 0.0f, -10.0f);
	m_drawables[1].material().AlbedoMap = tex;

	m_drawables.emplace_back(GetAssets().AddModel(), GetAssets());
	//m_drawables[2].model().LoadFromFile(RESOURCES_PATH"sandbox/dragon/dragon.obj");
	//m_drawables[2].model().LoadFromFile(RESOURCES_PATH"sandbox/factory/factory.obj");
	//m_drawables[2].material().Albedo = glm::vec3(0.52f, 0.81f, 0.93f);
	//m_drawables[2].trasform().scale = glm::vec3(4.0f);
	//m_drawables[2].trasform().eulerRot = glm::vec3(0.0f, 90.0f, 0.0f);

	// set up camera
	GetCamera().pos.z = 10.0f;
}

