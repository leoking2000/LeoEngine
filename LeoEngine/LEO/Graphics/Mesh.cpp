#include <glad/glad.h>
#include <glm/glm.hpp>
#include "Mesh.h"

namespace LEO
{
	Mesh::Mesh(DefaultMesh shape)
		:
		Mesh(GenerateMesh(shape))
	{}

	Mesh::Mesh(VertexArray& va, IndexBuffer& ib, u32 layout_size)
		:
		m_vertexArray(std::move(va)),
		m_indexBuffer(std::move(ib)),
		m_layout_size(layout_size),
		m_count(0)
	{}



	Mesh::Mesh(Mesh&& other) noexcept
		:
		m_vertexArray(std::move(other.m_vertexArray)),
		m_indexBuffer(std::move(other.m_indexBuffer)),
		m_layout_size(other.m_layout_size),
		m_count(other.m_count)
	{}

	Mesh& Mesh::operator=(Mesh&& other) noexcept
	{
		m_vertexArray = std::move(other.m_vertexArray);
		m_indexBuffer = std::move(other.m_indexBuffer);
		m_layout_size = other.m_layout_size;
		m_count = other.m_count;

		m_layout_size = 0;
		m_count = 0;

		return *this;
	}


	void Mesh::Draw() const
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

	bool Mesh::HasInstanceArray() const
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
		const u32 numVertices = (prec + 1) * (prec + 1);
		const u32 numIndices = prec * prec * 6;
		f32*    vertices = new f32[numVertices];
		u32*    indices  = new u32[numIndices];

		// calculate triangle vertices
		for (u32 i = 0; i <= prec; i++)
		{
			for (u32 j = 0; j <= prec; j++)
			{
				if ((i * (prec + 1) + j) * 8 + 7 >= numVertices) break;

				float y = (float)glm::cos(glm::radians(180.0f - i * 180.0f / prec));
				float x = -(float)glm::cos(glm::radians(j * 360.0f / prec)) * (float)abs(cos(asin(y)));
				float z = (float)glm::sin(glm::radians(j * 360.0f / (float)(prec))) * (float)abs(cos(asin(y)));

				vertices[(i * (prec + 1) + j) * 8 + 0] = x;
				vertices[(i * (prec + 1) + j) * 8 + 1] = y;
				vertices[(i * (prec + 1) + j) * 8 + 2] = z;

				vertices[(i * (prec + 1) + j) * 8 + 3] = ((float)j / prec);
				vertices[(i * (prec + 1) + j) * 8 + 4] = ((float)i / prec);

				vertices[(i * (prec + 1) + j) * 8 + 5] = x;
				vertices[(i * (prec + 1) + j) * 8 + 6] = y;
				vertices[(i * (prec + 1) + j) * 8 + 7] = z;

				// TODO: Caculate tangent and bitangent
			}
		}

		// calculate triangle indices
		for (u32 i = 0; i < prec; i++)
		{
			for (u32 j = 0; j < prec; j++)
			{
				if (6 * (i * prec + j) + 5 >= numIndices) break;

				indices[6 * (i * prec + j) + 0] = i * (prec + 1) + j;
				indices[6 * (i * prec + j) + 1] = i * (prec + 1) + j + 1;
				indices[6 * (i * prec + j) + 2] = (i + 1) * (prec + 1) + j;
				indices[6 * (i * prec + j) + 3] = i * (prec + 1) + j + 1;
				indices[6 * (i * prec + j) + 4] = (i + 1) * (prec + 1) + j + 1;
				indices[6 * (i * prec + j) + 5] = (i + 1) * (prec + 1) + j;
			}
		}

		VertexBuffer vertexBuffer((const void*)vertices, (u32)(numVertices * sizeof(float)));
		ElementType arr[3] = { FLOAT3, FLOAT2, FLOAT3_N };
		Layout<3> layout(arr);
		VertexArray vertexArray;
		vertexArray.AddBuffer(vertexBuffer, layout);
		IndexBuffer indexBuffer(indices, (u32)numIndices);
		Mesh mesh{ vertexArray, indexBuffer, 3 };

		delete[] vertices;
		delete[] indices;

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
}
