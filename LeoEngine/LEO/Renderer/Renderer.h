#pragma once
#include <glm/glm.hpp>
#include "../Utilities/Types.h"
#include "../AssetManager/AssetManager.h"
#include "../Scene/Event.h"
#include "../SceneData/Camera.h"
#include "../SceneData/Drawable.h"

namespace LEO
{
	struct RendererParameters
	{
		// Window size
		u32 width = 900;
		u32 height = 600u;

		// Rendering settings
		glm::vec3 background_color = glm::vec3(0.0f);
		f32 ambient_strength = 0.1f;

		// Projection settings
		f32 fov_angle = glm::radians(45.0f);
		f32 min_z = 0.1f;
		f32 max_z = 100.0f;
	};


	class Renderer
	{
	public:
		Renderer(const RendererParameters& params, AssetManager& assetManager);

		Renderer(const Renderer&) = delete;
		Renderer& operator=(const Renderer&) = delete;

		~Renderer();
	public:
		void OnEvent(const Event& e);
		void Render(std::vector<Drawable>& objects);
	public:
		Camera& GetCamera();
	private:
		AssetManager& m_assetManager;
		Camera m_camera;
		// light
		f32 m_ambient_strength = 0.1f;
	private:
		u32 m_shader;
	};
}