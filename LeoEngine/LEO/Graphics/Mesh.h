#pragma once
#include <string>
#include "BufferObjects.h"

namespace LEO
{
	enum class DefaultMesh
	{
		CUBE,
		SPHERE,
		QUARD,
		SCRERN_FILLED_QUARD
	};

	class Mesh
	{
	public:
		Mesh(DefaultMesh shape);
		Mesh(VertexArray& va, IndexBuffer& ib, u32 layout_size);

		Mesh(const Mesh& other) = delete;
		Mesh& operator=(const Mesh&) = delete;

		Mesh(Mesh&& other) noexcept;
		Mesh& operator=(Mesh&& other) noexcept;

		~Mesh() = default;
	public:
		void Draw() const;
	public:
		static Mesh GenerateMesh(DefaultMesh shape);
		static Mesh GenarateCube();
		static Mesh GenarateSphere(u32 prec = 48);
		static Mesh GenarateQuard(u32 repet = 1);
		static Mesh GenarateScreenFilledQuard();
	public:
		bool HasInstanceArray() const;
		void MakeInstancedArray(const glm::mat4* model_arr, u32 count);
	public:
		u32 m_count = 0;
		u32 m_layout_size = 0;
		VertexArray m_vertexArray;
		IndexBuffer m_indexBuffer;
	private:
		Mesh() = default;
		friend class AssetManager;
	};
}
