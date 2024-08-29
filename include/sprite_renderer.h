#pragma once
#include "ccobject.h"
#include "index_buffer.h"
#include "sprite.h"
#include "vertex_array.h"
#include "vertex_buffer.h"
#include "vertex_buffer_layout.h"

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
        glm::vec3(1000 - 100.0f, 1000 - 100.0f, 0.0f), // top left
        glm::vec3(1000         , 1000 - 100.0f, 0.0f), // top right
        glm::vec3(1000 - 100.0f, 1000         , 0.0f), // bottom left
        glm::vec3(1000         , 1000         , 0.0f), // bottom right
    };
    VertexBufferLayout m_vertexBufferLayout;
    VertexBuffer m_vertexBuffer;
    VertexArray m_vertexArray;
    IndexBuffer m_indexBuffer;
    std::vector<cabbage::UVCoordinate> uvData;
    };

}
