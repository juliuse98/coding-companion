#pragma once

#include "nlohmann/json.hpp"
#include <fstream>
#include <optional>

#include "companion.h"
#include "graphics_manager.h"
#include "sprite.h"

#define CONFIG_LOGGING

#ifdef CONFIG_LOGGING
#include <easylogging++.h>
#else
#define LOG(x)
#endif

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
template <typename T> std::optional<T> getValue(const json& data, const std::string& key)
{
    if (data.contains(key))
    {
        try
        {
            return std::optional<T>(data.at(key).get<T>());
        } catch (...)
        {
            LOG(ERROR) << "Could not getValue with key : " << key << ". Maybe wrong Type";
            return std::nullopt;
        }
    }
    return std::nullopt;
}

bool loadTextures(json& companionData, cabbage::GraphicsManager& graphicsManager)
{
    std::optional<json> texturesData = getValue<json>(companionData, "textures");
    if (!texturesData)
    {
        LOG(ERROR) << "Companion data does not contain 'textures' tag.";
        return false;
    }
    for (auto textureData : *texturesData)
    {
        std::optional<std::string> name = getValue<std::string>(textureData, "name");
        if (!name)
        {
            LOG(ERROR) << "Texture does not contain a 'name' tag.";
            return false;
        }
        std::optional<std::string> filePath = getValue<std::string>(textureData, "filePath");
        if (!filePath)
        {
            LOG(ERROR) << "Texture does not contain a 'filePath' tag.";
            return false;
        }
        graphicsManager.loadTexture(*name, *filePath);
    }
    LOG(INFO) << "Loaded Textures";
    return true;
}
bool loadSpriteSheets(json& companionData, cabbage::GraphicsManager& graphicsManager, const std::string& companionName)
{
    std::optional<json> spriteSheetsData = getValue<json>(companionData, "spriteSheets");
    if (!spriteSheetsData)
    {
        LOG(ERROR) << "Companion data does not contain 'spriteSheets' tag.";
        return false;
    }

    LOG(INFO) << "Loading sprite sheets.";
    for (auto spriteSheetData : *spriteSheetsData)
    {
        std::optional<std::string> spriteSheetName = getValue<std::string>(spriteSheetData, "name");
        if (!spriteSheetName)
        {
            LOG(ERROR) << "Sprite sheet data does not contain 'name' tag.";
            return false;
        }

        LOG(INFO) << "Loading Texture Name";
        std::optional<std::string> textureName = getValue<std::string>(spriteSheetData, "texture");
        if (!textureName)
        {
            LOG(ERROR) << "Sprite sheet data does not contain 'texture' tag.";
            return false;
        }

        LOG(INFO) << "Loading Sprite Rects";
        std::optional<json> spriteRectsData = getValue<json>(spriteSheetData, "spriteRects");
        if (!spriteRectsData)
        {
            LOG(ERROR) << "Sprite sheet data does not contain 'spriteRects' tag.";
            return false;
        }
        LOG(INFO) << "Loading Sprite Rects";
        std::vector<cabbage::SpriteRect> spriteRects;
        for (auto spriteRectData : *spriteRectsData)
        {
            std::optional<int> x = getValue<int>(spriteRectData, "x");
            std::optional<int> y = getValue<int>(spriteRectData, "y");
            std::optional<int> width = getValue<int>(spriteRectData, "width");
            std::optional<int> height = getValue<int>(spriteRectData, "height");
            if (!x || !y || !width || !height)
            {
                LOG(ERROR) << "Could not read SpriteRect data of sprite sheet.";
                return false;
            }
            spriteRects.emplace_back(*x, *y, *width, *height);
        }
        graphicsManager.createSpriteSheet(companionName, *spriteSheetName, *textureName, spriteRects);
    }
    LOG(INFO) << "Loaded Sprite Sheets";
    return true;
}
bool loadAnimations(json& companionData, cabbage::GraphicsManager& graphicsManager, coco::Companion& companion)
{
    std::optional<json> animationsData = getValue<json>(companionData, "animations");
    if (!animationsData)
    {
        LOG(ERROR) << "Companion data does not contain 'animations' tag.";
        return false;
        // This could be true if its allowed to have objects without animation.
        // For testing this is false.
    }
    for (auto animationData : *animationsData)
    {
        std::optional<std::string> animationName = getValue<std::string>(animationData, "name");
        if (!animationName)
        {
            LOG(ERROR) << "Animation data does not contain 'name' tag.";
            return false;
        }
        std::optional<json> framesData = getValue<json>(animationData, "frames");
        if (!framesData)
        {
            LOG(ERROR) << "Animation data does not contain 'frames' tag.";
            return false;
        }
        std::vector<cabbage::Frame> frames;
        for (auto frameData : *framesData)
        {
            std::optional<int> id = getValue<int>(frameData, "id");
            if (!id)
            {
                LOG(ERROR) << "Frames data does not contain 'id' tag.";
                return false;
            }
            std::optional<std::string> spriteSheetName = getValue<std::string>(frameData, "spriteSheet");
            if (!spriteSheetName)
            {
                LOG(ERROR) << "Frames data does not contain 'spriteSheet' tag.";
                return false;
            }

            cabbage::SpriteSheet* spriteSheet = graphicsManager.getSpriteSheet(companion.GetName(), *spriteSheetName);
            std::optional<int>    spriteId = getValue<int>(frameData, "spriteId");
            if (!spriteId)
            {
                LOG(ERROR) << "Frames data does not contain 'spriteId' tag.";
                return false;
            }

            cabbage::Sprite* sprite = spriteSheet->getSprite(*spriteId);

            LOG(INFO) << "Gathered sprite with id: ";
            std::optional<int> duration = getValue<int>(frameData, "duration");
            if (!duration)
            {
                LOG(ERROR) << "Frames data does not contain 'duration' tag.";
                return false;
            }
            frames.emplace_back(*id, sprite, *duration);
        }
        companion.addAnimation(*animationName, frames);
    }
    LOG(INFO) << "Loaded Animations";
    return true;
}
bool getDefaultSprite(json& companionData, cabbage::GraphicsManager& graphicsManager, coco::Companion& companion)
{
    std::optional<json> defaultSpriteData = getValue<json>(companionData, "defaultSprite");
    if (!defaultSpriteData)
    {
        LOG(ERROR) << "Companion data does not contain 'defaultSprite' tag.";
        return false;
    }
    std::optional<std::string> defaultSpriteSheetName = getValue<std::string>(*defaultSpriteData, "spriteSheet");
    if (!defaultSpriteSheetName)
    {
        LOG(ERROR) << "Default sprite data does not contain 'spriteSheet' tag.";
        return false;
    }

    std::optional<int> spriteId = getValue<int>(*defaultSpriteData, "spriteId");
    if (!spriteId)
    {
        LOG(ERROR) << "Companion data does not contain 'spriteId' tag.";
        return false;
    }

    companion.SetSprite(
        graphicsManager.getSpriteSheet(companion.GetName(), *defaultSpriteSheetName)->getSprite(*spriteId)
    );
    LOG(INFO) << "Loaded Default Sprite";
    return true;
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
        LOG(INFO) << "Opened file " << filePath;
    }
    std::optional<std::string> companionName = getValue<std::string>(companionData, "name");
    if (!companionName)
    {
        LOG(ERROR) << "Companion data does not contain 'name' tag";
        return;
    }
    companion.SetName(*companionName);
    loadTextures(companionData, graphicsManager);
    loadSpriteSheets(companionData, graphicsManager, companion.GetName());
    loadAnimations(companionData, graphicsManager, companion);
    getDefaultSprite(companionData, graphicsManager, companion);
    LOG(INFO) << "Loaded Companion";
}
} // namespace CompanionFactory

} // namespace coco
