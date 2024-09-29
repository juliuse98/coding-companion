#pragma once

#include "sprite.h"
#include <vector>
namespace cabbage
{

struct Frame
{
    int     id;
    Sprite* sprite;
    int     duration;
    Frame(int id, Sprite* sprite, int duration) : id(id), sprite(sprite), duration(duration)
    {
    }
};

struct Animation
{
    std::string name;

    std::vector<Frame> frames;
    Animation(const std::string& name, const std::vector<cabbage::Frame>& frames) : name(name), frames(frames)
    {
    }
};

} // namespace cabbage
