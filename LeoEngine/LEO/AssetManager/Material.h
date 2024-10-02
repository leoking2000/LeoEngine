#pragma once
#include <glm/glm.hpp>
#include <optional>
#include "AssetRef.h"

namespace LEO
{
	class Material
	{
	public:
		glm::vec3 Albedo = glm::vec3(1.0f, 0.0f, 0.0f);
		f32 Roughness = 0.5f;
		f32 Metallic = 0.0f;
	public:
		TextureRef AlbedoMap;
	public:
		ShaderRef MaterialShader;
	public:
		void SetMaterialUniforms() const;
	};
}