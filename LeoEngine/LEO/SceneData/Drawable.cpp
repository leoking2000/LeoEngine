#include "Drawable.h"


namespace LEO
{
	//Drawable::Drawable(Model& model)
	//	:
	//	m_model(model)
	//{

	//}

	//Transform& Drawable::trasform()
	//{
	//	return m_trasfrom;
	//}

	//Material& Drawable::material(u32 i)
	//{
	//	return m_model[i].material;
	//}

	//Mesh& Drawable::mesh(u32 i)
	//{
	//	return *m_model[i].mesh.Get();
	//}

	//Model& Drawable::model()
	//{
	//	return m_model;
	//}

	//void Drawable::Draw(const ShaderProgram& shader, const glm::mat4& proj_view, bool use_mat)
	//{
	//	m_model.Draw(shader, proj_view, m_trasfrom.ModelMatrix(),
	//		[&](const ShaderProgram& shader, const Material& material) {
	//			if (!use_mat) {
	//				return;
	//			}

	//			//material.AlbedoMap->
	//			shader.SetUniform("u_albedoMap", 0);

	//			shader.Bind();
	//		});
	//}

}