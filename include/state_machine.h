#pragma once

#include <random>
#include <string>
#include <unordered_map>
namespace coco
{
class Companion;
class State;
class State
{
  public:
    std::string  name;
    Companion*   m_companion;
    State*       nextState;
    virtual void onUpdate(float deltaTime) {};
};
class StateSleeping : public State
{
  public:
    StateSleeping(Companion* companion, int repeats);
    StateSleeping(Companion* companion) : m_repeatCount(0)
    {
    }
    virtual void onUpdate(float deltaTime) override;
    void         onAnimationEnd();

  private:
    int                              m_repeatCount;
    std::random_device               rd;
    std::mt19937                     gen;
    std::uniform_real_distribution<> dis;
};
class StateMoving : public State
{
  public:
    StateMoving(Companion* companion);
    virtual void onUpdate(float deltaTime) override;

  private:
    int                              m_repeatCount;
    std::random_device               rd;
    std::mt19937                     gen;
    std::uniform_real_distribution<> dis;
};
class StateAwake : public State
{
  public:
    StateAwake(Companion* companion, int repeats);
    StateAwake(Companion* companion) : m_repeatCount(0)
    {
    }
    virtual void onUpdate(float deltaTime) override;
    void         onAnimationEnd();

  private:
    int m_repeatCount;
};
class StateMachine
{
  public:
    StateMachine(Companion* companion);
    void       setState();
    Companion* m_companion;
    void       Update(float deltaTime);
    State*     currentState;

    std::unordered_map<std::string, State> m_states;
};

} // namespace coco
