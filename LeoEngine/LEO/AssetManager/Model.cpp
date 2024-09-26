#include "Model.h"
#include "../AssetManager/AssetManager.h"

#define U_PROJECTON_VIEW_MATRIX "u_proj_view_matrix"
#define U_MODEL_MATRIX          "u_model_matrix"
#define U_NORMAL_MATRIX         "u_normal_matrix"
#define U_HAS_INSTANCE_ARRAY    "u_hasInstanceArray"

namespace LEO
{
	Model::Model(AssetManager& assetManager)
		:
		m_assetManager(assetManager)
	{}

	Model::Model(std::vector<SubModel>& submodels, AssetManager& assetManager)
		:
		m_sub_models(std::move(submodels)),
		m_assetManager(assetManager)
	{}

	Model::Model(const SubModel& submodel, AssetManager& assetManager)
		:
		m_assetManager(assetManager)
	{
		m_sub_models.push_back(submodel);
	}

	SubModel& Model::operator[](u32 i)
	{
		return m_sub_models[i];
	}

	u32 Model::NumberOfSubModels()
	{
		return (u32)m_sub_models.size();
	}

	void Model::AddSubModel(const SubModel& mesh)
	{
		m_sub_models.push_back(mesh);
	}

	void Model::Draw(const ShaderProgram& shader, 
					 const glm::mat4& proj_view, const glm::mat4& model_matrix, 
					 SetExtraUniforms setExtraUniforms)
	{
		glm::mat3 normal_matrix = glm::mat3(glm::transpose(glm::inverse(model_matrix)));

		for (i32 i = 0; i < m_sub_models.size(); i++)
		{
			Material& material = m_sub_models[i].material;
			Mesh& mesh          = m_assetManager.GetMesh(m_sub_models[i].mesh_id);

			shader.SetUniform(U_PROJECTON_VIEW_MATRIX, proj_view);
			shader.SetUniform(U_MODEL_MATRIX, model_matrix);
			shader.SetUniform(U_NORMAL_MATRIX, normal_matrix);
			shader.SetUniform(U_HAS_INSTANCE_ARRAY, mesh.HasInstanceArray() ? 1 : 0);

			setExtraUniforms(shader, material);

			shader.Bind();
			mesh.Draw();
		}

		shader.UnBind();
	}
}