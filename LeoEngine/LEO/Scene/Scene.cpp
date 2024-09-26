#include "Scene.h"
#include "SceneManager.h"
#include "../Application/Window/Window.h"

#define SCENE_ASSERT() LEOASSERT(m_sceneManager != nullptr, "Scene's Scene Manager pointer is null.");\
					   LEOASSERT(m_id != 0, "Scene id is 0, this is not valid.");

namespace LEO
{
	u32 Scene::ID()
	{
		SCENE_ASSERT();

		return m_id;
	}

	void Scene::CloseApplication()
	{
		SCENE_ASSERT();

		ApplicationEvent e;
		e.action = ApplicationEventAction::CLOSE_APP;
		m_sceneManager->OnEvent(e);		
	}

	glm::uvec2 Scene::WindowSize()
	{
		return m_sceneManager->GetWindow().WindowSize();
	}

	glm::vec2 Scene::MousePos()
	{
		return m_sceneManager->GetWindow().MousePos();
	}

	void Scene::ToggleVsync()
	{
		static bool set = false;
		m_sceneManager->GetWindow().SetVsync(set);
		set = !set;
	}

	f32 Scene::Time()
	{
		return m_sceneManager->GetWindow().Time();
	}

	u32 Scene::AddScene(std::shared_ptr<Scene> scene)
	{
		SCENE_ASSERT();

		u32 id = m_sceneManager->AddScene(std::move(scene));
		ApplicationEvent e;
		e.action = ApplicationEventAction::ADD_SCENE;
		e.scene_id = id;
		m_sceneManager->OnEvent(e);

		return id;
	}

	void Scene::TransitionToScene(u32 id)
	{
		SCENE_ASSERT();

		ApplicationEvent e;
		e.action = ApplicationEventAction::TRANSITION_TO_SCENE;
		e.scene_id = id;
		m_sceneManager->OnEvent(e);
	}

	void Scene::RemoveScene(u32 id)
	{
		SCENE_ASSERT();

		ApplicationEvent e;
		e.action = ApplicationEventAction::REMOVE_SCENE;
		e.scene_id = id;
		m_sceneManager->OnEvent(e);
	}

	Renderer& Scene::GetRenderer()
	{
		SCENE_ASSERT();

		return m_sceneManager->GetRenderer();
	}

	AssetManager& Scene::GetAssets()
	{
		SCENE_ASSERT();

		return m_sceneManager->GetAssets();
	}

	Camera& Scene::GetCamera()
	{
		SCENE_ASSERT();

		return m_sceneManager->GetRenderer().GetCamera();
	}
}
