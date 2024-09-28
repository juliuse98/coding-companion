#pragma once
#include "ccobject.h"
#include "glm/fwd.hpp"
#include "index_buffer.h"
#include "render_batch.h"
#include "texture_manager.h"
#include "vertex_array.h"
#include "vertex_buffer.h"
#include "vertex_buffer_layout.h"
#include "window.h"
#include <memory>

#define COMPANION_DEFAULT_SIZE 1.0f

namespace cabbage
{

class SpriteRenderer
{
  public:
    SpriteRenderer(cabbage::Window* window);
    ~SpriteRenderer();
    void prepareDraw(coco::CCObject* rootObject);
    void draw(coco::CCObject& object);
    void draw();

  private:
    unsigned int m_indices[6] = {0, 2, 1, 1, 2, 3};
    glm::vec3    m_vertices[4] = {
        glm::vec3(0.0f, 0.0f, 0.0f),                                     // top left
        glm::vec3(COMPANION_DEFAULT_SIZE, 0.0f, 0.0f),                   // top right
        glm::vec3(0.0f, COMPANION_DEFAULT_SIZE, 0.0f),                   // bottom left
        glm::vec3(COMPANION_DEFAULT_SIZE, COMPANION_DEFAULT_SIZE, 0.0f), // bottom right
    };

    TextureManager m_textureManager;

    VertexBufferLayout m_vertexBufferLayout;
    VertexBufferLayout m_textureSlotBufferLayout;
    VertexBufferLayout m_transformBufferLayout;

    IndexBuffer m_indexBuffer;
    Shader      m_defaultShader;

    VertexBuffer                  m_vertexBuffer;
    std::unique_ptr<VertexBuffer> m_textureSlotBuffer;
    std::unique_ptr<VertexBuffer> m_transformBuffer;
    std::vector<float>            m_textureIds = {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f};
    std::vector<int>              m_samplers = {0, 1, 2, 3, 4, 5, 6, 7};
    glm::mat4                     m_projection;
    std::vector<glm::mat4>        m_transformData;

    std::vector<RenderBatch> m_batches;

    cabbage::Window* m_window;
};

} // namespace cabbage
