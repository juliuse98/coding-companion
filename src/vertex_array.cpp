#include "vertex_array.h"

#include "easylogging++.h"
#include "renderer.h"
#include "vertex_buffer_layout.h"
#include <cstdint>
#include <iostream>

namespace cabbage
{

VertexArray::VertexArray() : m_lastAttribArrayId(0)
{
    GLCall(glGenVertexArrays(1, &m_RendererId));
}

VertexArray::~VertexArray() { GLCall(glDeleteVertexArrays(1, &m_RendererId)); }

void VertexArray::AddBuffer(const VertexBuffer& vb,
                            const VertexBufferLayout& layout)
{
    Bind();
    vb.Bind();
    const auto& elements = layout.GetElements();

    m_vertexAttributes[&vb] = m_lastAttribArrayId;

    unsigned int offset = 0;
    for (unsigned int i = 0; i < elements.size(); ++i)
    {
        const auto& element = elements[i];
        GLCall(glEnableVertexAttribArray(m_lastAttribArrayId + i));
        if (element.type == GL_INT)
        {

            GLCall(glVertexAttribIPointer(
                m_lastAttribArrayId + i, element.count, element.type,
                layout.GetStride(),
                reinterpret_cast<const void*>(static_cast<intptr_t>(offset))));
        }
        else
        {
            GLCall(glVertexAttribPointer(
                m_lastAttribArrayId + i, element.count, element.type,
                element.normalized, layout.GetStride(),
                reinterpret_cast<const void*>(static_cast<intptr_t>(offset))));
        }
        offset +=
            element.count * VertexBufferElement::GetSizeOfType(element.type);
    }
    m_lastAttribArrayId = m_lastAttribArrayId + elements.size();
}
void VertexArray::UpdateBuffer(const VertexBuffer& vb,
                               const VertexBufferLayout& layout)
{
    Bind();
    vb.Bind();
    const auto& elements = layout.GetElements();

    std::cout << m_vertexAttributes[&vb] << std::endl;
    unsigned int offset = 0;
    for (unsigned int i = 0; i < elements.size(); ++i)
    {
        const auto& element = elements[i];
        GLCall(glVertexAttribPointer(
            m_vertexAttributes[&vb] + i, element.count, element.type,
            element.normalized, layout.GetStride(),
            reinterpret_cast<const void*>(static_cast<intptr_t>(offset))));
        offset +=
            element.count * VertexBufferElement::GetSizeOfType(element.type);
    }
}

void VertexArray::Bind() const { GLCall(glBindVertexArray(m_RendererId)); }

void VertexArray::Unbind() const { GLCall(glBindVertexArray(0)); }

} // namespace cabbage
