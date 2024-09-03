#include "vertex_buffer.h"

#include "renderer.h"

namespace cabbage {

		VertexBuffer::VertexBuffer(const void* data, unsigned int size, int GlDrawType)
		: m_Size(size)
		, m_GlDrawType(GlDrawType)
		{
				GLCall(glGenBuffers(1, &m_RendererId));
				GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererId));
				GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, GlDrawType));
		}

		VertexBuffer::~VertexBuffer()
		{
				GLCall(glDeleteBuffers(1, &m_RendererId));
		}

		void VertexBuffer::Bind() const
		{
				GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererId));
		}

		void VertexBuffer::Unbind() const
		{
				GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
		}

		void VertexBuffer::SetData(const unsigned int offset, const unsigned int size, const void* data)
		{
				GLCall(glBufferSubData(GL_ARRAY_BUFFER,offset,size,data));
		}

		void VertexBuffer::Resize(unsigned int newSize, const void* newData)
		{
			// Check if the new size is different from the current size
			if (newSize != m_Size)
			{
				// Allocate a new buffer of the new size
				GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererId));
				GLCall(glBufferData(GL_ARRAY_BUFFER, newSize, nullptr, m_GlDrawType)); // Create new buffer storage

				// Copy old data if it exists
				if (newData)
				{
					// Copy new data to the buffer
					GLCall(glBufferSubData(GL_ARRAY_BUFFER, 0, newSize, newData));
				}

				// Update the size of the buffer
				m_Size = newSize;
			}
		}

}
