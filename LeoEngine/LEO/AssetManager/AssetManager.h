#pragma once
#include <unordered_map>
#include "../Utilities/Log.h"
#include "../Graphics/Mesh.h"
#include "../Graphics/Shader.h"
#include "../Graphics/Texture.h"
#include "Model.h"

namespace LEO
{
	class AssetManager
	{
	public:
		AssetManager() = default;

		AssetManager(const AssetManager&) = delete;
		AssetManager& operator=(const AssetManager&) = delete;

		~AssetManager();
	public:
		u32   AddMesh(DefaultMesh shape);
		u32   AddMesh(Mesh&& m);
		void  RemoveMesh(u32 id);
		Mesh& GetMesh(u32 id);
	public:
		u32 AddModel();
		u32 AddModel(u32 mesh, Material mat = {});
		//u32 AddModel(const std::string& filepath);
		void RemoveModel(u32 id);
		Model& GetModel(u32 id);
	public:
		u32 AddTextureFromFile(const std::string& filepath);
		void RemoveTexture(u32 id);
		Texture& GetTexture(u32 id);
	public:
		u32 AddShader(const std::string& filepath);
		void RemoveShader(u32 id);
		ShaderProgram& GetShader(u32 id);
	private:
		template<typename T>
		static void Remove(std::unordered_map<u32, T>& map, u32 id, const char* typeName)
		{
			auto it = map.find(id);
			if (it != map.end())
			{
				map.erase(it);
			}
			else
			{
				LOGERROR("{} with id={} does not exist", typeName, id);
			}
		}

		template<typename T>
		static T& Get(std::unordered_map<u32, T>& map, u32 id, const char* typeName)
		{
			auto it = map.find(id);

			LEOASSERTF(it != map.end(), "{} with id={} does not exist", typeName, id);

			return it->second;
		}
	private:
		// Stores our current "asset" id. This is incremented whenever a "asset" is added.
		u32 m_insertedAssetID = 1;

		std::unordered_map<u32, Mesh> m_mesh_map;
		std::unordered_map<u32, Texture> m_texture_map;
		std::unordered_map<u32, ShaderProgram> m_shader_map;
		std::unordered_map<u32, Model> m_model_map;
	};
}