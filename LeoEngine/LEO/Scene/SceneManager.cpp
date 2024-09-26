#include "SceneManager.h"
#include "../Application/Window/Window.h"
#include "../Utilities/Log.h"
#include "../Utilities/Types.h"

#define CURRENT_SCENE_ASSERT() LEOASSERT(m_currentScene, "Currecnt scene is null");

namespace LEO
{
	SceneManager::SceneManager(Window& win, AssetManager& assetManager, Renderer& renderer)
		:
		m_window(win),
		m_assetManager(assetManager),
		m_renderer(renderer)
	{
	}

	SceneManager::~SceneManager()
	{
		RemoveAllScenes();
	}

	Window& SceneManager::GetWindow()
	{
		return m_window;
	}

	AssetManager& SceneManager::GetAssets()
	{
		return m_assetManager;
	}

	Renderer& SceneManager::GetRenderer()
	{
		return m_renderer;
	}

	bool SceneManager::Tick(f32 dt)
	{
		CURRENT_SCENE_ASSERT();

		if (dt > 0.16f) {
			dt = 0.16f;
		}

		// Update Scene
		m_time_accumulator += dt;
		while (m_time_accumulator >= dt)
		{
			m_currentScene->OnUpdate(dt);
			m_time_accumulator -= dt;
		}

		// render Scene
		m_currentScene->OnRender();
		//m_currentScene->ImGuiRender();

		// handle ApplicationEvents
		while (!m_event_queue.empty())
		{
			OnEvent(m_event_queue.front());
			m_event_queue.pop();
		}

		return m_running;
	}

	void SceneManager::OnEvent(const Event& e)
	{
		CURRENT_SCENE_ASSERT();

		if (e.GetEventType() == EventType::ApplicationEvent)
		{
			m_event_queue.push(reinterpret_cast<const ApplicationEvent&>(e));
			return;
		}

		m_currentScene->OnEvent(e);
	}

	void SceneManager::OnEvent(const ApplicationEvent& e)
	{
		if (e.action == ApplicationEventAction::CLOSE_APP)
		{
			m_running = false;
			return;
		}

		if (e.action == ApplicationEventAction::TRANSITION_TO_SCENE)
		{
			TransitionToScene(e.scene_id);
			return;
		}

		if (e.action == ApplicationEventAction::ADD_SCENE)
		{
			auto it = m_scenes_map.find(e.scene_id);
			if (it != m_scenes_map.end())
			{
				it->second->m_id = e.scene_id;
				it->second->m_sceneManager = this;

				it->second->OnCreate();
			}
			return;
		}

		if (e.action == ApplicationEventAction::REMOVE_SCENE)
		{
			RemoveScene(e.scene_id);
			return;
		}
	}

	u32 SceneManager::AddScene(std::shared_ptr<Scene> scene)
	{
		LEOASSERT(scene, "scene that was given to add is null.");

		auto inserted = m_scenes_map.insert(std::make_pair(m_insertedSceneID, scene));
		m_insertedSceneID++;

		return m_insertedSceneID - 1;
	}

	void SceneManager::TransitionToScene(u32 id)
	{
		LEOASSERT(id != 0, "id of 0 is not valid, id was 0");

		auto it = m_scenes_map.find(id);
		if (it != m_scenes_map.end())
		{
			if (m_currentScene)
			{
				// If we have a current scene, we call its OnDeactivate method.
				m_currentScene->OnDeactivate();
			}

			// Setting the current scene ensures that it is updated and drawn.
			m_currentScene = it->second;

			m_currentScene->OnActivate();
		}
		else
		{
			LOGERROR("Scene with id={} does not exist", id);
		}
	}

	void SceneManager::RemoveScene(u32 id)
	{
		LEOASSERT(id != 0, "id of 0 is not valid, id was 0");

		auto it = m_scenes_map.find(id);
		if (it != m_scenes_map.end())
		{
			if (m_currentScene == it->second)
			{
				// If the scene we are removing is the current scene, 
				// we also want to set that to a null pointer so the scene 
				// is no longer updated.
				m_currentScene->OnDeactivate();
				m_currentScene = nullptr;
			}

			// We make sure to call the OnDestroy method 
			// of the scene we are removing.
			it->second->OnDestroy();

			m_scenes_map.erase(it);
		}
		else
		{
			LOGERROR("Scene with id={} does not exist", id);
		}
	}

	Scene& SceneManager::GetCurrentScene()
	{
		CURRENT_SCENE_ASSERT();

		return *m_currentScene;
	}

	void SceneManager::RemoveAllScenes()
	{
		if (m_currentScene) {
			m_currentScene->OnDeactivate();
		}
		
		for (auto& it : m_scenes_map) {
			it.second->OnDestroy();
		}

		m_currentScene = nullptr;
		m_scenes_map.clear();
		m_insertedSceneID = 1;
	}
}