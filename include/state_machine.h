#pragma once

#include <string>
#include <unordered_map>

namespace coco
{

struct State
{
    std::string name;
    int         duration; // in ms
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
class Transition
{
    State* from;
    State* to;
    float  propability;
};
class StateMachine
{
    std::unordered_map<std::string, State> m_states;
};

} // namespace coco
