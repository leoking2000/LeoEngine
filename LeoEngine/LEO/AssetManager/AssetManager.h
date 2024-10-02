#pragma once
#include "../Graphics/Mesh.h"
#include "../Graphics/Texture.h"
#include "../Graphics/Shader.h"

namespace LEO
{
	typedef u64 assetID;

	class AssetManager
	{
	public:
		AssetManager() = default;

		AssetManager(const AssetManager&) = delete;
		AssetManager& operator=(const AssetManager&) = delete;
	public:
		assetID  CreateMesh(DefaultMesh type);
		assetID  CreateMesh(VertexArray& va, IndexBuffer& ib, u32 layout_size);
		void     RemoveMesh(assetID id);
		Mesh&    GetMesh(assetID id);
	public:
		assetID  CreateTexture(const std::string& filepath);
		assetID  CreateTexture(u32 width, u32 height, TextureFormat format = TextureFormat::RGBA8UB, u8* data = nullptr);
		void     RemoveTexture(assetID id);
		Texture& GetTexuture(assetID id);
	public:
		assetID  CreateShader(const std::string& filepath);
		void     RemoveShader(assetID id);
		ShaderProgram& GetShader(assetID id);
		bool ReloadShader(const std::string& filepath, assetID id);
	private:
		template<typename T>
		static void Remove(std::unordered_map<assetID, T>& map, assetID id, const char* typeName)
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
		static T& Get(std::unordered_map<assetID, T>& map, assetID id, const char* typeName)
		{
			auto it = map.find(id);

			LEOASSERTF(it != map.end(), "{} with id={} does not exist", typeName, id);

			return it->second;
		}
	private:
		// Stores our current "asset" id. This is incremented whenever a "asset" is added.
		assetID m_insertedAssetID = 1;
	private:
		std::unordered_map<assetID, Mesh> m_mesh_map;
		std::unordered_map<assetID, Texture> m_texture_map;
		std::unordered_map<assetID, ShaderProgram> m_shader_map;
	};
}
