#include "VertexBuffer.h"

#include "Renderer.h"

namespace cabbage {
		VertexBuffer::VertexBuffer(const void* data, unsigned int size, int GlDrawType)
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
}
