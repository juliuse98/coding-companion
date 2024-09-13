#include "sprite_renderer.h"

#include <GL/glew.h>
#include <easylogging++.h>
#include <vector>

#include "ccobject.h"
#include "glm/ext/matrix_clip_space.hpp"
#include "render_batch.h"

namespace cabbage
{

SpriteRenderer::SpriteRenderer()
    : m_indexBuffer(m_indices, 6)
    , m_vertexBuffer(m_vertices, sizeof(glm::vec3) * 4)
    , m_defaultShader("resources/shaders/sprite.shader")
    , m_projection(glm::ortho(0.0f, 1920.0f, 1000.0f, 0.0f, -1.0f, 1.0f))
{
    // Prepare Shader
    m_defaultShader.Bind();
    m_defaultShader.SetUniform1iv("ourTexture", 8, m_samplers.data());
    m_defaultShader.SetUniformMat4f("projection", m_projection);
    m_vertexBufferLayout.Push<float>(3);
}
SpriteRenderer::~SpriteRenderer()
{
}
void SpriteRenderer::prepareDraw(coco::CCObject* rootObject)
{
    m_batches.emplace_back(&m_vertexBuffer, &m_vertexBufferLayout);
    m_batches.at(m_batches.size() - 1).addCCObject(rootObject);
}
void SpriteRenderer::draw()
{
    m_batches.at(0).update();
    m_defaultShader.Bind();
    m_indexBuffer.Bind();
    m_batches.at(0).bind();
    glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, m_batches.at(0).objectCount());
}

} // namespace cabbage
