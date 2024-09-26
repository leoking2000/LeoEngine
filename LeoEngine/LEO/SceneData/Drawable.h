#pragma once
#include "../AssetManager/AssetManager.h"
#include "Transform.h"


namespace LEO
{
	class Drawable
	{
	public:
		Drawable(u32 model, AssetManager& assetManager);
	public:
		Transform& trasform();
		Material&  material(u32 i = 0);
		Mesh&      mesh(u32 i = 0);
		Model&     model();
	public:
		void       Draw(const ShaderProgram& shader, const glm::mat4& proj_view, bool use_mat) const;
	private:
		Transform m_trasfrom;
		u32 m_model;
	private:
		AssetManager& m_assetManager;
	};
}