#include "sprite_renderer.h"

#include <GL/glew.h>

#include "ccobject.h"

namespace cabbage {

    SpriteRenderer::SpriteRenderer()
    : m_indexBuffer(m_indices, 6)
    , m_vertexBuffer(m_vertices, sizeof(glm::vec3) * 4)
    {
        m_vertexBufferLayout.Push<float>(3);
        m_vertexArray.AddBuffer(m_vertexBuffer, m_vertexBufferLayout);
        
    }
    SpriteRenderer::~SpriteRenderer(){}
    void SpriteRenderer::prepareDraw()
    {
        
    }
    void SpriteRenderer::draw(coco::CCObject& object)
    {
        m_vertexArray.Bind();
        m_indexBuffer.Bind();
        glDrawElementsInstanced(GL_TRIANGLES,6, GL_UNSIGNED_INT, 0,1);
    }

}
