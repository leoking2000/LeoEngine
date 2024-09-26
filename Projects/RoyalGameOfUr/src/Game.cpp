#include "Game.h"

void Game::OnCreate()
{
	glm::uvec2 win_size = WindowSize();
	m_renderer.Init(&GetAssets(), win_size.x, win_size.y);

	CreateRoom();
}

void Game::OnUpdate(f32 dt)
{
	m_camera.OnUpdate(dt);
}

void Game::OnRender()
{
	m_renderer.Render(m_camera);
}

void Game::OnEvent(const LEO::Event& e)
{
	m_camera.OnEvent(e);

	if (e.GetEventType() == LEO::EventType::KeyEvent)
	{
		const LEO::KeyEvent key_event = EVENT_CAST(e, LEO::KeyEvent);

		i32 keycode = key_event.keyCode;
		i32 action  = key_event.action;

		if (keycode == KEY_F && action == KEY_PRESS)
		{
			LOGINFO("F was pressed!");
		}

		if (keycode == KEY_ESCAPE && action == KEY_PRESS)
		{
			CloseApplication();
		}
	}

	if (e.GetEventType() == LEO::EventType::WindowResizeEvent)
	{
		const LEO::WindowReseizeEvent& win_size = EVENT_CAST(e, LEO::WindowReseizeEvent);
		m_renderer.Resize(win_size.width, win_size.height);
	}
}

#define FLOOR_SIZE 15
#define NUMBER_OF_BOARD_WHITE 15
#define NUMBER_OF_BOARD_RED 5

void Game::CreateRoom()
{
	u32 count = 0;
	glm::mat4* model_arr = nullptr;

	m_camera.pos = glm::vec3(-5.0f, 2.0f, 0.0f);
	m_camera.dir = glm::vec3(1.0f, 0.0f, 0.0f);

	// set Up FLOOR
	count = FLOOR_SIZE * FLOOR_SIZE;
	model_arr = new glm::mat4[count];

	GameRenderer::ObjectType floorBoxes = {
		GetAssets().AddMesh(LEO::DefaultMesh::CUBE),
		GetAssets().AddTextureFromFile("scene_data/Ur/wood.jpg"),
		count
	};

	LEO::Transform t;
	for (i32 x = 0; x < FLOOR_SIZE; x++)
	{
		for (i32 z = 0; z < FLOOR_SIZE; z++)
		{
			t.pos = glm::vec3(x, -1.0f, z);
			model_arr[z + x * FLOOR_SIZE] = t.ModelMatrix();
		}
	}

	m_renderer.AddObjectType(floorBoxes, model_arr);

	// set Up White blocks
	count = NUMBER_OF_BOARD_WHITE;
	GameRenderer::ObjectType whiteBoxes = {
		GetAssets().AddMesh(LEO::DefaultMesh::CUBE),
		GetAssets().AddTextureFromFile("scene_data/Ur/whiteMable_BaseColor.jpg"),
		count
	};

	t.pos = glm::vec3(3.0f, 0.0f, 7.0f);
	model_arr[0] = t.ModelMatrix();

	t.pos = glm::vec3(4.0f, 0.0f, 6.0f);
	model_arr[1] = t.ModelMatrix();
	t.pos = glm::vec3(4.0f, 0.0f, 7.0f);
	model_arr[2] = t.ModelMatrix();
	t.pos = glm::vec3(4.0f, 0.0f, 8.0f);
	model_arr[3] = t.ModelMatrix();

	t.pos = glm::vec3(5.0f, 0.0f, 6.0f);
	model_arr[4] = t.ModelMatrix();
	t.pos = glm::vec3(5.0f, 0.0f, 7.0f);
	model_arr[5] = t.ModelMatrix();
	t.pos = glm::vec3(5.0f, 0.0f, 8.0f);
	model_arr[6] = t.ModelMatrix();

	t.pos = glm::vec3(6.0f, 0.0f, 6.0f);
	model_arr[7] = t.ModelMatrix();
	t.pos = glm::vec3(6.0f, 0.0f, 8.0f);
	model_arr[8] = t.ModelMatrix();

	t.pos = glm::vec3(7.0f, 0.0f, 7.0f);
	model_arr[9] = t.ModelMatrix();
	t.pos = glm::vec3(8.0f, 0.0f, 7.0f);
	model_arr[10] = t.ModelMatrix();
	t.pos = glm::vec3(9.0f, 0.0f, 7.0f);
	model_arr[11] = t.ModelMatrix();
	t.pos = glm::vec3(10.0f, 0.0f, 6.0f);
	model_arr[12] = t.ModelMatrix();

	t.pos = glm::vec3(10.0f, 0.0f, 7.0f);
	model_arr[13] = t.ModelMatrix();
	t.pos = glm::vec3(10.0f, 0.0f, 8.0f);
	model_arr[14] = t.ModelMatrix();

	m_renderer.AddObjectType(whiteBoxes, model_arr);

	// set Up red blocks
	count = NUMBER_OF_BOARD_RED;
	GameRenderer::ObjectType redBoxes = {
		GetAssets().AddMesh(LEO::DefaultMesh::CUBE),
		GetAssets().AddTextureFromFile("scene_data/Ur/red_albedo.png"),
		count
	};

	t.pos = glm::vec3(3.0f, 0.0f, 6.0f);
	model_arr[0] = t.ModelMatrix();
	t.pos = glm::vec3(3.0f, 0.0f, 8.0f);
	model_arr[1] = t.ModelMatrix();
	t.pos = glm::vec3(6.0f, 0.0f, 7.0f);
	model_arr[2] = t.ModelMatrix();
	t.pos = glm::vec3(9.0f, 0.0f, 6.0f);
	model_arr[3] = t.ModelMatrix();
	t.pos = glm::vec3(9.0f, 0.0f, 8.0f);
	model_arr[4] = t.ModelMatrix();


	m_renderer.AddObjectType(redBoxes, model_arr);
	delete[] model_arr;
}
