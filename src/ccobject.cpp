#include "ccobject.h"
#include "sprite.h"

namespace coco {

    CCObject::CCObject()
    : m_enabled(true)
    , m_sprite(nullptr)
    {}

    void CCObject::SetSprite(cabbage::Sprite* sprite)
    {
        m_sprite = sprite;
    }
}
