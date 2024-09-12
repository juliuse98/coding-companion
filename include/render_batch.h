#pragma once
#include <memory>
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
    RenderBatch(
        VertexBuffer* vertexBuffer, VertexBufferLayout* vertexBufferLayout
    )
        : m_vertexArray()
    {
        m_vertexArray.AddBuffer(*vertexBuffer, *vertexBufferLayout);
        //  Prepare buffer layouts
        m_uvBufferLayout.Push<float>(2);
        m_textureSlotBufferLayout.Push<float>(1,1);
        m_transformBufferLayout.Push<float>(4,1);
        m_transformBufferLayout.Push<float>(4,1);
        m_transformBufferLayout.Push<float>(4,1);
        m_transformBufferLayout.Push<float>(4,1);
    };

  private:
    std::vector<coco::CCObject*>                   m_objects;
    std::unordered_map<cabbage::Texture*, int>     m_textureSlotMap;
    std::unordered_map<coco::CCObject*, glm::mat4> m_objectToWorldTransform;

    VertexArray m_vertexArray;

    std::unique_ptr<VertexBuffer> m_uvBuffer;
    std::unique_ptr<VertexBuffer> m_textureSlotBuffer;
    std::unique_ptr<VertexBuffer> m_transformBuffer;

    VertexBufferLayout m_uvBufferLayout;
    VertexBufferLayout m_textureSlotBufferLayout;
    VertexBufferLayout m_transformBufferLayout;

    std::vector<float>     m_uvData;
    std::vector<float>     m_textureSlotData;
    std::vector<glm::mat4> m_transformData;

  public:
    void addCCObject(coco::CCObject* object)
    {
        m_objects.push_back(object);
    };
    void addCCObject(
        std::vector<coco::CCObject*>&                   objects,
        std::unordered_map<coco::CCObject*, glm::mat4>& objectToWorldTransform
    )
    {
        for (auto it = objects.begin(); it != objects.end(); it++)
        {
            m_objects.push_back((*it));
        }
        m_objectToWorldTransform = objectToWorldTransform;
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
            if (m_textureSlotMap.find((*object)->GetTexture())
                == m_textureSlotMap.end())
            {
		// TODO: Add updating of texture slots and updating of transforms
                m_textureSlotMap.insert({(*object)->GetTexture(), textureIndex}
                );
                m_textureSlotData.push_back(textureIndex);
                textureIndex++;
            }
            else
            {
                m_textureSlotData.push_back(
                    m_textureSlotMap.find((*object)->GetTexture())->second
                );
            }
            m_transformData.push_back(
                m_objectToWorldTransform.find((*object))->second
            );
            index++;
        }

        m_uvBuffer = std::make_unique<VertexBuffer>(
            m_uvData.data(), sizeof(float) * m_uvData.size()
        );
        m_textureSlotBuffer = std::make_unique<VertexBuffer>(
            m_textureSlotData.data(), sizeof(float) * m_textureSlotData.size()
        );
        m_transformBuffer = std::make_unique<VertexBuffer>(
            m_transformData.data(), sizeof(glm::mat4) * m_transformData.size()
        );

        m_vertexArray.AddBuffer(*m_uvBuffer, m_uvBufferLayout);

        m_vertexArray.AddBuffer(
            *m_textureSlotBuffer, m_textureSlotBufferLayout
        );

        m_vertexArray.AddBuffer(*m_transformBuffer, m_transformBufferLayout);
    }
    void update()
    {
	m_uvData.clear();
	m_transformData.clear();
	
        int index = 0;
        int textureIndex = 0;
        for (auto object = m_objects.begin(); object != m_objects.end();
             object++)
        {
            std::array<float, 8> uvD =
                (*object)->GetSprite()->GetUVRect().toUVArray();
            m_uvData.insert(m_uvData.end(), uvD.begin(), uvD.end());
	    // TODO: Add updating of texture slots and updating of transforms
            m_transformData.push_back(
                m_objectToWorldTransform.find((*object))->second
            );
            index++;
        }

	m_uvBuffer->Bind();
	m_uvBuffer->SetData(0, sizeof(float) * m_uvData.size(), m_uvData.data());
	m_textureSlotBuffer->Bind();
	m_textureSlotBuffer->SetData(0, sizeof(float) * m_textureSlotData.size(), m_textureSlotData.data());
	m_transformBuffer->Bind();
	m_transformBuffer->SetData(0, sizeof(glm::mat4) * m_transformData.size(), m_transformData.data());
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
    int objectCount()
    {
        return m_objects.size();
    }

    // updateData
    // public bind
};

} // namespace cabbage
