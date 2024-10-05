#include "animation_controller.h"
#include "animation.h"
#include "companion.h"
#include "sprite.h"
#include <cstdlib>
#include <limits>
namespace coco
{

AnimationController::AnimationController(coco::Companion* companion)
    : m_companion(companion), m_currentAnimation(nullptr)
{
}

void AnimationController::addAnimation(std::string& name, std::vector<cabbage::Frame>& frames)
{
    m_animations.emplace(name, cabbage::Animation{name, frames});
}
void AnimationController::PlayAnimation(const std::string& animationName)
{
    m_currentAnimation = &m_animations.at(animationName);

    currentFrame = 0;
    timeSinceLastUpdate = std::numeric_limits<float>::infinity();
}
void AnimationController::StopAnimation()
{
    m_currentAnimation = nullptr;
}
void AnimationController::update(float deltaTime)
{
    float animationSpeed = 1.0f;
    if (m_currentAnimation == nullptr || m_currentAnimation->frames.size() == 0) return;
    timeSinceLastUpdate += deltaTime;
    if (timeSinceLastUpdate < m_currentAnimation->frames.at(currentFrame).duration * 0.001f * animationSpeed) return;
    timeSinceLastUpdate = 0.0f;
    cabbage::Sprite* sprite;
    m_companion->SetSprite(m_currentAnimation->frames.at(currentFrame).sprite);
    currentFrame += 1 * int(animationSpeed / abs(animationSpeed));
    if (currentFrame >= m_currentAnimation->frames.size()) currentFrame = 0;
}

} // namespace coco
