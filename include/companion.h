#pragma once

#include "animation.h"
#include "ccobject.h"
#include "state_machine.h"
#include "graphics_manager.h"
#include <animation_controller.h>
#include <easylogging++.h>
#include <memory>
#include <unordered_map>
#include <vector>
namespace coco
{
enum class CompanionState
{
    SLEEPING,
    PEEKING
};

class Companion : public CCObject
{
  public:
    
  public:
    Companion() : m_animationController(std::make_unique<coco::AnimationController>(this)), currentState(CompanionState::PEEKING)
    {
        m_animationController->animationEnd.connect<&Companion::onAnimationEnd>(this);
        
        StateAwake* awake = new StateAwake(this,1);
        StateSleeping* sleeping = new StateSleeping(this,2);
        sleeping->nextState = awake;
        awake->nextState = sleeping;
        m_stateMachine->currentState = sleeping;
    };
    void SetName(const std::string& name)
    {
        m_name = name;
    }
    const std::string& GetName()
    {
        return m_name;
    }
    coco::AnimationController& getAnimationController(){ return *m_animationController; }
    coco::StateMachine& getStateMachine(){ return *m_stateMachine; }

    void addAnimation(std::string& name, std::vector<cabbage::Frame>& frames)
    {
        m_animationController->addAnimation(name, frames);
    }
    void PlayAnimation(const std::string& animationName)
    {
        m_animationController->PlayAnimation(animationName);
    }
    void StopAnimation()
    {
        m_animationController->StopAnimation();
    }

  private:
    void update(float deltaTime) override
    {
        // LOG(INFO) << "UPDATE";
       // if (currentState == CompanionState::SLEEPING)
       // {
       //     if (shouldDisconnect)
       //     {
       //         m_animationController->animationEnd.disconnect<&Companion::onAnimationEnd>(this);
       //     }
       //     // LOG(INFO) << "SLEEPING";
       //     std::optional<std::string> name = m_animationController->GetAnimation();
       //     if (name != "sleeping")
       //     {
       //         m_animationController->PlayAnimation("sleeping");
       //     }
       // }
       // else if (currentState == CompanionState::PEEKING)
       // {
       //     // LOG(INFO) << "PEEKING";
       //     if (m_animationController->GetAnimation() != "awake")
       //     {
       //         m_animationController->PlayAnimation("awake");
       //         shouldDisconnect = false;
       //     }
       // }
        m_animationController->update(deltaTime);
        m_stateMachine->Update(deltaTime);
    }
    void onAnimationEnd()
    {
        // LOG(INFO) << "onAnimationEnd";
        if (currentState == CompanionState::SLEEPING)
        {
            counter++;
            if (counter > 2)
            {
                currentState = CompanionState::PEEKING;
                counter = 0;
            }
        }
        else if (currentState == CompanionState::PEEKING)
        {
            currentState = CompanionState::SLEEPING;
        }
    }
    cabbage::GraphicsManager* m_graphicsManager;
    coco::StateMachine* m_stateMachine;

    std::unordered_map<std::string, cabbage::Animation> m_animations; // <animationName, animation>
    std::unique_ptr<coco::AnimationController> m_animationController;
    //coco::AnimationController m_animationController;

    std::string    m_name;
    bool           shouldDisconnect = false;
    int            counter = 0;
    CompanionState currentState;
};

} // namespace coco
