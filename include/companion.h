#pragma once

#include "animation.h"
#include "ccobject.h"
#include "graphics_manager.h"
#include <animation_controller.h>
#include <unordered_map>
#include <vector>
namespace coco
{

class Companion : public CCObject
{
  public:
    Companion() : m_animationController(this) {};
    void SetName(const std::string& name)
    {
        m_name = name;
    }
    const std::string& GetName()
    {
        return m_name;
    }
    void addAnimation(std::string& name, std::vector<cabbage::Frame>& frames)
    {
        m_animationController.addAnimation(name, frames);
    }
    void PlayAnimation(const std::string& animationName)
    {
        m_animationController.PlayAnimation(animationName);
    }
    void StopAnimation()
    {
        m_animationController.StopAnimation();
    }

  private:
    void update(float deltaTime) override
    {
        m_animationController.update(deltaTime);
    }
    cabbage::GraphicsManager* m_graphicsManager;

    std::unordered_map<std::string, cabbage::Animation> m_animations; // <animationName, animation>
    //
    coco::AnimationController m_animationController;

    std::string m_name;
};

} // namespace coco
