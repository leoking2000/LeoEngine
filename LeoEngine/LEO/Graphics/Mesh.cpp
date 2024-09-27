#include <glad/glad.h>
#include <glm/glm.hpp>
#include <tiny_obj_loader.h>
#include "Mesh.h"

struct Vertex
{
	glm::vec3 pos;
	glm::vec2 texCord;
	glm::vec3 normal;
	//glm::vec3 tangent;
	//glm::vec3 bitangent;

	bool operator==(const Vertex& rhs)
	{
		return pos == rhs.pos &&
			texCord == rhs.texCord &&
			normal == rhs.normal;//&&
			//tangent == rhs.tangent &&
			//bitangent == rhs.bitangent;
	}
};

namespace LEO
{
	Mesh::Mesh(DefaultMesh shape)
		:
		Mesh(GenerateMesh(shape))
	{}

	Mesh::Mesh(DefaultMesh shape, const glm::mat4* model_arr, u32 count)
		:
		Mesh(GenerateMesh(shape))
	{
		LEOASSERT(model_arr != nullptr, "model array is null");
		LEOASSERT(count != 0, "count can't be 0");

		MakeInstancedArray(model_arr, count);
	}

	Mesh::Mesh(VertexArray& va, IndexBuffer& ib, u32 layout_size)
		:
		m_vertexArray(std::move(va)),
		m_indexBuffer(std::move(ib)),
		m_layout_size(layout_size),
		m_count(0)
	{}

	Mesh::Mesh(VertexArray& va, IndexBuffer& ib, u32 layout_size, const glm::mat4* model_arr, u32 count)
		:
		m_vertexArray(std::move(va)),
		m_indexBuffer(std::move(ib)),
		m_layout_size(layout_size),
		m_count(0)
	{
		LEOASSERT(model_arr != nullptr, "model array is null");
		LEOASSERT(count != 0, "count can't be 0");

		MakeInstancedArray(model_arr, count);
	}

	Mesh::Mesh(Mesh&& other)
		:
		m_vertexArray(std::move(other.m_vertexArray)),
		m_indexBuffer(std::move(other.m_indexBuffer)),
		m_layout_size(other.m_layout_size),
		m_count(other.m_count)
	{}

	Mesh& Mesh::operator=(Mesh&& other)
	{
		m_vertexArray = std::move(other.m_vertexArray);
		m_indexBuffer = std::move(other.m_indexBuffer);
		m_layout_size = other.m_layout_size;
		m_count = other.m_count;

		m_layout_size = 0;
		m_count = 0;

		return *this;
	}

	void Mesh::Draw()
	{
		m_vertexArray.Bind();
		m_indexBuffer.Bind();

		if (m_count == 0)
		{
			glDrawElements(GL_TRIANGLES, m_indexBuffer.GetCount(), GL_UNSIGNED_INT, nullptr);
		}
		else
		{
			glDrawElementsInstanced(GL_TRIANGLES, m_indexBuffer.GetCount(),
				GL_UNSIGNED_INT, nullptr, m_count);
		}
		
		m_vertexArray.UnBind();
		m_indexBuffer.UnBind();
	}

	bool Mesh::HasInstanceArray()
	{
		return m_count != 0;
	}

	void Mesh::MakeInstancedArray(const glm::mat4* model_arr, u32 count)
	{
		LEOASSERT(model_arr != nullptr, "model array is null");
		LEOASSERT(count != 0, "count can not be zero");

		m_count = count;

		LEO::VertexBuffer model_VBO(model_arr, m_count * sizeof(glm::mat4));
		LEO::ElementType layout_arr[1] = { LEO::MAT4 };
		LEO::Layout<1> layout(layout_arr);
		m_vertexArray.AddBuffer(model_VBO, layout, m_layout_size, true);
		m_layout_size += 4;
	}

	Mesh Mesh::GenerateMesh(DefaultMesh shape)
	{
		switch (shape)
		{
			case DefaultMesh::CUBE:                return GenarateCube();
			case DefaultMesh::SPHERE:              return GenarateSphere();
			case DefaultMesh::QUARD:               return GenarateQuard();
			case DefaultMesh::SCRERN_FILLED_QUARD: return GenarateScreenFilledQuard();
		}
	}

