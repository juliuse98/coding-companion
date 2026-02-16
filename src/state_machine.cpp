#include "companion.h"
#include "nano_signal_slot.hpp"
#include <random>
#include <state_machine.h>

namespace coco
{

StateSleeping::StateSleeping(Companion* companion, int repeats) : m_repeatCount(repeats)
{
    m_companion = companion;
    gen = std::mt19937(rd());                         // Mersenne Twister engine
    dis = std::uniform_real_distribution<>(0.0, 1.0); // Range [0, 1]

    // Generate a random number between 0 and 1
}

void StateSleeping::onUpdate(float deltaTime)
{
    // LOG(INFO) << "SleepingState Update";
    if (m_companion->getAnimationController().GetAnimation() != "sleeping")
    {
        m_companion->getAnimationController().animationEnd.connect<&StateSleeping::onAnimationEnd>(this);
        m_companion->getAnimationController().PlayAnimation("sleeping");
    }
}
void StateSleeping::onAnimationEnd()
{
    LOG(INFO) << "onAnimationEnd" << m_repeatCount;
    if (m_repeatCount <= 0)
    {
        // go to next animation
        double random_number = dis(gen);
        if (random_number < 0.1)
        {
            LOG(INFO) << "Go to next animation";
            m_companion->getAnimationController().animationEnd.disconnect<&StateSleeping::onAnimationEnd>(this);
            m_companion->getStateMachine().currentState = nextState;

            m_repeatCount = 3;
            return;
        }
    }
    m_repeatCount--;
}
StateMoving::StateMoving(Companion* companion)
{
}
void StateMoving::onUpdate(float deltaTime)
{
}

StateAwake::StateAwake(Companion* companion, int repeats) : m_repeatCount(repeats)
{
    m_companion = companion;
}
void StateAwake::onUpdate(float deltaTime)
{
    // LOG(INFO) << "SleepingState Update";
    if (m_companion->getAnimationController().GetAnimation() != "awake")
    {
        m_companion->getAnimationController().animationEnd.connect<&StateAwake::onAnimationEnd>(this);
        m_companion->getAnimationController().PlayAnimation("awake");
    }
}
void StateAwake::onAnimationEnd()
{
    LOG(INFO) << "Go to next animation";
    // go to next animation
    m_companion->getAnimationController().animationEnd.disconnect<&StateAwake::onAnimationEnd>(this);
    m_companion->getStateMachine().currentState = nextState;
}
StateMachine::StateMachine(Companion* companion) : m_companion(companion) {};
void StateMachine::setState()
{
}
void StateMachine::Update(float deltaTime)
{
    // LOG(INFO) << "StateMachine: UPDATE";
    if (!currentState) return;
    currentState->onUpdate(deltaTime);
}
} // namespace coco
