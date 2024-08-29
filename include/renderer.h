#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "vertex_array.h"
#include "index_buffer.h"
#include "shader.h"

#define ASSERT(x) assert(x);


#define GLCall(x) GLClearError();\
x;\
ASSERT(GLLogCall(#x,__FILE__,__LINE__))

void GLClearError();

bool GLLogCall(const char* function, const char* file, int line);

namespace cabbage 
{
	class Renderer
	{
	public:
		Renderer();
		~Renderer();
		void Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const;
		void DrawInstanced(const VertexArray& va, const IndexBuffer& ib, const Shader& shader, const unsigned int count) const;
		void Clear() const;
		glm::vec4 m_ClearColor;
	};
}
