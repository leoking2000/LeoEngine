#include <glad/glad.h>
#include "BufferObjects.h"


namespace LEO
{
	VertexBuffer::VertexBuffer(const void* data, u32 size)
	{
		glGenBuffers(1, &m_id);
		glBindBuffer(GL_ARRAY_BUFFER, m_id);
		glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	VertexBuffer::VertexBuffer(VertexBuffer&& other)
		:
		m_id(other.m_id)
	{
		other.m_id = 0;
	}

	VertexBuffer& VertexBuffer::operator=(VertexBuffer&& other)
	{
		glDeleteBuffers(1, &m_id);

		m_id = other.m_id;
		other.m_id = 0;
		return *this;
	}

	VertexBuffer::~VertexBuffer()
	{
		glDeleteBuffers(1, &m_id);
	}

	void VertexBuffer::Bind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_id);
	}

	void VertexBuffer::UnBind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	//----------------------------------------------------------

	IndexBuffer::IndexBuffer(const u32* data, u32 count)
		:
		m_count(count)
	{
		glGenBuffers(1, &m_id);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(u32), (const void*)data, GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	IndexBuffer::IndexBuffer(IndexBuffer&& other)
		:
		m_count(other.m_count),
		m_id(other.m_id)
	{
		other.m_id = 0;
	}

	IndexBuffer& IndexBuffer::operator=(IndexBuffer&& other)
	{
		glDeleteBuffers(1, &m_id);

		m_id = other.m_id;
		m_count = other.m_count;

		other.m_id = 0;

		return *this;
	}

	IndexBuffer::~IndexBuffer()
	{
		glDeleteBuffers(1, &m_id);
	}

	void IndexBuffer::Bind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id);
	}

	void IndexBuffer::UnBind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	//----------------------------------------------------------

	VertexArray::VertexArray()
	{
		glGenVertexArrays(1, &m_id);
	}

	VertexArray::VertexArray(VertexArray&& other)
		:
		m_id(other.m_id)
	{
		other.m_id = 0;
		for (i32 i = 0; i < MAX_VERTEXBUFFERS; i++)
		{
			m_buffers[i] = std::move(other.m_buffers[i]);
		}	
	}

	VertexArray& VertexArray::operator=(VertexArray&& other)
	{
		glDeleteVertexArrays(1, &m_id);

		m_id = other.m_id;
		other.m_id = 0;

		for (i32 i = 0; i < MAX_VERTEXBUFFERS; i++)
		{
			m_buffers[i] = std::move(other.m_buffers[i]);
		}

		return *this;
	}

	VertexArray::~VertexArray()
	{
		glDeleteVertexArrays(1, &m_id);
	}

	void VertexArray::Bind() const
	{
		glBindVertexArray(m_id);
	}

	void VertexArray::UnBind() const
	{
		glBindVertexArray(0);
	}

	void VertexArray::AddAttrib(u32 i, ElementType element_type, u32 stride, u32& offset, bool per_instance)
	{
		auto normalize = element_type % 2 == 0 ? GL_FALSE : GL_TRUE;
		auto type = GL_FLOAT;
		u32 size = sizeof(float);
		u32 count = 1;

		switch (element_type)
		{
		case FLOAT1:
		case FLOAT1_N:
			type = GL_FLOAT;
			size = sizeof(float);
			count = 1;
			break;
		case FLOAT2:
		case FLOAT2_N:
			type = GL_FLOAT;
			size = sizeof(float);
			count = 2;
			break;
		case FLOAT3:
		case FLOAT3_N:
			type = GL_FLOAT;
			size = sizeof(float);
			count = 3;
			break;
		case FLOAT4:
		case FLOAT4_N:
			type = GL_FLOAT;
			size = sizeof(float);
			count = 4;
			break;

		case UCHAR3:
		case UCHAR3_N:
			type = GL_UNSIGNED_BYTE;
			size = 1;
			count = 3;
			break;
		case UCHAR4:
		case UCHAR4_N:
			type = GL_UNSIGNED_BYTE;
			size = 1;
			count = 4;
			break;

		case MAT4:
			type = GL_FLOAT;
			size = sizeof(float);
			count = 4;
			normalize = GL_FALSE;
			break;
		default:
			LEOASSERT(false, "VertexArray::AddBuffer error unknowed ElementType!!!")LEOWATCH(element_type);
			break;
		}

		if (element_type == MAT4)
		{
			glEnableVertexAttribArray(i + 0);
			glVertexAttribPointer(i + 0, count, type, normalize, stride, (const void*)((u64)offset + 0));

			glEnableVertexAttribArray(i + 1);
			glVertexAttribPointer(i + 1, count, type, normalize, stride, (const void*)((u64)offset + 1 * sizeof(glm::vec4)));

			glEnableVertexAttribArray(i + 2);
			glVertexAttribPointer(i + 2, count, type, normalize, stride, (const void*)((u64)offset + 2 * sizeof(glm::vec4)));

			glEnableVertexAttribArray(i + 3);
			glVertexAttribPointer(i + 3, count, type, normalize, stride, (const void*)((u64)offset + 3 * sizeof(glm::vec4)));

			glVertexAttribDivisor(i + 0, 1);
			glVertexAttribDivisor(i + 1, 1);
			glVertexAttribDivisor(i + 2, 1);
			glVertexAttribDivisor(i + 3, 1);

			offset += count * size;
			return;
		}

		glEnableVertexAttribArray(i);
		glVertexAttribPointer(i, count, type, normalize, stride, (const void*)((u64)offset));
		glVertexAttribDivisor(i, per_instance ? 1 : 0);

		offset += count * size;
	}

}
