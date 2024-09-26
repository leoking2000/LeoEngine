#include "Drawable.h"


namespace LEO
{
	Drawable::Drawable(u32 model, AssetManager& assetManager)
		:
		m_assetManager(assetManager),
		m_model(model)
	{

	}

	Transform& Drawable::trasform()
	{
		return m_trasfrom;
	}

	Material& Drawable::material(u32 i)
	{
		return m_assetManager.GetModel(m_model)[i].material;
	}

	Mesh& Drawable::mesh(u32 i)
	{
		return m_assetManager.GetMesh(m_assetManager.GetModel(m_model)[i].mesh_id);
	}

	Model& Drawable::model()
	{
		return m_assetManager.GetModel(m_model);
	}

	void Drawable::Draw(const ShaderProgram& shader, const glm::mat4& proj_view, bool use_mat) const
	{
		m_assetManager.GetModel(m_model).Draw(shader, proj_view, m_trasfrom.ModelMatrix(), 
		[&](const ShaderProgram& shader, const Material& material) {
				if (!use_mat) {
					return;
				}

				material.SetMaterialUniforms(shader, m_assetManager);
		});
	}

}