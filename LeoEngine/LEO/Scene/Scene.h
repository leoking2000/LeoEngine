#pragma once
#include "../Renderer/Renderer.h"

namespace LEO
{
	class SceneManager;

	// Abstract class, represents a game scene with all the game data and logic.
	class Scene
	{
	public:
		/// <summary>
		/// Called when the scene is created, called only onces.
		/// </summary>
		virtual void OnCreate() = 0;

		/// <summary>
		/// Called when the scene is deleted, called only onces.
		/// </summary>
		virtual void OnDestroy() {}

		/// <summary>
		/// Called whenever a scene is transitioned into. Can be called many times in a typical game.
		/// </summary>
		virtual void OnActivate() {}

		/// <summary>
		/// Called whenever a transition out of a scene occurs. Can be called many times in a typical game.
		/// </summary>
		virtual void OnDeactivate() {}

		/// <summary>
		/// Called after OnUpdate, OnRender to handle a Event
		/// </summary>
		virtual void OnEvent(const Event& e) = 0;

		/// <summary>
		/// Called every frame, runs Scene's game logic 
		/// </summary>
		/// <param name="dt">deltaTime in sec</param>
		virtual void OnUpdate(f32 dt) = 0;

		/// <summary>
		/// Called every frame, runs render code
		/// </summary>
		virtual void OnRender() = 0;

		virtual ~Scene() = default;
	protected:
		AssetManager&    GetAssets();
		Renderer&        GetRenderer();
		Camera&          GetCamera();
	protected:
		f32              Time();
		glm::uvec2       WindowSize();
		glm::vec2        MousePos();
		void             ToggleVsync();
	protected:
		u32              ID();
		void             CloseApplication();	     
		u32              AddScene(std::shared_ptr<Scene> scene);
		void             TransitionToScene(u32 id);
		void             RemoveScene(u32 id);
	private:
		friend SceneManager;
		u32 m_id = 0;
		SceneManager* m_sceneManager = nullptr;
	};
}