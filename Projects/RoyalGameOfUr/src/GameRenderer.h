#pragma once
#include <LEO/LeoEngine.h>

class GameRenderer
{
public:
	struct ObjectType
	{
		u32 mesh_id;
		u32 albedo_id;
		u32 count;
	};
public:
	void Init(LEO::AssetManager* assetManager, u32 width, u32 height);

	void Resize(u32 width, u32 height);

	void AddObjectType(ObjectType t, glm::mat4* model_arr);

	void Render(const LEO::Camera& camera);
private:
	void GeometryPass(const LEO::Camera& camera);
	void ShadingPass(const LEO::Camera& camera);
private:
	std::vector<ObjectType> m_objectTypes;
	LEO::AssetManager* m_assetManager;

	glm::mat4 m_proj;

	u32 m_geometry_shader;
	std::unique_ptr<LEO::FrameBuffer> m_geometry_framebuffer;

	u32 m_screen_filled_quad;
	u32 m_shading_shader;
	u32 m_postProsses_shader;
};