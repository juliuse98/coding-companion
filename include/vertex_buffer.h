#pragma once
#include <GL/glew.h>
class VertexBuffer {
private:
	unsigned int m_RendererId;
public:
	VertexBuffer(const void* data, unsigned int size, int glDrawType = GL_STATIC_DRAW);
	~VertexBuffer();

	void SetData(const unsigned int offset,const unsigned int size, const void* data);
	void Bind() const;
	void Unbind() const;
};