	Mesh Mesh::GenarateCube()
	{
		float vertexs[] = {
			// pos                 // tex cord      // normal               // Tangent           //Bitangent
			// Forward
			-0.5f, -0.5f, -0.5f,    0.0f, 0.0f,     0.0f,  0.0f, -1.0f,     1.0f, 0.0f, 0.0f,   0.0f, 1.0f, 0.0f,//0
			-0.5f,  0.5f, -0.5f,    0.0f, 1.0f,     0.0f,  0.0f, -1.0f,     1.0f, 0.0f, 0.0f,   0.0f, 1.0f, 0.0f,//1
			 0.5f,  0.5f, -0.5f,    1.0f, 1.0f,     0.0f,  0.0f, -1.0f,     1.0f, 0.0f, 0.0f,   0.0f, 1.0f, 0.0f,//2
			 0.5f, -0.5f, -0.5f,    1.0f, 0.0f,     0.0f,  0.0f, -1.0f,     1.0f, 0.0f, 0.0f,   0.0f, 1.0f, 0.0f,//3

			// EAST							  
			 0.5f,  0.5f, -0.5f,     0.0f, 1.0f,    1.0f,  0.0f,  0.0f,     0.0f, 0.0f, 1.0f,   0.0f, 1.0f, 0.0f,//4
			 0.5f, -0.5f, -0.5f,     0.0f, 0.0f,    1.0f,  0.0f,  0.0f,     0.0f, 0.0f, 1.0f,   0.0f, 1.0f, 0.0f,//5
			 0.5f,  0.5f,  0.5f,     1.0f, 1.0f,    1.0f,  0.0f,  0.0f,     0.0f, 0.0f, 1.0f,   0.0f, 1.0f, 0.0f,//6
			 0.5f, -0.5f,  0.5f,     1.0f, 0.0f,    1.0f,  0.0f,  0.0f,     0.0f, 0.0f, 1.0f,   0.0f, 1.0f, 0.0f,//7

			// BACK							  
			 0.5f,  0.5f,  0.5f,     0.0f, 1.0f,    0.0f,  0.0f, 1.0f,     -1.0f, 0.0f, 0.0f,   0.0f, 1.0f, 0.0f,//8
			 0.5f, -0.5f,  0.5f,     0.0f, 0.0f,    0.0f,  0.0f, 1.0f,     -1.0f, 0.0f, 0.0f,   0.0f, 1.0f, 0.0f,//9
			-0.5f,  0.5f,  0.5f,     1.0f, 1.0f,    0.0f,  0.0f, 1.0f,     -1.0f, 0.0f, 0.0f,   0.0f, 1.0f, 0.0f,//10
			-0.5f, -0.5f,  0.5f,     1.0f, 0.0f,    0.0f,  0.0f, 1.0f,     -1.0f, 0.0f, 0.0f,   0.0f, 1.0f, 0.0f,//11

			// WEST							  
			-0.5f, -0.5f, -0.5f,     1.0f, 0.0f,   -1.0f,  0.0f,  0.0f,      0.0f, 0.0f, -1.0f,   0.0f, 1.0f, 0.0f,//12
			-0.5f,  0.5f, -0.5f,     1.0f, 1.0f,   -1.0f,  0.0f,  0.0f,      0.0f, 0.0f, -1.0f,   0.0f, 1.0f, 0.0f,//13
			-0.5f,  0.5f,  0.5f,     0.0f, 1.0f,   -1.0f,  0.0f,  0.0f,      0.0f, 0.0f, -1.0f,   0.0f, 1.0f, 0.0f,//14
			-0.5f, -0.5f,  0.5f,     0.0f, 0.0f,   -1.0f,  0.0f,  0.0f,      0.0f, 0.0f, -1.0f,   0.0f, 1.0f, 0.0f,//15

			// NORTH						  
			-0.5f,  0.5f, -0.5f,     0.0f, 0.0f,    0.0f,  1.0f,  0.0f,      1.0f, 0.0f, 0.0f,   0.0f,  0.0f,  1.0f,//16
			 0.5f,  0.5f, -0.5f,     0.0f, 1.0f,    0.0f,  1.0f,  0.0f,      1.0f, 0.0f, 0.0f,   0.0f,  0.0f,  1.0f,//17
			 0.5f,  0.5f,  0.5f,     1.0f, 1.0f,    0.0f,  1.0f,  0.0f,      1.0f, 0.0f, 0.0f,   0.0f,  0.0f,  1.0f,//18
			-0.5f,  0.5f,  0.5f,     1.0f, 0.0f,    0.0f,  1.0f,  0.0f,      1.0f, 0.0f, 0.0f,   0.0f,  0.0f,  1.0f,//19

			// SOUTH						  
			-0.5f, -0.5f, -0.5f,     0.0f, 0.0f,    0.0f, -1.0f,  0.0f,      -1.0f, 0.0f, 0.0f,   0.0f,  0.0f, -1.0f,//20
			 0.5f, -0.5f, -0.5f,     1.0f, 0.0f,    0.0f, -1.0f,  0.0f,      -1.0f, 0.0f, 0.0f,   0.0f,  0.0f, -1.0f,//21
			 0.5f, -0.5f,  0.5f,     1.0f, 1.0f,    0.0f, -1.0f,  0.0f,      -1.0f, 0.0f, 0.0f,   0.0f,  0.0f, -1.0f,//22
			-0.5f, -0.5f,  0.5f,     0.0f, 1.0f,    0.0f, -1.0f,  0.0f,		 -1.0f, 0.0f, 0.0f,   0.0f,  0.0f, -1.0f //23
		};

		VertexBuffer vertexBuffer(vertexs, sizeof(vertexs));

		ElementType arr[5] = { FLOAT3, FLOAT2, FLOAT3_N, FLOAT3_N, FLOAT3_N };
		Layout<5> layout(arr);

		VertexArray vertexArray;
		vertexArray.AddBuffer(vertexBuffer, layout);

		// index buffer
		uint32_t indices[] = {
			// Forward
			0, 1, 2, /**/ 0, 2, 3,
			// EAST
			5, 4, 6, /**/ 5, 6, 7,
			// BACK
			9, 8, 10, /**/ 9, 10, 11,
			// WEST
			15, 14, 13, /**/ 15, 13, 12,
			// NORTH
			16, 19, 18, /**/ 16, 18, 17,
			// SOUTH
			22, 23, 20, /**/ 22, 20, 21
		};
		IndexBuffer indexBuffer(indices, 36);

		Mesh mesh{ vertexArray, indexBuffer, 5 };

		return mesh;
	}

