#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"

#define ASSERT(x) if(!(x)) __debugbreak();


#define GLCall(x) GLClearError();\
x;\
ASSERT(GLLogCall(#x,__FILE__,__LINE__))

//#define GLCall(x) x

void GLClearError();

bool GLLogCall(const char* function, const char* file, int line);

namespace timothy 
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