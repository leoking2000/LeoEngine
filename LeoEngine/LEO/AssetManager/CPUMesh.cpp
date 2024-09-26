#include "CPUMesh.h"
#include "../AssetManager/AssetManager.h"
#include "../Utilities/FileUtilities.h"
#include "../Utilities/Log.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace LEO
{
	std::vector<CPUMesh> CPUMesh::Load(const std::string& filename, AssetManager& assetManager)
	{
		LOGDEBUG("Loading 3D Model file: {}", filename);

		Assimp::Importer imp;

		const aiScene* model = imp.ReadFile(filename,
			aiProcess_Triangulate | aiProcess_CalcTangentSpace |
			aiProcess_JoinIdenticalVertices | aiProcess_OptimizeMeshes | aiProcess_OptimizeGraph);

		if (model == nullptr)
		{
			LOGERROR("Failed Loading 3D Model file: {}", filename);
			return std::vector<CPUMesh>();
		}

		auto matirials = model->mMaterials;

		std::vector<CPUMesh> mesh_vector;

		for (u32 i = 0; i < model->mNumMeshes; i++)
		{
			const aiMesh* mesh = model->mMeshes[i];

			std::vector<Vertex> in_vertices;
			std::vector<u32> in_indices;

			for (u32 v = 0; v < mesh->mNumVertices; v++)
			{
				Vertex vert;

				vert.pos = glm::vec3(mesh->mVertices[v].x, mesh->mVertices[v].y, mesh->mVertices[v].z);

				if (mesh->mNormals != nullptr)
				{
					vert.normal = glm::vec3(mesh->mNormals[v].x, mesh->mNormals[v].y, mesh->mNormals[v].z);
					//vert.tangent = glm::vec3(mesh->mTangents[v].x, mesh->mTangents[v].y, mesh->mTangents[v].z);
					//vert.bitangent = glm::vec3(mesh->mBitangents[v].x, mesh->mBitangents[v].y, mesh->mBitangents[v].z);
				}

				if (mesh->mTextureCoords[0] != nullptr)
				{
					vert.texCord = glm::vec2(mesh->mTextureCoords[0][v].x, mesh->mTextureCoords[0][v].y);
				}
				else
				{
					vert.texCord = glm::vec2(0.0f);
				}

				in_vertices.push_back(vert);
			}

			for (uint32_t f = 0; f < mesh->mNumFaces; f++)
			{
				const aiFace& face = mesh->mFaces[f];
				assert(face.mNumIndices == 3);

				in_indices.push_back(face.mIndices[0]);
				in_indices.push_back(face.mIndices[1]);
				in_indices.push_back(face.mIndices[2]);
			}

			CPUMesh in_mesh;

			uint32_t matirialIndex = mesh->mMaterialIndex;

			if (matirialIndex >= 0)
			{
				auto& material = *matirials[matirialIndex];
				aiString texFileName;

				if (material.GetTexture(aiTextureType_DIFFUSE, 0, &texFileName) == aiReturn_SUCCESS)
				{
					std::string tex_path = DirNameOf(filename) + "/" + texFileName.C_Str();
					in_mesh.material.AlbedoMap = assetManager.AddTextureFromFile(tex_path);
				}
				else
				{
					aiColor3D c(0.0f, 0.0f, 0.0f);
					material.Get(AI_MATKEY_COLOR_DIFFUSE, c);
					in_mesh.material.Albedo = glm::vec3(c.r, c.g, c.b);
				}

				if (material.GetTexture(aiTextureType_NORMALS, 0, &texFileName) == aiReturn_SUCCESS)
				{
					//std::string tex_path = DirNameOf(filename) + "\\" + texFileName.C_Str();
					//in_mesh.material.NormalMap = assetManager.AddTextureFromFile(tex_path);
				}

				// TODO: get Metallic and Roughness
			}

			in_mesh.vertices = in_vertices;
			in_mesh.indices = in_indices;

			mesh_vector.push_back(std::move(in_mesh));
		}

		LOGDEBUG("{} Has been Loaded!!! (sub_meshs: {})", filename, mesh_vector.size());

		return mesh_vector;
	}

	Mesh CPUMesh::Convert(const CPUMesh& cpu_mesh)
	{
		std::vector<float> vertex_buffer;

		for (auto& v : cpu_mesh.vertices)
		{
			vertex_buffer.emplace_back(v.pos.x);
			vertex_buffer.emplace_back(v.pos.y);
			vertex_buffer.emplace_back(v.pos.z);

			vertex_buffer.emplace_back(v.texCord.s);
			vertex_buffer.emplace_back(v.texCord.t);

			vertex_buffer.emplace_back(v.normal.x);
			vertex_buffer.emplace_back(v.normal.y);
			vertex_buffer.emplace_back(v.normal.z);

			vertex_buffer.emplace_back(v.tangent.x);
			vertex_buffer.emplace_back(v.tangent.y);
			vertex_buffer.emplace_back(v.tangent.z);

			vertex_buffer.emplace_back(v.bitangent.x);
			vertex_buffer.emplace_back(v.bitangent.y);
			vertex_buffer.emplace_back(v.bitangent.z);
		}

		VertexBuffer vertexBuffer((const void*)vertex_buffer.data(), (u32)(vertex_buffer.size() * sizeof(float)));

		ElementType arr[5] = { FLOAT3, FLOAT2, FLOAT3_N, FLOAT3_N, FLOAT3_N };
		Layout<5> layout(arr);

		VertexArray vertexArray;
		vertexArray.AddBuffer(vertexBuffer, layout);

		IndexBuffer indexBuffer(cpu_mesh.indices.data(), (u32)cpu_mesh.indices.size());

		Mesh mesh{vertexArray, indexBuffer, 5};

		return mesh;
	}

}