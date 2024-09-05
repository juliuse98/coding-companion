#include "ccobject.h"
#include "sprite.h"
#include "transform.h"
#include <vector>

namespace coco {

    CCObject::CCObject()
    : m_enabled(true)
    , m_sprite(nullptr)
    {
        m_transform.Rotation = glm::vec3(0,0,0);
        m_transform.Position = glm::vec3(0,0,0);
        m_transform.Scale = glm::vec3(1,1,1);
    }

    void CCObject::SetSprite(cabbage::Sprite* sprite)
    {
        m_sprite = sprite;
    }
    cabbage::Sprite* CCObject::GetSprite()
    {
        return m_sprite;
    }
    void CCObject::setScene(cabbage::Scene* scene)
    {
           m_scene = scene;
    }
    void CCObject::addChild(CCObject* object)
    {
        m_children.push_back(object);
    }
    std::vector<CCObject*> CCObject::GetChildren()
    {
        return m_children;
    }
    cabbage::Transform& CCObject::GetTransform()
    {
        return m_transform;
    }
    void CCObject::SetTransform(cabbage::Transform& transform)
    {
        m_transform = transform;
    }

}
