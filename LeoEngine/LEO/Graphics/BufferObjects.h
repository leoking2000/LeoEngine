#pragma once
#include <vector>
#include <glm/glm.hpp>
#include "../Utilities/Types.h"
#include "../Utilities/Log.h"

namespace LEO
{
	class VertexBuffer
	{
	public:
		VertexBuffer(const void* data, u32 size);

		VertexBuffer(const VertexBuffer& other) = delete;
		VertexBuffer& operator=(const VertexBuffer&) = delete;

		VertexBuffer(VertexBuffer&& other);
		VertexBuffer& operator=(VertexBuffer&& other);

		~VertexBuffer();
	public:
		void Bind() const;
		void UnBind() const;
	private:
		u32 m_id;
	};

	class IndexBuffer
	{
	public:
		IndexBuffer(const u32* data, u32 count);

		IndexBuffer(const IndexBuffer& other) = delete;
		IndexBuffer& operator=(const IndexBuffer& other) = delete;

		IndexBuffer(IndexBuffer&& other);
		IndexBuffer& operator=(IndexBuffer&& other);

		~IndexBuffer();
	public:
		void Bind() const;
		void UnBind() const;

		inline u32 GetCount() const { return m_count; }
	private:
		u32 m_id;
		u32 m_count;
	};

	/*
	* Vertex attribute type
	* _N is for normalize
	*/
	enum ElementType
	{
		// a float number
		FLOAT1 = 0,
		FLOAT1_N = 1,

		// 2d float vector
		FLOAT2 = 2,
		FLOAT2_N = 3,

		// 3d float vector
		FLOAT3 = 4,
		FLOAT3_N = 5,

		// 4d float vector
		FLOAT4 = 6,
		FLOAT4_N = 7,

		// 3d unsigned char vector
		UCHAR3 = 8,
		UCHAR3_N = 9,

		// 4d unsigned char vector
		UCHAR4 = 10,
		UCHAR4_N = 11,

		MAT4 = 12
	};

	/*
	* Represents the attribute layout of a VBO
	*/
	template<u32 ELEMENTS_COUNT>
	class Layout
	{
	public:
		Layout(ElementType* arr)
		{
			LEOASSERT(arr != nullptr, "Element type array is null.");
			m_stride = 0;

			for (u32 i = 0; i < ELEMENTS_COUNT; i++)
			{
				m_arr[i] = arr[i];

				switch (m_arr[i])
				{
				case FLOAT1:
				case FLOAT1_N:
					m_stride += 1 * sizeof(float);
					break;
				case FLOAT2:
				case FLOAT2_N:
					m_stride += 2 * sizeof(float);
					break;
				case FLOAT3:
				case FLOAT3_N:
					m_stride += 3 * sizeof(float);
					break;
				case FLOAT4:
				case FLOAT4_N:
					m_stride += 4 * sizeof(float);
					break;

				case UCHAR3:
				case UCHAR3_N:
					m_stride += 3 * sizeof(char);
					break;
				case UCHAR4:
				case UCHAR4_N:
					m_stride += 4 * sizeof(char);
					break;

				case MAT4:
					m_stride += 4 * sizeof(glm::vec4);
					break;
				default:
					LEOASSERT(false, "Unknown Element Type");
					break;
				}
			}
		}

		inline ElementType operator[](int i) const
		{
			return m_arr[i];
		}

		inline u32 GetStride() const
		{
			return m_stride;
		}

		inline u32 GetCount() const { return ELEMENTS_COUNT; }
	private:
		ElementType m_arr[ELEMENTS_COUNT];
		u32 m_stride;
	};

	class VertexArray
	{
	public:
		VertexArray();

		VertexArray(const VertexArray& other) = delete;
		VertexArray& operator=(const VertexArray) = delete;

		VertexArray(VertexArray&& other);
		VertexArray& operator=(VertexArray&& other);

		~VertexArray();
	public:
		void Bind() const;
		void UnBind() const;

		template<u32 ELEMENTS_COUNT>
		void AddBuffer(VertexBuffer& vb, const Layout<ELEMENTS_COUNT>& layout, u32 start = 0, bool per_instance = false)
		{
			Bind();
			vb.Bind();

			u32 offset = 0;

			for (u32 i = start; i < start + ELEMENTS_COUNT; i++)
			{
				AddAttrib(i, layout[i - start], layout.GetStride(), offset, per_instance);
			}

			UnBind();
			vb.UnBind();

			m_buffers.emplace_back(std::move(vb));
		}
	private:
		void AddAttrib(u32 i, ElementType element_type, u32 stride, u32& offset, bool per_instance);
	private:
		u32 m_id;
		std::vector<VertexBuffer> m_buffers;
	};
}