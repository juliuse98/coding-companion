#pragma once
#include "ccobject.h"
#include "glm/fwd.hpp"
#include "index_buffer.h"
#include "sprite.h"
#include "texture_manager.h"
#include "vertex_array.h"
#include "vertex_buffer.h"
#include "vertex_buffer_layout.h"
#include <memory>

namespace cabbage {

    class SpriteRenderer
    {
    public:
        SpriteRenderer();
        ~SpriteRenderer();
        void prepareDraw();
        void draw(coco::CCObject& object); 
    private:
    unsigned int m_indices[6] = {0,2,1,1,2,3};
    glm::vec3 m_vertices[4] = {
        glm::vec3(200 - 100.0f, 200 - 100.0f, 0.0f), // top left
        glm::vec3(200         , 200 - 100.0f, 0.0f), // top right
        glm::vec3(200 - 100.0f, 200         , 0.0f), // bottom left
        glm::vec3(200         , 200         , 0.0f), // bottom right
    };
   std::vector<cabbage::UVCoordinate> test = {cabbage::UVCoordinate(0.0f,1.0f),cabbage::UVCoordinate(1.0f,1.0f),cabbage::UVCoordinate(0.0f,0.0f),cabbage::UVCoordinate(1.0f,0.0f)};

    TextureManager m_textureManager;

    VertexBufferLayout m_vertexBufferLayout;
    VertexBufferLayout m_uvBufferLayout;
    VertexBufferLayout m_textureSlotBufferLayout;

    IndexBuffer m_indexBuffer;
    Shader m_defaultShader;

    VertexBuffer m_vertexBuffer;
    VertexBuffer m_uvBuffer;
    std::unique_ptr<VertexBuffer> m_textureSlotBuffer;
    VertexArray m_vertexArray;
    std::vector<float> m_uvData;
    std::vector<cabbage::UVCoordinate> uvData;
    std::vector<float> m_textureIds = {0.0f,1.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f};
    std::vector<int> m_samplers = {0,1,2,3,4,5,6,7};
    glm::mat4 m_projection;
    };

}
