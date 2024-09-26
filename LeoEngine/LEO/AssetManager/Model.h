#pragma once
#include <functional>
#include "Material.h"

namespace LEO
{
	struct SubModel
	{
		u32 mesh_id;
		Material material;
	};

	class Model
	{
	public:
		using SetExtraUniforms = std::function<void(const ShaderProgram&, const Material&)>;
	public:
		Model(AssetManager& assetManager);
		Model(std::vector<SubModel>& submodels, AssetManager& assetManager);
		Model(const SubModel& submodel, AssetManager& assetManager);
	public:
		SubModel& operator[](u32 i);
		u32  NumberOfSubModels();
		void AddSubModel(const SubModel& mesh);
	public:
		void Draw(const ShaderProgram& shader, 
				  const glm::mat4& proj_view, const glm::mat4& model_matrix, 
				  SetExtraUniforms setExtraUniforms);
	private:
		AssetManager& m_assetManager;
		std::vector<SubModel> m_sub_models;
	};


}
