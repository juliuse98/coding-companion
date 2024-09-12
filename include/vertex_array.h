#pragma once

#include "vertex_buffer.h"

#include <map>

namespace cabbage
{

class VertexBufferLayout;

class VertexArray
{
  private:
  public:
    unsigned int m_RendererId;
    unsigned int m_lastAttribArrayId;
    std::map<const VertexBuffer*, unsigned int>
        m_vertexAttributes; // Map from a VertexBuffer to the first
                            // AttributePointerIndex that is used by its
                            // attributes
    VertexArray();
    ~VertexArray();
    void AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout);
    void UpdateBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout);
    void RemoveBuffer();

    void Bind() const;
    void Unbind() const;
};

} // namespace cabbage
