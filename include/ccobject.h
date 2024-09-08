#pragma once

#include "texture.h"
#include "transform.h"
#include <nano_signal_slot.hpp>
#include <unordered_set>
#include <vector>

namespace cabbage
{
class Scene;
class Sprite;
} // namespace cabbage
//
namespace coco
{
enum class CCObjectDirtyElement
{
    Sprite,
    Children,
    Enabled,
    Transform
};

class CCObject
{
  public:
    CCObject();
    void SetSprite(cabbage::Sprite* sprite);
    cabbage::Sprite* GetSprite();
    void setScene(cabbage::Scene* scene);
    void addChild(CCObject* object);
    cabbage::Texture* GetTexture() const;

  public:
    std::vector<CCObject*> GetChildren();
    cabbage::Transform& GetTransform();
    void SetTransform(cabbage::Transform& transform);
    bool isDirty() const { return m_dirtyElements.size() > 0; }
    const std::unordered_set<CCObjectDirtyElement>& GetDirtyElements() const
    {
        return m_dirtyElements;
    }

  private:
    bool m_enabled;
    cabbage::Scene* m_scene;
    cabbage::Transform m_transform;
    cabbage::Sprite* m_sprite;
    std::vector<CCObject*> m_children;
    std::unordered_set<CCObjectDirtyElement> m_dirtyElements;
    void onTransformChanged();
};

} // namespace coco
