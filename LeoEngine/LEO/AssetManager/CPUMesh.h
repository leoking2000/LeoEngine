#pragma once
#include "Material.h"
#include "../Graphics/Mesh.h"

namespace LEO
{
	struct Vertex
	{
		glm::vec3 pos;
		glm::vec2 texCord;
		glm::vec3 normal;
		glm::vec3 tangent;
		glm::vec3 bitangent;

		bool operator==(const Vertex& rhs)
		{
			return pos == rhs.pos &&
				texCord == rhs.texCord &&
				normal == rhs.normal &&
				tangent == rhs.tangent &&
				bitangent == rhs.bitangent;
		}
	};

	class CPUMesh
	{
	public:
		std::vector<Vertex> vertices;
		std::vector<u32> indices;
		Material material;
	public:
		static std::vector<CPUMesh> Load(const std::string& filename, AssetManager& assetManager);
		static Mesh Convert(const CPUMesh& cpu_mesh);
	};
}