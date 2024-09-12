#pragma once
#include "ccobject.h"
#include "glm/fwd.hpp"
#include "index_buffer.h"
#include "render_batch.h"
#include "sprite.h"
#include "texture_manager.h"
#include "vertex_array.h"
#include "vertex_buffer.h"
#include "vertex_buffer_layout.h"
#include <memory>

namespace cabbage
{

class SpriteRenderer
{
  public:
    SpriteRenderer();
    ~SpriteRenderer();
    void prepareDraw(coco::CCObject* rootObject);
    void draw(coco::CCObject& object);
    void draw();

  private:
    unsigned int m_indices[6] = {0, 2, 1, 1, 2, 3};
    glm::vec3    m_vertices[4] = {
        glm::vec3(0, 0, 0),     // top left
        glm::vec3(100, 0, 0),   // top right
        glm::vec3(0, 100, 0),   // bottom left
        glm::vec3(100, 100, 0), // bottom right
    };

    TextureManager m_textureManager;

    VertexBufferLayout m_vertexBufferLayout;
    VertexBufferLayout m_textureSlotBufferLayout;
    VertexBufferLayout m_transformBufferLayout;

    IndexBuffer m_indexBuffer;
    Shader      m_defaultShader;

    VertexBuffer                       m_vertexBuffer;
    std::unique_ptr<VertexBuffer>      m_textureSlotBuffer;
    std::unique_ptr<VertexBuffer>      m_transformBuffer;
    std::vector<float>                 m_textureIds = {0.0f, 1.0f, 0.0f, 0.0f,
                                                       0.0f, 0.0f, 0.0f, 0.0f};
    std::vector<int>                   m_samplers = {0, 1, 2, 3, 4, 5, 6, 7};
    glm::mat4                          m_projection;
    std::vector<glm::mat4>             m_transformData;

    std::vector<RenderBatch> m_batches;
};

} // namespace cabbage
