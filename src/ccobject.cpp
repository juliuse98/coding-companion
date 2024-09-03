#include "ccobject.h"
#include "sprite.h"
#include <vector>

namespace coco {

    CCObject::CCObject()
    : m_enabled(true)
    , m_sprite(nullptr)
    {}

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

}
