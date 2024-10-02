#pragma once
#include <vector>
#include <functional>
#include "Material.h"
#include "Transform.h"


namespace LEO
{
	class SubModel
	{
	public:
		SubModel(MeshRef& mesh, Material& mat, Transform& local);
	public:
		Transform local_transform;
		MeshRef mesh;
		Material material;
	};

	class Model
	{
	public:
		SubModel& operator[](u32 i);
		u32  NumberOfSubModels() const;
		void AddSubModel(MeshRef& mesh, Material& mat, Transform& local);
	public:
		using SetExtraUniforms = std::function<void(const ShaderProgram&, const Material&)>;
		void Draw(const ShaderProgram& shader,
			const glm::mat4& proj_view, const glm::mat4& model_matrix,
			SetExtraUniforms setExtraUniforms);
	private:
		Model() = default;
	private:
		std::vector<SubModel> m_sub_models;
	};
}
