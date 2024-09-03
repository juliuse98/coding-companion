#pragma once
#include <GL/glew.h>
namespace cabbage {

	class VertexBuffer {
	private:
		unsigned int m_RendererId;
		unsigned int m_Size;
		int m_GlDrawType;
	public:
		VertexBuffer(const void* data, unsigned int size, int glDrawType = GL_STATIC_DRAW);
		~VertexBuffer();

		void SetData(const unsigned int offset,const unsigned int size, const void* data);
		void Resize(unsigned int newSize, const void* newData);
		void Bind() const;
		void Unbind() const;
	};

}
