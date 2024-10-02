#include <stb/stb_image.h>
#include "AssetManager.h"

namespace LEO
{
	assetID AssetManager::CreateMesh(DefaultMesh type)
	{
		auto inserted = m_mesh_map.insert(std::make_pair(m_insertedAssetID, Mesh(type)));
		m_insertedAssetID++;

		return m_insertedAssetID - 1;
	}

	assetID AssetManager::CreateMesh(VertexArray& va, IndexBuffer& ib, u32 layout_size)
	{
		auto inserted = m_mesh_map.insert(std::make_pair(m_insertedAssetID, Mesh(va, ib, layout_size)));
		m_insertedAssetID++;

		return m_insertedAssetID - 1;
	}

	void AssetManager::RemoveMesh(assetID id)
	{
		Remove<Mesh>(m_mesh_map, id, "Mesh");
	}

	Mesh& AssetManager::GetMesh(assetID id)
	{
		return Get<Mesh>(m_mesh_map, id, "Mesh");
	}

	assetID AssetManager::CreateTexture(const std::string& filepath)
	{
		stbi_set_flip_vertically_on_load(1);

		int width;
		int height;
		int bpp;

		stbi_uc* data = stbi_load((filepath).c_str(), &width, &height, &bpp, 4);
		bool fail = false;

		if (data == nullptr)
		{
			LOGERROR("Failed to Created Texture From: {}", filepath);
			stbi_uc data_error[] = {
					255, 255, 255, 255, /**/  0,   0,   0, 255,
					  0,   0,   0, 255,     255, 255, 255, 255
			};
			width = 2;
			height = 2;
			data = data_error;
			fail = true;
		}

		Texture tex(DIM_2D, { width, height, 0 }, TextureFormat::RGBA8UB,
			TextureMinFiltering::MIN_LINEAR_MIPMAP_LINEAR, TextureMagFiltering::MAG_LINEAR,
			TextureWrapping::REPEAT, TextureWrapping::REPEAT, data);
		if (!fail) { stbi_image_free(data); };

		auto inserted = m_texture_map.insert(std::make_pair(m_insertedAssetID, std::move(tex)));
		m_insertedAssetID++;

		return m_insertedAssetID - 1;
	}

	assetID AssetManager::CreateTexture(u32 width, u32 height, TextureFormat format, u8* data)
	{
		auto inserted = m_texture_map.insert(std::make_pair(m_insertedAssetID, Texture(width, height, format, data)));
		m_insertedAssetID++;

		return m_insertedAssetID - 1;
	}

	void AssetManager::RemoveTexture(assetID id)
	{
		Remove<Texture>(m_texture_map, id, "Texture");
	}

	Texture& AssetManager::GetTexuture(assetID id)
	{
		return Get<Texture>(m_texture_map, id, "Texture");
	}


	assetID AssetManager::CreateShader(const std::string& filepath)
	{
		auto inserted = m_shader_map.insert(std::make_pair(m_insertedAssetID, ShaderProgram(filepath)));
		m_insertedAssetID++;

		return m_insertedAssetID - 1;
	}

	void AssetManager::RemoveShader(assetID id)
	{
		Remove<ShaderProgram>(m_shader_map, id, "Shader");
	}

	ShaderProgram& AssetManager::GetShader(assetID id)
	{
		Get<ShaderProgram>(m_shader_map, id, "Shader");
	}

	bool AssetManager::ReloadShader(const std::string& filepath, assetID id)
	{
		return GetShader(id).Reload(filepath);
	}


}