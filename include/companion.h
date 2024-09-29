#pragma once

#include "animation.h"
#include "ccobject.h"
#include "graphics_manager.h"
#include <unordered_map>
#include <vector>
namespace coco
{

class Companion : public CCObject
{
  public:
    Companion() {};
    void SetName(const std::string& name)
    {
        m_name = name;
    }
    const std::string& GetName()
    {
        return m_name;
    }
    void addAnimation(std::string name, std::vector<cabbage::Frame>& frames)
    {
        m_animations.emplace(name, cabbage::Animation{name, frames});
    }

  private:
    cabbage::GraphicsManager* m_graphicsManager;

    std::unordered_map<std::string, cabbage::Animation*> m_animations; // <animationName, animation>
    // cabbage::AnimationController m_animationController;

    std::string m_name;
};

} // namespace coco
