#pragma once
#include <vector>
#include "../Utilities/Log.h"
#include "../Graphics/Shader.h"
#include "../Graphics/Mesh.h"
#include "../Graphics/Texture.h"

namespace LEO
{
	typedef u16 assetID;

	enum class AssetType : u16
	{
		SHADER  = 0,
		MESH    = 1,
		TEXTURE = 3
	};

	class AssetManager
	{
	public:
		AssetManager();

		AssetManager(const AssetManager&) = delete;
		AssetManager& operator=(const AssetManager&) = delete;
	public:
		void    RemoveAsset(assetID id, AssetType type);
		void*   GetAsset(assetID id, AssetType type);
	public:
		assetID  CreateShader(const std::string& filepath);
		assetID  CreateMesh(DefaultMesh type);
		assetID  CreateMesh(VertexArray& va, IndexBuffer& ib, u32 layout_size);
		assetID  CreateTexture(const std::string& filepath);
		assetID  CreateTexture(u32 width, u32 height, TextureFormat format = TextureFormat::RGBA8UB, u8* data = nullptr);
	public:
		void     RemoveShader(assetID id);
		void     RemoveMesh(assetID id);
		void     RemoveTexture(assetID id);
	public:
		ShaderProgram& GetShader(assetID id);
		Mesh& GetMesh(assetID id);
		Texture& GetTexuture(assetID id);
	public:
		void ReloadAllShaders();
	private:
		std::vector<ShaderProgram> m_shaders;
		std::vector<Mesh> m_meshs;
		std::vector<Texture> m_textures;
	};
}
