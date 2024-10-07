#pragma once

#include "nano_signal_slot.hpp"
#include <cstdlib>
#include <string>
#include <unordered_map>
#include <vector>

namespace coco
{
struct State
{
    std::string name;
    // Slots
    virtual void onUpdate() = 0;
};
class ConstantState
{
};
class RangeState
{
};
class SingleShotState
{
};
struct Transition
{
    State* from;
    State* to;
    float  propability;
};
class StateMachine
{
  public:
    StateMachine() {};
    void Update()
    {
        float random = ((float)rand()) / (float)RAND_MAX;
        auto  transitions = m_transitions.find(currentState->name);
        if (transitions == m_transitions.end()) return;

        float totalPropability = 0.0f;
        for (Transition transition : transitions->second)
        {
            totalPropability += transition.propability;
            if (random < totalPropability)
            {
                currentState = transition.to;
                break;
            }
        }
    }
    State* currentState;

    std::unordered_map<std::string, State>                   m_states;
    std::unordered_map<std::string, std::vector<Transition>> m_transitions; // <stateName, std::vector<Transitions>>
};

} // namespace coco
