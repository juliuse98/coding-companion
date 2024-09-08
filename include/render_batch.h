#pragma once
#include <memory>
#include <unordered_set>
#include <vector>

#include "ccobject.h"
#include "sprite.h"
#include "texture.h"
#include "vertex_array.h"
#include "vertex_buffer.h"
#include "vertex_buffer_layout.h"
#include <easylogging++.h>

namespace cabbage
{

class RenderBatch
{
  public:
    RenderBatch(VertexBuffer& vertexBuffer,
                VertexBufferLayout& vertexBufferLayout)
    {
        m_vertexArray.AddBuffer(vertexBuffer, vertexBufferLayout);
        glVertexAttribDivisor(0, 0);
        // Prepare buffer layouts
        m_uvBufferLayout.Push<float>(2);
        m_textureSlotBufferLayout.Push<float>(1);
        m_transformBufferLayout.Push<float>(4);
        m_transformBufferLayout.Push<float>(4);
        m_transformBufferLayout.Push<float>(4);
        m_transformBufferLayout.Push<float>(4);
    };

  private:
    std::vector<coco::CCObject*> m_objects;
    std::unordered_map<cabbage::Texture*, int> m_textureSlotMap;

    VertexArray m_vertexArray;

    std::unique_ptr<VertexBuffer> m_uvBuffer;
    std::unique_ptr<VertexBuffer> m_textureSlotBuffer;
    std::unique_ptr<VertexBuffer> m_transformBuffer;

    VertexBufferLayout m_uvBufferLayout;
    VertexBufferLayout m_textureSlotBufferLayout;
    VertexBufferLayout m_transformBufferLayout;

    std::vector<float> m_uvData;
    std::vector<float> m_textureSlotData;
    std::vector<glm::mat4> m_transformData;

  public:
    void addCCObject(coco::CCObject* object) { m_objects.push_back(object); };
    void addCCObject(std::vector<coco::CCObject*> objects)
    {
        for (auto it = objects.begin(); it != objects.end(); it++)
        {
            m_objects.push_back((*it));
        }
    };

    void prepare()
    {
        m_uvData.reserve(m_objects.size() * 8);
        m_textureSlotData.reserve(m_objects.size());
        m_transformData.reserve(m_objects.size());

        int index = 0;
        int textureIndex = 0;
        for (auto object = m_objects.begin(); object != m_objects.end();
             object++)
        {

            std::array<float, 8> uvD =
                (*object)->GetSprite()->GetUVRect().toUVArray();
            m_uvData.insert(m_uvData.end(), uvD.begin(), uvD.end());
            if (m_textureSlotMap.find((*object)->GetTexture()) ==
                m_textureSlotMap.end())
            {
                m_textureSlotMap.insert(
                    {(*object)->GetTexture(), textureIndex});
                m_textureSlotData.push_back(textureIndex);
                textureIndex++;
            }
            else
            {
                m_textureSlotData.push_back(
                    m_textureSlotMap.find((*object)->GetTexture())->second);
            }
            m_transformData.push_back(
                (*object)->GetTransform().getModelMatrix());
            index++;
        }

        m_uvBuffer = std::make_unique<VertexBuffer>(
            m_uvData.data(), sizeof(float) * m_uvData.size());
        m_textureSlotBuffer = std::make_unique<VertexBuffer>(
            m_textureSlotData.data(), sizeof(float) * m_textureSlotData.size());
        m_transformBuffer = std::make_unique<VertexBuffer>(
            m_transformData.data(), sizeof(glm::mat4) * m_transformData.size());

        m_vertexArray.AddBuffer(*m_uvBuffer, m_uvBufferLayout);
        glVertexAttribDivisor(1, 0);

        m_vertexArray.AddBuffer(*m_textureSlotBuffer,
                                m_textureSlotBufferLayout);
        glVertexAttribDivisor(2, 1);

        m_vertexArray.AddBuffer(*m_transformBuffer, m_transformBufferLayout);
        glVertexAttribDivisor(3, 1);
        glVertexAttribDivisor(4, 1);
        glVertexAttribDivisor(5, 1);
        glVertexAttribDivisor(6, 1);
        LOG(INFO) << "Finish Prepare Batch";
    }
    void bind()
    {
        m_vertexArray.Bind();
        for (auto texture = m_textureSlotMap.begin();
             texture != m_textureSlotMap.end(); texture++)
        {
            texture->first->bind(texture->second);
        }
    };

    // updateData
    // public bind
};

} // namespace cabbage
