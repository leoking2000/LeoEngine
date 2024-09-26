#pragma once
#include "../Renderer/Renderer.h"
#include "../Scene/SceneManager.h"
#include "Window/Window.h"


namespace LEO
{
	void EngineInitialization();
	void EngineTerminate();

	struct ApplicationParameters
	{
		// Window settings
		u32 width = 900;
		u32 height = 600u;
		std::string title = "LeoEngine";
		bool isResizable = false;
		bool vsync = true;

		// Rendering settings
		glm::vec3 background_color = glm::vec3(0.0f);
		f32 ambient_strength = 0.1f;

		// Projection settings
		f32 fov_angle = glm::radians(45.0f);
		f32 min_z = 0.1f;
		f32 max_z = 100.0f;

		std::shared_ptr<Scene> startScene;

		// Log settings	
		LEO::LogLevel logLevel = LEO::LogLevel::INFO;
	};

	class Application final
	{
	public:
		Application(ApplicationParameters& params);

		Application(const Application&) = delete;
		Application& operator=(const Application&) = delete;
	public:
		void Run();
	private:
		void AddStartScene(std::shared_ptr<Scene> start_scene);
	private:
		LEO::Window       m_window;
		LEO::AssetManager m_assetManager;
		LEO::Renderer     m_renderer;
		LEO::SceneManager m_sceneManager;
	};
}