	Mesh Mesh::GenarateSphere(u32 prec)
	{
		const uint32_t numVertices = (prec + 1) * (prec + 1);
		const uint32_t numIndices = prec * prec * 6;

		std::vector<Vertex> vertices(numVertices);

		// calculate triangle vertices
		for (u32 i = 0; i <= prec; i++)
		{
			for (u32 j = 0; j <= prec; j++)
			{
				float y = (float)glm::cos(glm::radians(180.0f - i * 180.0f / prec));
				float x = -(float)glm::cos(glm::radians(j * 360.0f / prec)) * (float)abs(cos(asin(y)));
				float z = (float)glm::sin(glm::radians(j * 360.0f / (float)(prec))) * (float)abs(cos(asin(y)));

				vertices[i * (prec + 1) + j].pos = glm::vec3(x, y, z);
				vertices[i * (prec + 1) + j].texCord = glm::vec2(((float)j / prec), ((float)i / prec));
				vertices[i * (prec + 1) + j].normal = glm::vec3(x, y, z);

				// TODO: Caculate tangent and bitangent
				//vertices[i * (prec + 1) + j].tangent = glm::vec3(x, y, z);
				//vertices[i * (prec + 1) + j].bitangent = glm::vec3(x, y, z);
			}
		}

		std::vector<uint32_t> indices(numIndices);

		// calculate triangle indices
		for (u32 i = 0; i < prec; i++)
		{
			for (u32 j = 0; j < prec; j++)
			{
				indices[6 * (i * prec + j) + 0] = i * (prec + 1) + j;
				indices[6 * (i * prec + j) + 1] = i * (prec + 1) + j + 1;
				indices[6 * (i * prec + j) + 2] = (i + 1) * (prec + 1) + j;
				indices[6 * (i * prec + j) + 3] = i * (prec + 1) + j + 1;
				indices[6 * (i * prec + j) + 4] = (i + 1) * (prec + 1) + j + 1;
				indices[6 * (i * prec + j) + 5] = (i + 1) * (prec + 1) + j;
			}
		}

		std::vector<float> vertex_buffer;
		vertex_buffer.reserve(numVertices * 8);

		for (Vertex& v : vertices)
		{
			vertex_buffer.emplace_back(v.pos.x);
			vertex_buffer.emplace_back(v.pos.y);
			vertex_buffer.emplace_back(v.pos.z);

			vertex_buffer.emplace_back(v.texCord.s);
			vertex_buffer.emplace_back(v.texCord.t);

			vertex_buffer.emplace_back(v.normal.x);
			vertex_buffer.emplace_back(v.normal.y);
			vertex_buffer.emplace_back(v.normal.z);

			//vertex_buffer.emplace_back(v.tangent.x);
			//vertex_buffer.emplace_back(v.tangent.y);
			//vertex_buffer.emplace_back(v.tangent.z);

			//vertex_buffer.emplace_back(v.bitangent.x);
			//vertex_buffer.emplace_back(v.bitangent.y);
			//vertex_buffer.emplace_back(v.bitangent.z);
		}

		VertexBuffer vertexBuffer((const void*)vertex_buffer.data(), (u32)(vertex_buffer.size() * sizeof(float)));

		ElementType arr[3] = { FLOAT3, FLOAT2, FLOAT3_N };
		Layout<3> layout(arr);

		VertexArray vertexArray;
		vertexArray.AddBuffer(vertexBuffer, layout);


		IndexBuffer indexBuffer(indices.data(), (u32)indices.size());

		Mesh mesh{ vertexArray, indexBuffer, 3 };

		return mesh;
	}

