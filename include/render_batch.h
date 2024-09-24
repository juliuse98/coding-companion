#pragma once
#include <cassert>
#include <memory>
#include <vector>

#include "glm/fwd.hpp"

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
    RenderBatch(VertexBuffer* vertexBuffer, VertexBufferLayout* vertexBufferLayout) : m_vertexArray()
    {
        m_vertexArray.AddBuffer(*vertexBuffer, *vertexBufferLayout);
        //  Prepare buffer layouts
        m_uvBufferLayout.Push<float>(2);
        m_textureSlotBufferLayout.Push<float>(1, 1);
        m_transformBufferLayout.Push<float>(4, 1);
        m_transformBufferLayout.Push<float>(4, 1);
        m_transformBufferLayout.Push<float>(4, 1);
        m_transformBufferLayout.Push<float>(4, 1);
    };

  private:
    coco::CCObject* m_rootObject;

    std::unordered_map<cabbage::Texture*, int>     m_textureSlotMap;
    std::unordered_map<coco::CCObject*, glm::mat4> m_objectToWorldTransform;

    VertexArray m_vertexArray;

    std::unique_ptr<VertexBuffer> m_transformBuffer;
    std::unique_ptr<VertexBuffer> m_uvBuffer;
    std::unique_ptr<VertexBuffer> m_textureSlotBuffer;

    VertexBufferLayout m_transformBufferLayout;
    VertexBufferLayout m_uvBufferLayout;
    VertexBufferLayout m_textureSlotBufferLayout;

    std::vector<glm::mat4> m_transformData;
    std::vector<float>     m_uvData;
    std::vector<float>     m_textureSlotData;

  public:
    void addCCObject(coco::CCObject* object)
    {
        m_rootObject = object;
    };

    void update()
    {
        m_transformData.clear();
        m_uvData.clear();
        m_textureSlotData.clear();

        std::vector<coco::CCObject*> allObjects;
        allObjects.push_back(m_rootObject);

        coco::CCObject* currentObject = nullptr;

        int index = 0;
        while (allObjects.size() > index)
        {
            assert(
                allObjects.size() < 32
            ); // This is the maximum number of objects that can be in a tree of object for now
            currentObject = allObjects[index];
            index++;

            //~~<>~~<>~~~<>~~~<>~~~~<+>~~<[ Transform ]>~~<+>~~~~<>~~~<>~~~<>~~<>~~//
            glm::mat4 localTransform = currentObject->GetTransform().getModelMatrix();

            glm::mat4 parentWorldTransform = glm::mat4(1.0f);

            coco::CCObject* parent = currentObject->GetParent();
            if (parent)
            {
                parentWorldTransform = m_objectToWorldTransform.find(parent)->second;
            }
            glm::mat4 worldTransform = parentWorldTransform * localTransform;
            m_objectToWorldTransform.insert_or_assign(currentObject, worldTransform);

            if (currentObject->GetSprite())
            {

                m_transformData.push_back(worldTransform);

                //~~<>~~<>~~~<>~~~<>~~~~<+>~~<[ UV ]>~~<+>~~~~<>~~~<>~~~<>~~<>~~//
                std::array<float, 8> uvD = currentObject->GetSprite()->GetUVRect().toUVArray();
                m_uvData.insert(m_uvData.end(), uvD.begin(), uvD.end());

                //~~<>~~<>~~~<>~~~<>~~~~<+>~~<[ textureSlot ]>~~<+>~~~~<>~~~<>~~~<>~~<>~~//
                int  textureSlotIndex = 0;
                auto it = m_textureSlotMap.find(currentObject->GetSprite()->GetTexutre());
                if (it == m_textureSlotMap.end())
                {
                    textureSlotIndex = m_textureSlotMap.size();
                    m_textureSlotMap.insert({currentObject->GetSprite()->GetTexutre(), textureSlotIndex});
                }
                else
                {
                    textureSlotIndex = it->second;
                }
                m_textureSlotData.push_back(textureSlotIndex);
            }
            //~~<>~~<>~~~<>~~~<>~~~~<+>~~<[ Children ]>~~<+>~~~~<>~~~<>~~~<>~~<>~~//
            std::vector<coco::CCObject*>& children = currentObject->GetChildren();
            allObjects.insert(allObjects.end(), children.begin(), children.end());
        }

        //~~<>~~<>~~~<>~~~<>~~~~<+>~~<[ Set Buffers ]>~~<+>~~~~<>~~~<>~~~<>~~<>~~//
        if (m_uvBuffer)
        {
            m_uvBuffer->Bind();
            m_uvBuffer->SetData(0, sizeof(float) * m_uvData.size(), m_uvData.data());

            m_textureSlotBuffer->Bind();
            m_textureSlotBuffer->SetData(0, sizeof(float) * m_textureSlotData.size(), m_textureSlotData.data());

            m_transformBuffer->Bind();
            m_transformBuffer->SetData(0, sizeof(glm::mat4) * m_transformData.size(), m_transformData.data());
            return;
        }

        m_uvBuffer = std::make_unique<VertexBuffer>(m_uvData.data(), sizeof(float) * m_uvData.size());
        m_textureSlotBuffer =
            std::make_unique<VertexBuffer>(m_textureSlotData.data(), sizeof(float) * m_textureSlotData.size());
        m_transformBuffer =
            std::make_unique<VertexBuffer>(m_transformData.data(), sizeof(glm::mat4) * m_transformData.size());

        m_vertexArray.AddBuffer(*m_uvBuffer, m_uvBufferLayout);

        m_vertexArray.AddBuffer(*m_textureSlotBuffer, m_textureSlotBufferLayout);

        m_vertexArray.AddBuffer(*m_transformBuffer, m_transformBufferLayout);
    }
    void bind()
    {
        m_vertexArray.Bind();
        for (auto texture = m_textureSlotMap.begin(); texture != m_textureSlotMap.end(); texture++)
        {
            texture->first->bind(texture->second);
        }
    };
    int objectCount()
    {
        // TODO: implement this correctly
        return 2;
    }
};

} // namespace cabbage
