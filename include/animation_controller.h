#pragma once

#include "animation.h"
#include <optional>
#include <vector>
namespace coco
{
class Companion;
class AnimationController
{
  public:
    AnimationController(coco::Companion* companion);
    void addAnimation(std::string& name, std::vector<cabbage::Frame>& frames);
    void PlayAnimation(const std::string& animationName);
    void StopAnimation();
    void update(float deltaTime);

    std::optional<std::string> GetAnimation();
    Nano::Signal<void()>       animationEnd;

  private:
    coco::Companion* m_companion;

    cabbage::Animation* m_currentAnimation;
    int                 currentFrame;
    float               timeSinceLastUpdate = 0.0f;

    std::unordered_map<std::string, cabbage::Animation> m_animations; // <animationName, animation>
};

} // namespace coco
