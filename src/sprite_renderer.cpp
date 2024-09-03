#include "sprite_renderer.h"

#include <GL/glew.h>
#include <memory>
#include <vector>
#include <iostream>

#include "ccobject.h"
#include "sprite.h"

namespace cabbage {

    SpriteRenderer::SpriteRenderer()
    : m_indexBuffer(m_indices, 6)
    , m_vertexBuffer(m_vertices, sizeof(glm::vec3) * 4)
    , m_uvBuffer(test.data(), sizeof(cabbage::UVCoordinate) * 4)
    , m_textureManager()
    {
        m_textureSlotBuffer = std::make_unique<VertexBuffer>(m_textureIds.data(), sizeof(float) * 8);
        m_textureSlotBufferLayout.Push<float>(1);
        m_vertexBufferLayout.Push<float>(3);
        m_uvBufferLayout.Push<float>(2);
        m_vertexArray.AddBuffer(m_vertexBuffer, m_vertexBufferLayout);
        glVertexAttribDivisor(0,0);
        m_vertexArray.AddBuffer(m_uvBuffer, m_uvBufferLayout);
        glVertexAttribDivisor(1,0);
        m_vertexArray.AddBuffer(*m_textureSlotBuffer, m_textureSlotBufferLayout);
        glVertexAttribDivisor(2,1);
    }
    SpriteRenderer::~SpriteRenderer(){}
    void SpriteRenderer::prepareDraw()
    {
        
    }
    void SpriteRenderer::draw(coco::CCObject& object)
    {
        std::vector<coco::CCObject*> children = object.GetChildren();
        
        std::vector<coco::CCObject*>::iterator it;
        
        
        //std::cout << "hello " << m_textureManager.AddTexture(object.GetSprite()->GetTexutre()) << std::endl;
        //m_textureIds[0] = m_textureManager.bindTexture(object.GetSprite()->GetTexutre());
        m_uvData.clear();
        std::array<float, 8> uvD = object.GetSprite()->GetUVRect().toUVArray();
        m_uvData.insert(m_uvData.end(),uvD.begin(),uvD.end());
        for(it = children.begin(); it != children.end(); it++)
        {
            std::array<float, 8> uvD = (*it)->GetSprite()->GetUVRect().toUVArray();
            m_uvData.insert(m_uvData.end(),uvD.begin(),uvD.end());
            //m_textureManager.AddTexture((*it)->GetSprite()->GetTexutre());
            //std::cout << "hello " << m_textureManager.AddTexture((*it)->GetSprite()->GetTexutre()) << std::endl;
            //m_textureIds[1] = m_textureManager.bindTexture((*it)->GetSprite()->GetTexutre());
        } 
        m_uvBuffer.Bind();
        m_uvBuffer.Resize(sizeof(cabbage::UVCoordinate) * m_uvData.size(),m_uvData.data());    
        m_textureSlotBuffer->Bind();
 //       m_textureSlotBuffer->SetData(0, sizeof(int) * 8, m_textureIds.data());
        m_vertexArray.UpdateBuffer(m_uvBuffer, m_uvBufferLayout);
        m_indexBuffer.Bind();
        m_vertexArray.Bind();
        m_uvBuffer.Bind();
        m_vertexBuffer.Bind();
        m_textureSlotBuffer->Bind();
        glDrawElementsInstanced(GL_TRIANGLES,6, GL_UNSIGNED_INT, 0,2);
    }

}
