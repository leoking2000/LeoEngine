#include <tiny_obj_loader.h>
#include "Model.h"
#include "../AssetManager/AssetManager.h"

#define U_PROJECTON_VIEW_MATRIX "u_proj_view_matrix"
#define U_MODEL_MATRIX          "u_model_matrix"
#define U_NORMAL_MATRIX         "u_normal_matrix"
#define U_HAS_INSTANCE_ARRAY    "u_hasInstanceArray"

namespace LEO
{
	Model::Model(const std::string& filepath, AssetManager& assetManager)
		:
		m_assetManager(assetManager)
	{
		LoadFromFile(filepath);
	}

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

	void Model::LoadFromFile(const std::string& filepath)
	{
		LOGDEBUG("Loading Model {}", filepath);

		tinyobj::attrib_t attrib;
		std::vector<tinyobj::shape_t> shapes;
		std::vector<tinyobj::material_t> materials;

		std::string warn;
		std::string err;

		bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, filepath.c_str());

		if (!warn.empty()) {
			LOGWARN("{}", warn);
		}

		if (!err.empty()) {
			LOGERROR("{}", err);
		}

		if (!ret) {
			LEOASSERT(ret, "Failed Loading Model {}", filepath);
			return;
		}

		std::vector<f32> vertex_buffer;
		std::vector<u32> index_buffer;

		// Loop over shapes
		for (size_t s = 0; s < shapes.size(); s++) 
		{
			index_buffer.clear();
			for (auto& i : shapes[s].mesh.indices)
			{
				index_buffer.push_back(i.vertex_index);
			}

			vertex_buffer.clear();
			// Loop over faces(polygon)
			size_t index_offset = 0;
			for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++) 
			{
				size_t fv = size_t(shapes[s].mesh.num_face_vertices[f]);

				// Loop over vertices in the face.
				for (size_t v = 0; v < fv; v++) 
				{
					// access to vertex
					tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];

					vertex_buffer.push_back(attrib.vertices[3 * size_t(idx.vertex_index) + 0]);
					vertex_buffer.push_back(attrib.vertices[3 * size_t(idx.vertex_index) + 1]);
					vertex_buffer.push_back(attrib.vertices[3 * size_t(idx.vertex_index) + 2]);

					// Check if `texcoord_index` is zero or positive. negative = no texcoord data
					if (idx.texcoord_index >= 0) {
						vertex_buffer.push_back(attrib.texcoords[2 * size_t(idx.texcoord_index) + 0]);
						vertex_buffer.push_back(attrib.texcoords[2 * size_t(idx.texcoord_index) + 1]);
					}
					else
					{
						vertex_buffer.push_back(0.0f);
						vertex_buffer.push_back(0.0f);
					}

					// Check if `normal_index` is zero or positive. negative = no normal data
					if (idx.normal_index >= 0) {
						vertex_buffer.push_back(attrib.normals[3 * size_t(idx.normal_index) + 0]);
						vertex_buffer.push_back(attrib.normals[3 * size_t(idx.normal_index) + 1]);
						vertex_buffer.push_back(attrib.normals[3 * size_t(idx.normal_index) + 2]);
					}
					else
					{
						vertex_buffer.push_back(0.0f);
						vertex_buffer.push_back(0.0f);
						vertex_buffer.push_back(0.0f);
					}
				}

				index_offset += fv;
			}

			// per-face material
			tinyobj::material_t matririal = materials[shapes[s].mesh.material_ids[0]];

			Material mat;
			if (matririal.diffuse_texname != "")
			{
				mat.AlbedoMap = m_assetManager.AddTextureFromFile(filepath + matririal.diffuse_texname);
			}

			VertexBuffer vertexBuffer((const void*)vertex_buffer.data(), (u32)(vertex_buffer.size() * sizeof(float)));

			ElementType arr[3] = { FLOAT3, FLOAT2, FLOAT3_N };
			Layout<3> layout(arr);

			VertexArray vertexArray;
			vertexArray.AddBuffer(vertexBuffer, layout);

			IndexBuffer indexBuffer(index_buffer.data(), (u32)index_buffer.size());
			u32 m = m_assetManager.AddMesh(Mesh{ vertexArray, indexBuffer, 3 });

			AddSubModel({ m, mat });
		}
	}
}