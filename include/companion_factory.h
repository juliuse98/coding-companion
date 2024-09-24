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
    CCLOG("After ifstream , " << filePath);

    json companionData;

    if (file.is_open())
    {
        file >> companionData;
        CCLOG("file.is_open()" << companionData["name"]);
    }
    std::string companionName = companionData["name"];
    CCLOG("getValue" << companionName);
    companion.SetName(companionName);
    CCLOG("setName" << companionData.contains("textures"));
    CCLOG("setName" << companionData.at("textures")[0].at("name"));
    for (auto textureData : companionData["textures"])
    {
        const std::string name = textureData.at("name").get<std::string>();
        CCLOG("getValue" << name);

        const std::string filePath = textureData.at("filePath").get<std::string>();
        CCLOG("getValue" << name);
        graphicsManager.loadTexture(name, filePath);
        CCLOG("getValue" << name);
    }
    CCLOG("getValue");
    for (auto spriteSheetData : companionData["spriteSheets"])
    {
        std::string spriteSheetName = spriteSheetData.at("name").get<std::string>();
        CCLOG("getValue HHHAAAAA");

        std::string textureName = spriteSheetData.at("texture").get<std::string>();
        CCLOG("getValue HHHAAAAA");

        std::vector<cabbage::SpriteRect> spriteRects;
        CCLOG("getValue HHHAAAAA");
        CCLOG("getValue HHHAAAAA " << spriteSheetData);

        for (auto spriteRectData : spriteSheetData["spriteRects"])
        {
            int x = spriteRectData["x"].get<int>();
            int y = spriteRectData["y"].get<int>();
            int width = spriteRectData["width"].get<int>();
            int height = spriteRectData["height"].get<int>();
            spriteRects.emplace_back(x, y, width, height);
        }

        CCLOG("getValue HHHAAAAA End spriteRects, Size of spriteRects: " << spriteRects.size());
        graphicsManager.createSpriteSheet(companionName, spriteSheetName, textureName, spriteRects);
        CCLOG("getValue HHHAAAAA End Loop");
    }
    CCLOG("getValue HHHAAAAA");
    std::string defaultSpriteSheetName = companionData["defaultSprite"]["spriteSheet"].get<std::string>();

    int spriteId = companionData["defaultSprite"]["spriteId"].get<int>();

    companion.SetSprite(graphicsManager.getSpriteSheet(companionName, defaultSpriteSheetName)->getSprite(spriteId));
}
} // namespace CompanionFactory

} // namespace coco
