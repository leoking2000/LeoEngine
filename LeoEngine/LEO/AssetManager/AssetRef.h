#pragma once
#include "AssetManager.h"

namespace LEO
{

#define ZZ_ASSET_ARROW_OPERATOR(type, get) type* operator->() { return &m_manager->get(m_asset_id); }
#define ZZ_ASSET_ARROW_OPERATOR_CONST(type, get) const type* operator->() const { return &m_manager->get(m_asset_id); }

#define ZZ_ASSET_ISNULL() bool IsNull() const { return m_asset_id == 0; }

#define ZZ_ASSET_BOOL() operator bool() const { return !IsNull(); }

#define ZZ_ASSET_CLASS(name, type, get)\
	class name\
	{\
		public:\
			name(assetID id, AssetManager* manager) : m_asset_id(id), m_manager(manager) {}\
		public:\
			ZZ_ASSET_ARROW_OPERATOR(type, get)\
			ZZ_ASSET_ARROW_OPERATOR_CONST(type, get)\
			ZZ_ASSET_ISNULL()\
			ZZ_ASSET_BOOL()\
		private:\
			assetID m_asset_id;\
			AssetManager* m_manager;\
	}

	ZZ_ASSET_CLASS(ShaderRef , ShaderProgram, GetShader);

	ZZ_ASSET_CLASS(TextureRef, Texture      , GetTexuture);

	ZZ_ASSET_CLASS(MeshRef   , Mesh         , GetMesh);
}
