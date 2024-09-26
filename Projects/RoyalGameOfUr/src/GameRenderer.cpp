#include <glad/glad.h>
#include "GameRenderer.h"

void GameRenderer::Init(LEO::AssetManager* assetManager, u32 width, u32 height)
{
	m_assetManager = assetManager;

	m_proj = glm::perspective(glm::radians(45.0f),
		(f32)width / (f32)height,
		0.1f, 100.0f);

	glViewport(0, 0, width, height);
	glClearColor(0.52f, 0.81f, 0.93f, 1.0f);

	m_geometry_shader = m_assetManager->AddShader("scene_data/shaders/geometry/geometry_pass");
	m_geometry_framebuffer = std::make_unique<LEO::FrameBuffer>(width, height, 4,
		LEO::TextureMinFiltering::MIN_NEAREST, LEO::TextureMagFiltering::MAG_NEAREST);

	m_screen_filled_quad = m_assetManager->AddMesh(LEO::DefaultMesh::SCRERN_FILLED_QUARD);
	m_shading_shader     = m_assetManager->AddShader("scene_data/shaders/lighting/shading_pass");
}

void GameRenderer::Resize(u32 width, u32 height)
{
	glViewport(0, 0, width, height);
	m_geometry_framebuffer->Resize(width, height);

	m_proj = glm::perspective(glm::radians(45.0f),
		(f32)width / (f32)height,
		0.1f, 100.0f);
}

void GameRenderer::AddObjectType(ObjectType t, glm::mat4* model_arr)
{
	m_objectTypes.push_back(t);

	LEO::Mesh& mesh = m_assetManager->GetMesh(t.mesh_id);
	mesh.MakeInstancedArray(model_arr, t.count);
}

void GameRenderer::Render(const LEO::Camera& camera)
{
	GeometryPass(camera);
	ShadingPass(camera);
}

void GameRenderer::GeometryPass(const LEO::Camera& camera)
{
	m_geometry_framebuffer->Bind();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glViewport(0, 0, m_geometry_framebuffer->Width(), m_geometry_framebuffer->Height());

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glClearDepth(1.0f);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);

	LEO::ShaderProgram& shader = m_assetManager->GetShader(m_geometry_shader);

	for (ObjectType& t : m_objectTypes)
	{
		LEO::Mesh& mesh = m_assetManager->GetMesh(t.mesh_id);

		m_assetManager->GetTexture(t.albedo_id).Bind(0);
		shader.SetUniform("u_HasAlbedoMap", 1);
		shader.SetUniform("u_albedoMap", 0);

		shader.SetUniform("u_HasNormalMap", 0);

		shader.SetUniform("u_view_matrix", camera.GetCameraView());
		shader.SetUniform("u_proj_matrix", m_proj);

		shader.Bind();
		mesh.Draw();
	}

	m_geometry_framebuffer->UnBind();
}

void GameRenderer::ShadingPass(const LEO::Camera& camera)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glDisable(GL_DEPTH_TEST);

	LEO::ShaderProgram& shader = m_assetManager->GetShader(m_shading_shader);

	m_geometry_framebuffer->BindColorTexture(0, 0);
	m_geometry_framebuffer->BindColorTexture(1, 1);
	m_geometry_framebuffer->BindColorTexture(2, 2);
	m_geometry_framebuffer->BindColorTexture(3, 3);
	m_geometry_framebuffer->BindDepthTexture(5);

	shader.SetUniform("u_tex_albedo", 0);
	shader.SetUniform("u_tex_pos", 1);
	shader.SetUniform("u_tex_normal", 2);
	shader.SetUniform("u_tex_mask", 3);
	shader.SetUniform("u_tex_depth", 5);

	shader.SetUniform("u_camera_pos", camera.pos);
	shader.SetUniform("u_camera_dir", camera.dir);

	shader.SetUniform("u_light.type", 0);
	shader.SetUniform("u_light.dir", glm::normalize(glm::vec3(6.0f, 0.0f, 7.0f) - glm::vec3(7.0f, 17.0f, 7.5f)));

	shader.SetUniform("u_light.radiance", glm::vec3(1.0f));

	shader.Bind();
	m_assetManager->GetMesh(m_screen_filled_quad).Draw();
}

//LEO::ShaderProgram& shader = m_assetManager->GetShader(m_shader);

//for (ObjectType& t : m_objectTypes)
//{
//	LEO::Mesh& mesh = m_assetManager->GetMesh(t.mesh_id);

//	m_assetManager->GetTexture(t.albedo_id).Bind(0);
//	shader.SetUniform("u_albedoMap", 0);
//	shader.SetUniform("u_view_matrix", view);
//	shader.SetUniform("u_proj_matrix", m_proj);

//	shader.Bind();
//	mesh.m_vertexArray.Bind();
//	mesh.m_indexBuffer.Bind();

//	glDrawElementsInstanced(GL_TRIANGLES, mesh.m_indexBuffer.GetCount(),
//		GL_UNSIGNED_INT, nullptr, t.count);
//}
