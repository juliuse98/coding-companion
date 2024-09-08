#include "ccobject.h"
#include "sprite.h"
#include "texture.h"
#include "transform.h"

namespace coco
{

CCObject::CCObject() : m_enabled(true), m_sprite(nullptr)
{
    m_transform.SetRotation(glm::vec3(0, 0, 0), true);
    m_transform.SetPosition(glm::vec3(0, 0, 0), true);
    m_transform.SetScale(glm::vec3(1, 1, 1), true);
    m_transform.changed.connect<&CCObject::onTransformChanged>(this);
}

cabbage::Texture* CCObject::GetTexture() const
{
    return m_sprite->GetTexutre();
}
void CCObject::SetSprite(cabbage::Sprite* sprite)
{
    // if (m_sprite->GetTexutre() != sprite->GetTexutre())
    //{
    //     m_dirtyElements.insert(CCObjectDirtyElement::Sprite);
    // }
    // if (m_sprite->GetUVRect() != sprite->GetUVRect())
    //{
    //     m_dirtyElements.insert(CCObjectDirtyElement::Sprite);
    // }
    m_sprite = sprite;
}
cabbage::Sprite* CCObject::GetSprite() { return m_sprite; }
void CCObject::setScene(cabbage::Scene* scene) { m_scene = scene; }
void CCObject::addChild(CCObject* object)
{
    m_children.push_back(object);
    m_dirtyElements.insert(CCObjectDirtyElement::Children);
}
std::vector<CCObject*> CCObject::GetChildren() { return m_children; }
cabbage::Transform& CCObject::GetTransform() { return m_transform; }
void CCObject::SetTransform(cabbage::Transform& transform)
{
    m_transform.SetPosition(transform.Position(), true);
    m_transform.SetRotation(transform.Rotation(), true);
    m_transform.SetScale(transform.Scale(), true);
    onTransformChanged();
}
void CCObject::onTransformChanged()
{
    m_dirtyElements.insert(CCObjectDirtyElement::Transform);
}

} // namespace coco