	Mesh Mesh::GenarateQuard(u32 repet)
	{
		float vertexs[] = {
			// pos                 // tex cord                      // normal			// tan				// bi
			 0.5f,  0.5f, 0.0f,    (float)repet, (float)repet,     0.0f,  0.0f, 1.0f,  -1.0f,  0.0f, 0.0f,	0.0f,  -1.0f, 0.0f,//0
			 0.5f, -0.5f, 0.0f,    (float)repet,         0.0f,     0.0f,  0.0f, 1.0f,  -1.0f,  0.0f, 0.0f,	0.0f,  -1.0f, 0.0f,//1
			-0.5f, -0.5f, 0.0f,		       0.0f,         0.0f,     0.0f,  0.0f, 1.0f,  -1.0f,  0.0f, 0.0f,	0.0f,  -1.0f, 0.0f,//2
			-0.5f,  0.5f, 0.0f,            0.0f, (float)repet,     0.0f,  0.0f, 1.0f,  -1.0f,  0.0f, 0.0f,	0.0f,  -1.0f, 0.0f //3
		};

		VertexBuffer vertexBuffer(vertexs, sizeof(vertexs));

		ElementType arr[5] = { FLOAT3, FLOAT2, FLOAT3_N, FLOAT3_N, FLOAT3_N };
		Layout<5> layout(arr);

		VertexArray vertexArray;
		vertexArray.AddBuffer(vertexBuffer, layout);

		// index buffer
		uint32_t indices[] = {
			0, 1, 2,
			3, 0, 2
		};

		IndexBuffer indexBuffer(indices, 6);

		Mesh mesh{ vertexArray, indexBuffer, 5 };

		return mesh;
	}

	Mesh Mesh::GenarateScreenFilledQuard()
	{
		float vertexs[] = {
			// pos      
			 1.0f,  1.0f,
			-1.0f,  1.0f,
			-1.0f, -1.0f,
			 1.0f, -1.0f
		};

		VertexBuffer vertexBuffer(vertexs, sizeof(vertexs));
		ElementType arr[1] = { FLOAT2 };
		Layout<1> layout(arr);

		VertexArray vertexArray;
		vertexArray.AddBuffer(vertexBuffer, layout);

		// index buffer
		uint32_t indices[] = {
			0, 1, 2,
			0, 2, 3
		};

		IndexBuffer indexBuffer(indices, 6);

		Mesh mesh{ vertexArray, indexBuffer, 1 };
		mesh.m_layout_size = 1;

		return mesh;
	}

	std::vector<Mesh> Mesh::LoadFromFile(const std::string& filepath)
	{
		LEOASSERT(false, "NOT YET IMPLEMETED!!!");

		return std::vector<Mesh>();
	}
}
