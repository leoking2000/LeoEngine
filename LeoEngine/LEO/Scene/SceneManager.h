#pragma once
#include <queue>
#include <unordered_map>
#include "Scene.h"

namespace LEO
{
	class Window;

	// It works like a Finite-state machine.
	class SceneManager final
	{
	public:
		SceneManager(Window& win, AssetManager& assetManager, Renderer& renderer);

		SceneManager(const SceneManager&) = delete;
		SceneManager& operator=(const SceneManager&) = delete;

		~SceneManager();
	public:
		Window&       GetWindow();
		AssetManager& GetAssets();
		Renderer&     GetRenderer();
	public:
		bool Tick(f32 dt);
		void OnEvent(const Event& e);
		void OnEvent(const ApplicationEvent& e);
	public:
		/// <summary>
		/// Adds a scene to the state machine and returns the id of that scene.
		/// Does not change the current scene, does not call scene->OnCreate()
		/// scene->OnCreate() is called once it gets a ApplicationEvent AddScene action
		/// </summary>
		/// <param name="scene">the scene to be added</param>
		/// <returns>a uniqe id for the scene</returns>
		u32 AddScene(std::shared_ptr<Scene> scene); 

		/// <summary>
		/// Make the scene with specified id the current scene
		/// </summary>
		void TransitionToScene(u32 id);

		/// <summary>
		/// Removes scene from state machine.
		/// </summary>
		void RemoveScene(u32 id);

		/// <summary>
		/// returns the current scene
		/// </summary>
		Scene& GetCurrentScene();

		/// <summary>
		/// Removes all Scenes from the manager
		/// </summary>
		void RemoveAllScenes();
	private:
		std::unordered_map<u32, std::shared_ptr<Scene>> m_scenes_map;
		std::shared_ptr<Scene> m_currentScene;
		u32 m_insertedSceneID = 1;
		std::queue<LEO::ApplicationEvent> m_event_queue;
	private:
		Window& m_window;
		AssetManager& m_assetManager;
		Renderer& m_renderer;
	private:
		f32 m_time_accumulator = 0.0f;
		bool m_running = true;
	};
}