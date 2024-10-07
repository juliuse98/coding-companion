#include "animation_controller.h"
#include "animation.h"
#include "companion.h"
#include "sprite.h"
#include <cstdlib>
#include <limits>
#include <optional>
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
    cabbage::Sprite* spriteToSet = m_currentAnimation->frames.at(currentFrame).sprite;
    m_companion->SetSprite(spriteToSet);
    timeSinceLastUpdate = 0.0f;
}
std::optional<std::string> AnimationController::GetAnimation()
{
    if (!m_currentAnimation) return std::nullopt;
    return std::optional<const std::string>(m_currentAnimation->name);
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
    currentFrame++;
    if (currentFrame >= m_currentAnimation->frames.size())
    {
        animationEnd.fire();
        currentFrame = 0;
    }
    cabbage::Sprite* spriteToSet = m_currentAnimation->frames.at(currentFrame).sprite;
    m_companion->SetSprite(spriteToSet);
}

} // namespace coco
