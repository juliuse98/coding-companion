#pragma once

#include "nlohmann/json.hpp"
#include <easylogging++.h>
#include <fstream>

#include "companion.h"
#include "graphics_manager.h"
#include "sprite.h"

#define CCLOG(x) LOG(INFO) << x
namespace coco
{

namespace CompanionFactory
{
using json = nlohmann::json;
template <typename T> T getValue(const json& data, const std::string& key, const T defaultValue)
{
    if (data.contains(key))
    {
        return data.at(key).get<T>();
    }
    else
    {
        return defaultValue;
    }
}
template <typename T> T getValue(const json& data, const std::string& key, bool& success)
{
    if (data.contains(key))
    {
        success = true;
        return data.at(key).get<T>();
    }
}
inline void
loadCompanion(coco::Companion& companion, const std::string& filePath, cabbage::GraphicsManager& graphicsManager)
{

    CCLOG("Start loadCompanion , " << filePath);
    std::ifstream file(filePath);

    json companionData;

    if (file.is_open())
    {
        file >> companionData;
    }
    std::string companionName = companionData["name"];
    companion.SetName(companionName);
    for (auto textureData : companionData["textures"])
    {
        const std::string name = textureData.at("name").get<std::string>();

        const std::string filePath = textureData.at("filePath").get<std::string>();
        graphicsManager.loadTexture(name, filePath);
    }
    for (auto spriteSheetData : companionData["spriteSheets"])
    {
        std::string spriteSheetName = spriteSheetData.at("name").get<std::string>();
        std::string textureName = spriteSheetData.at("texture").get<std::string>();

        std::vector<cabbage::SpriteRect> spriteRects;
        for (auto spriteRectData : spriteSheetData["spriteRects"])
        {
            int x = spriteRectData["x"].get<int>();
            int y = spriteRectData["y"].get<int>();
            int width = spriteRectData["width"].get<int>();
            int height = spriteRectData["height"].get<int>();
            spriteRects.emplace_back(x, y, width, height);
        }

        graphicsManager.createSpriteSheet(companionName, spriteSheetName, textureName, spriteRects);
    }
    std::string defaultSpriteSheetName = companionData["defaultSprite"]["spriteSheet"].get<std::string>();

    int spriteId = companionData["defaultSprite"]["spriteId"].get<int>();

    companion.SetSprite(graphicsManager.getSpriteSheet(companionName, defaultSpriteSheetName)->getSprite(spriteId));

    for (auto animationData : companionData["animations"])
    {
        std::string                 animationName = animationData["name"].get<std::string>();
        std::vector<cabbage::Frame> frames;
        for (auto frameData : animationData["frames"])
        {
            const int              id = frameData["id"].get<int>();
            const cabbage::Sprite* sprite =
                graphicsManager.getSpriteSheet(companionName, animationData["name"].get<std::string>())
                    ->getSprite(frameData["spriteId"].get<int>());
            const int duration = frameData["id"].get<int>();
            frames.emplace_back(id, sprite, duration);
        }
        companion.addAnimation(animationName, frames);
    }
}
} // namespace CompanionFactory

} // namespace coco
