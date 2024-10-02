#include "Material.h"

#define U_ALBEDO          "u_albedo"
#define U_ALBEDO_MAP      "u_albedoMap"
#define U_HAS_ALBEDO      "u_hasAlbedoMap"
#define U_NORMAL_MAP      "u_normalMap"
#define U_HAS_NORMAL_MAP  "u_hasNormalMap"
#define U_ROUGHNESS       "u_roughness"
#define U_METALLIC        "u_metallic"

namespace LEO
{
	void Material::SetMaterialUniforms() const
	{
		if (MaterialShader.IsNull()) return;

		if (AlbedoMap)
		{
			AlbedoMap->Bind(0);
			MaterialShader->SetUniform(U_ALBEDO_MAP, 0);
		}

		MaterialShader->SetUniform(U_ALBEDO, Albedo);
		MaterialShader->SetUniform(U_HAS_ALBEDO, AlbedoMap.IsNull() ? 0 : 1);

		MaterialShader->SetUniform(U_ROUGHNESS, Roughness);
		MaterialShader->SetUniform(U_METALLIC, Metallic);
	}
}