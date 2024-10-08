#pragma once
#include "AssetManager.h"

namespace LEO
{
	template<typename T>
	class AssetRef
	{
	public:
		static AssetRef<ShaderProgram> CreateShader(AssetManager& assetManager, const std::string& filepath)
		{
			assetID id = assetManager.CreateShader(filepath);
			return  AssetRef<ShaderProgram>(id, AssetType::SHADER, &assetManager);
		}

		static AssetRef<Mesh> CreateMesh(AssetManager& assetManager, DefaultMesh type)
		{
			assetID id = assetManager.CreateMesh(type);
			return  AssetRef<Mesh>(id, AssetType::MESH, &assetManager);
		}
		static AssetRef<Mesh> CreateMesh(AssetManager& assetManager, VertexArray& va, IndexBuffer& ib, u32 layout_size)
		{
			assetID id = assetManager.CreateMesh(va, ib, layout_size);
			return  AssetRef<Mesh>(id, AssetType::MESH, &assetManager);
		}

		static AssetRef<Texture> CreateTexture(AssetManager& assetManager, const std::string& filepath)
		{
			assetID id = assetManager.CreateTexture(filepath);
			return  AssetRef<Texture>(id, AssetType::TEXTURE, &assetManager);
		}
		static AssetRef<Texture> CreateTexture(AssetManager& assetManager, 
			u32 width, u32 height, TextureFormat format = TextureFormat::RGBA8UB, u8* data = nullptr)
		{
			assetID id = assetManager.CreateTexture(width, height, format, data);
			return  AssetRef<Texture>(id, AssetType::TEXTURE, &assetManager);
		}
	public:
		AssetRef() = default;
	public:
		inline bool IsNull() const { return m_manager == nullptr; }
		inline assetID ID() const { return m_asset_id; }
		inline AssetType Type() const { return m_asset_type; }
	public:
		inline T& Get() { (T&)*m_manager->GetAsset(m_asset_id, m_asset_type); }
		inline const T& Get() const { (const T&)*m_manager->GetAsset(m_asset_id, m_asset_type); }
	public:
		inline void Nullify() { m_manager = nullptr; }
	public:
		operator bool() const { return !IsNull(); }
		T* operator->() { return &Get(); }
		const T* operator->() const { return &Get(); }
	private:
		AssetRef(assetID id, AssetType type, AssetManager* assetManager)
			:
			m_asset_id(id),
			m_asset_type(type),
			m_assetManager(assetManager)
		{}
	private:
		assetID m_asset_id = 0;
		AssetType m_asset_type = AssetType::SHADER;
		AssetManager* m_manager = nullptr;
	};

	typedef AssetRef<ShaderProgram> ShaderRef;
	typedef AssetRef<Mesh> MeshRef;
	typedef AssetRef<Texture> TextureRef;
}