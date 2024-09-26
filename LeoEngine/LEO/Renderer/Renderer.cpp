#include <glad/glad.h>
#include "../Graphics/LeoGraphics.h"
#include "Renderer.h"

namespace LEO
{
	Renderer::Renderer(const RendererParameters& params, AssetManager& assetManager)
		:
		m_assetManager(assetManager)
	{
		LEO::GraphicsInitialization();

		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);
		glClearDepth(1.0f);

		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		glFrontFace(GL_CCW);

		glClearColor(params.background_color.r, params.background_color.g, params.background_color.b, 1.0f);

		m_ambient_strength = params.ambient_strength;

		m_camera.GenarateProjectionMatrix(params.width, params.height, params.fov_angle, params.min_z, params.max_z);
		m_shader = m_assetManager.AddShader(RESOURCES_PATH"sandbox/shader");	
	}

	Renderer::~Renderer()
	{
		m_assetManager.RemoveShader(m_shader);
	}

	void Renderer::OnEvent(const Event& e)
	{
		m_camera.OnEvent(e);

		if (e.GetEventType() == LEO::EventType::WindowResizeEvent)
		{
			const LEO::WindowReseizeEvent& s = EVENT_CAST(e, LEO::WindowReseizeEvent);
			glViewport(0, 0, s.width, s.height);
		}
	}

#define U_AMBIENT_STRENGT "u_ambientStrengt"
#define U_LIGHT_DIR       "u_lightDir"

	void Renderer::Render(std::vector<Drawable>& objects)
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		LEO::ShaderProgram& shader = m_assetManager.GetShader(m_shader);

		for (const LEO::Drawable& obj : objects)
		{
			// set up lights
			shader.SetUniform(U_AMBIENT_STRENGT, m_ambient_strength);
			shader.SetUniform(U_LIGHT_DIR, glm::normalize(glm::vec3(0.0f, -1.0f, -1.0f)));

			obj.Draw(shader, m_camera.GetProjectionMatrix() * m_camera.GetCameraView(), true);
		}
	}

	Camera& Renderer::GetCamera()
	{
		return m_camera;
	}
}