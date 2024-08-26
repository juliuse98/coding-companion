#include "Renderer.h"
#include <iostream>

void GLClearError()
{
    while (glGetError() != GL_NO_ERROR);
};

bool GLLogCall(const char* function, const char* file, int line)
{
    while (GLenum error = glGetError())
    {
        std::cout << "[OPENGL_ERROR]: (" << error << "): " << function << " " << file << ":" << line << std::endl;
        return false;
    }
    return true;
}

namespace timothy 
{
    Renderer::Renderer()
    : m_ClearColor(glm::vec4(200/255.0f, 230/255.0f, 240/255.0f, 1.0f))
    {
        GLCall(glEnable(GL_DEPTH_TEST));
        GLCall(glDisable(GL_CULL_FACE));
    }

    Renderer::~Renderer()
    {
    }

    void Renderer::Draw(const VertexArray& va,const IndexBuffer& ib,const Shader& shader) const
    {
        shader.Bind();
        va.Bind();
        ib.Bind();

        GLCall(glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr));
    }

    void Renderer::DrawInstanced(const VertexArray& va, const IndexBuffer& ib, const Shader& shader, const unsigned int count) const
    {
        shader.Bind();
        va.Bind();
        ib.Bind();
        GLCall(glDrawElementsInstanced(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr, count));
    }

    void Renderer::Clear() const
    {
        GLCall(glClearColor(m_ClearColor.x, m_ClearColor.y, m_ClearColor.z, m_ClearColor.w));
        GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
    }
}