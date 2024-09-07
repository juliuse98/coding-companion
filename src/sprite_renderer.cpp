#include "sprite_renderer.h"

#include <GL/glew.h>
#include <memory>
#include <vector>
#include <easylogging++.h>

#include "ccobject.h"
#include "glm/ext/matrix_clip_space.hpp"
#include "sprite.h"

namespace cabbage {

    SpriteRenderer::SpriteRenderer()
    : m_indexBuffer(m_indices, 6)
    , m_vertexBuffer(m_vertices, sizeof(glm::vec3) * 4)
    , m_uvBuffer(test.data(), sizeof(cabbage::UVCoordinate) * 4)
    , m_defaultShader("resources/shaders/sprite.shader")
    , m_projection(glm::ortho(0.0f,1920.0f,1000.0f,0.0f, -1.0f, 1.0f))
    {
        //Prepare Shader
        m_defaultShader.Bind();  
        m_defaultShader.SetUniform1iv("ourTexture", 8, m_samplers.data());
        m_defaultShader.SetUniformMat4f("projection", m_projection);
        
        m_textureSlotBuffer = std::make_unique<VertexBuffer>(m_textureIds.data(), sizeof(float) * 8);
        m_transformData.push_back(glm::mat4(1.0f));
        m_transformBuffer = std::make_unique<VertexBuffer>(m_transformData.data(), sizeof(glm::mat4) * 1);
        
        m_transformBufferLayout.Push<float>(4);
        m_transformBufferLayout.Push<float>(4);
        m_transformBufferLayout.Push<float>(4);
        m_transformBufferLayout.Push<float>(4);
        m_textureSlotBufferLayout.Push<float>(1);
        m_vertexBufferLayout.Push<float>(3);
        m_uvBufferLayout.Push<float>(2);
        m_vertexArray.AddBuffer(m_vertexBuffer, m_vertexBufferLayout);
        glVertexAttribDivisor(0,0);
        m_vertexArray.AddBuffer(m_uvBuffer, m_uvBufferLayout);
        glVertexAttribDivisor(1,0);
        m_vertexArray.AddBuffer(*m_textureSlotBuffer, m_textureSlotBufferLayout);
        glVertexAttribDivisor(2,1);
        m_vertexArray.AddBuffer(*m_transformBuffer, m_transformBufferLayout);
        glVertexAttribDivisor(3,1);
        glVertexAttribDivisor(4,1);
        glVertexAttribDivisor(5,1);
        glVertexAttribDivisor(6,1);
    }
    SpriteRenderer::~SpriteRenderer(){}
    void SpriteRenderer::prepareDraw()
    {
        
    }
    void SpriteRenderer::draw(coco::CCObject& object)
    {
        m_defaultShader.Bind();
        std::vector<coco::CCObject*> children = object.GetChildren();
        
        std::vector<coco::CCObject*>::iterator it;
        
        m_textureManager.AddTexture(object.GetSprite()->GetTexutre());
        m_textureIds[0] = m_textureManager.bindTexture(object.GetSprite()->GetTexutre());
        m_transformData.clear();
        m_transformData.push_back(object.GetTransform().getModelMatrix());
        m_uvData.clear();
        std::array<float, 8> uvD = object.GetSprite()->GetUVRect().toUVArray();
        m_uvData.insert(m_uvData.end(),uvD.begin(),uvD.end());
        for(it = children.begin(); it != children.end(); it++)
        {
            std::array<float, 8> uvD = (*it)->GetSprite()->GetUVRect().toUVArray();
            m_uvData.insert(m_uvData.end(),uvD.begin(),uvD.end());
            m_textureManager.AddTexture((*it)->GetSprite()->GetTexutre());
            m_textureIds[1] = m_textureManager.bindTexture((*it)->GetSprite()->GetTexutre());
            m_transformData.push_back((*it)->GetTransform().getModelMatrix());
        } 
        m_transformBuffer->Bind();
        m_transformBuffer->Resize(sizeof(glm::mat4) * m_transformData.size(), m_transformData.data());
        m_vertexArray.UpdateBuffer(*m_transformBuffer, m_transformBufferLayout);    
        m_uvBuffer.Bind();
        m_uvBuffer.Resize(sizeof(cabbage::UVCoordinate) * m_uvData.size(),m_uvData.data());    
        m_textureSlotBuffer->Bind();
        m_textureSlotBuffer->SetData(0, sizeof(int) * 8, m_textureIds.data());
        m_vertexArray.UpdateBuffer(m_uvBuffer, m_uvBufferLayout);
        m_indexBuffer.Bind();
        m_vertexArray.Bind();
        m_uvBuffer.Bind();
        m_vertexBuffer.Bind();
        m_textureSlotBuffer->Bind();
        glDrawElementsInstanced(GL_TRIANGLES,6, GL_UNSIGNED_INT, 0,2);
    }

}
