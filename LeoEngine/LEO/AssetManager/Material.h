#pragma once
#include <glm/glm.hpp>
#include "../Graphics/Shader.h"

namespace LEO
{
	class AssetManager;

	class Material
	{
	public:
		glm::vec3 Albedo = glm::vec3(1.0f, 0.0f, 0.0f);
		f32 Roughness = 0.5f;
		f32 Metallic = 0.0f;
	public:
		u32 AlbedoMap    = 0;
		//u32 NormalMap    = 0;	
		//u32 RoughnessMap = 0;		
		//u32 MetallicMap  = 0;
	public:
		void SetMaterialUniforms(const ShaderProgram& shader, AssetManager& assetManager) const;
	};
}