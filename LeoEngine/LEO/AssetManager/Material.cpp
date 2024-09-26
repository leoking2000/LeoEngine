#include "Material.h"
#include "../AssetManager/AssetManager.h"

#define U_ALBEDO          "u_albedo"
#define U_ALBEDO_MAP      "u_albedoMap"
#define U_HAS_ALBEDO      "u_hasAlbedoMap"
#define U_NORMAL_MAP      "u_normalMap"
#define U_HAS_NORMAL_MAP  "u_hasNormalMap"
#define U_ROUGHNESS       "u_roughness"
#define U_METALLIC        "u_metallic"

namespace LEO
{
	void Material::SetMaterialUniforms(const ShaderProgram& shader, AssetManager& assetManager) const
	{
		if (AlbedoMap != 0)
		{
			assetManager.GetTexture(AlbedoMap).Bind(0);
			shader.SetUniform(U_ALBEDO_MAP, 0);
		}

		shader.SetUniform(U_ALBEDO, Albedo);
		shader.SetUniform(U_HAS_ALBEDO, AlbedoMap == 0 ? 0 : 1);

		//if (NormalMap != 0)
		//{
			//assetManager.GetTexture(NormalMap).Bind(1);
			//shader.SetUniform(U_NORMAL_MAP, 1);
		//}
		//shader.SetUniform(U_HAS_NORMAL_MAP, NormalMap == 0 ? 0 : 1);

		//shader.SetUniform(U_ROUGHNESS, Roughness);
		//shader.SetUniform(U_METALLIC, Metallic);
	}
}