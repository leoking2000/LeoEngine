#include <assert.h>
#include "../Graphics/LeoGraphics.h"
#include "Application.h"

namespace LEO
{
	static bool g_innitEngine = false;

	void EngineInitialization()
	{
		assert(g_innitEngine == false);

		LOGInitialization(new LEO::ConsoleChannel());
		WINInitialization();

		GetDefaultLogChannel()->SetLoggingLevel(LogLevel::VERBOSE);
	}

	void EngineTerminate()
	{
		WINTerminate();
		LOGTerminate();
	}

	Application::Application(ApplicationParameters& params)
		:
		m_window(params.width, params.height, params.isResizable, params.vsync),
		m_renderer(RendererParameters{ params.width, params.height, 
			params.background_color, params.ambient_strength, params.fov_angle, params.min_z, params.max_z}, m_assetManager),
		m_sceneManager(m_window, m_assetManager, m_renderer)
	{
		m_window.SetWindowTitle(params.title);
		GetDefaultLogChannel()->SetLoggingLevel(params.logLevel);

		m_window.SetKeyboardCallback([this](int key, int action) 
		{
			KeyEvent e;
			e.keyCode = key;
			e.action = action;
			m_sceneManager.OnEvent(e);
		});

		m_window.SetMouseButtonCallBack([this](int key, int action)
		{
			glm::vec2 pos = m_window.MousePos();
			MouseEvent e;
			e.keycode = key;
			e.action = action;
			e.x = pos.x;
			e.y = pos.y;
			m_sceneManager.OnEvent(e);
		});
		
		m_window.SetMouseMoveCallback([this](float x, float y)
		{
			MouseEvent e;
			e.keycode = -1;
			e.action  = -1;
			e.x = x;
			e.y = y;
			m_sceneManager.OnEvent(e);
		});

		m_window.SetResizeCallback([this](int width, int height) {
			WindowReseizeEvent e;
			e.width = width;
			e.height = height;

			m_sceneManager.OnEvent(e);
		});
	}

	void Application::Run(CreateStartSceneCallBack createScene)
	{	
		AddStartScene(createScene());

		f32 deltaTime;
		f32 start = m_window.Time();
		f32 end = start;

		while (!m_window.ShouldWindowClose())
		{
			end = m_window.Time();
			deltaTime = end - start;
			start = end;

			m_window.NewFrame();
			bool should_close = !m_sceneManager.Tick(deltaTime);
			m_window.EndFrame();

			if (should_close)
			{
				m_window.CloseWindow();
			}

#if !PRODUCTION_BUILD
			m_window.SetWindowTitle("FPS:" + std::to_string(int(1.0f / deltaTime)) + 
				" (" + std::to_string(int(deltaTime * 1000.0f)) + " ms)");
#endif // PRODUCTION_BUILD
		}

		m_sceneManager.RemoveAllScenes();
	}

	void Application::AddStartScene(std::shared_ptr<Scene> start_scene)
	{
		u32 main_id = m_sceneManager.AddScene(std::move(start_scene));

		ApplicationEvent e;
		e.action = ApplicationEventAction::ADD_SCENE;
		e.scene_id = main_id;
		m_sceneManager.OnEvent(e);

		m_sceneManager.TransitionToScene(main_id);
	}
}