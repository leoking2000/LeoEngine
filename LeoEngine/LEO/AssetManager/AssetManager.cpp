#include "../Utilities/FileUtilities.h"
#include "AssetManager.h"


namespace LEO
{
	AssetManager::AssetManager()
	{
		m_shaders.reserve(20);
		m_meshs.reserve(20);
		m_textures.reserve(20);
	}

	void AssetManager::RemoveAsset(assetID id, AssetType type)
	{
		switch (type)
		{
		case LEO::AssetType::SHADER:
			RemoveShader(id);
			break;
		case LEO::AssetType::MESH:
			RemoveMesh(id);
			break;
		case LEO::AssetType::TEXTURE:
			RemoveTexture(id);
			break;
		}
	}

	void* AssetManager::GetAsset(assetID id, AssetType type)
	{
		switch (type)
		{
		case LEO::AssetType::SHADER:
			return &GetShader(id);
		case LEO::AssetType::MESH:
			return &GetMesh(id);
		case LEO::AssetType::TEXTURE:
			return &GetTexuture(id);
		}
	}

	assetID AssetManager::CreateShader(const std::string& filepath)
	{
		for (i32 i = 0; i < m_shaders.size(); i++)
		{
			if (!m_shaders[i].IsValid())
			{
				m_shaders[i].Reload(filepath);
				return i;
			}
		}

		m_shaders.emplace_back(filepath);
		return (assetID)m_shaders.size() - 1;
	}

	assetID AssetManager::CreateMesh(DefaultMesh type)
	{
		for (i32 i = 0; i < m_meshs.size(); i++)
		{
			if (m_meshs[i].m_indexBuffer.GetCount() == 0)
			{
				m_meshs[i] = Mesh(type);
				return i;
			}
		}

		m_meshs.emplace_back(type);
		return (assetID)m_meshs.size() - 1;
	}

	assetID AssetManager::CreateMesh(VertexArray& va, IndexBuffer& ib, u32 layout_size)
	{
		for (i32 i = 0; i < m_meshs.size(); i++)
		{
			if (m_meshs[i].m_indexBuffer.GetCount() == 0)
			{
				m_meshs[i] = Mesh(va, ib, layout_size);
				return i;
			}
		}

		m_meshs.emplace_back(va, ib, layout_size);
		return (assetID)m_meshs.size() - 1;
	}

	assetID AssetManager::CreateTexture(const std::string& filepath)
	{
		ImageData image = ReadImageData(filepath);

		Texture tex = Texture(TextureDimensions::DIM_2D, glm::uvec3{ image.width, image.height, 0 }, TextureFormat::RGBA8UB,
			TextureMinFiltering::MIN_LINEAR_MIPMAP_LINEAR, TextureMagFiltering::MAG_LINEAR,
			TextureWrapping::REPEAT, TextureWrapping::REPEAT, true, image.data);

		FreeImageData(image);

		for (i32 i = 0; i < m_textures.size(); i++)
		{
			if (m_textures[i].ID() == 0)
			{
				m_textures[i] = std::move(tex);
				return i;
			}
		}
	
		m_textures.emplace_back(std::move(tex));
		return (assetID)m_textures.size() - 1;
	}

	assetID AssetManager::CreateTexture(u32 width, u32 height, TextureFormat format, u8* data)
	{
		Texture tex = Texture(width, height, format, data);

		for (i32 i = 0; i < m_textures.size(); i++)
		{
			if (m_textures[i].ID() == 0)
			{
				m_textures[i] = std::move(tex);
				return i;
			}
		}

		m_textures.emplace_back(std::move(tex));
		return (assetID)m_textures.size() - 1;
	}


	void AssetManager::RemoveShader(assetID id)
	{
		m_shaders[id] = {};
	}

	void AssetManager::RemoveMesh(assetID id)
	{
		m_meshs[id] = {};
	}

	void AssetManager::RemoveTexture(assetID id)
	{
		m_textures[id] = {};
	}

	ShaderProgram& AssetManager::GetShader(assetID id)
	{
		return m_shaders[id];
	}

	Mesh& AssetManager::GetMesh(assetID id)
	{
		return m_meshs[id];
	}

	Texture& AssetManager::GetTexuture(assetID id)
	{
		return m_textures[id];
	}

	void AssetManager::ReloadAllShaders()
	{
		for (ShaderProgram& shader : m_shaders)
		{
			if (shader.IsValid())
			{
				shader.Reload();
			}
		}
	}

}