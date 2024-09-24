#pragma once

#include "sprite.h"
#include "texture.h"
#include "texture_loader.h"
#include <GL/glew.h>
#include <easylogging++.h>
#include <string>
#include <unordered_map>
#include <vector>
namespace cabbage
{

class GraphicsManager
{
  public:
    GraphicsManager()
    {
    }
    std::unordered_map<cabbage::Texture*, int>         texturesToTexureSlot;
    std::unordered_map<std::string, cabbage::Texture*> m_textures;

    std::unordered_map<std::string, std::unordered_map<std::string, cabbage::SpriteSheet*>>
        m_spriteSheets; // <name of Companion, <name of SpriteSheet, SpriteSheet*>>

    cabbage::Texture* loadTexture(const std::string& textureName, const std::string& filePath)
    {
        auto it = m_textures.find(textureName);
        if (it != m_textures.end())
        {
            return it->second;
        }
        auto* texture = cabbage::TextureLoader::load("resources/textures/" + filePath);
        m_textures.insert({textureName, texture});
        return texture;
    };
    cabbage::SpriteSheet* createSpriteSheet(
        const std::string& companionName, const std::string& spriteSheetName, const std::string& textureName,
        std::vector<cabbage::SpriteRect> spriteRects
    )
    {

        std::vector<cabbage::SpriteUVRect> spriteUvRects;

        cabbage::Texture* texture = m_textures.find(textureName)->second;

        int texWidth = texture->Width();
        int texHeight = texture->Height();

        for (const auto& spriteRect : spriteRects)
        {
            float x = static_cast<float>(spriteRect.X) / texWidth;
            float y = static_cast<float>(spriteRect.Y) / texHeight;
            float width = static_cast<float>(spriteRect.Width) / texWidth;
            float height = static_cast<float>(spriteRect.Height) / texHeight;

            spriteUvRects.emplace_back(x, y, width, height);
        }

        // Create the SpriteSheet object and store it in the map
        auto& companionMap = m_spriteSheets[companionName]; // Access or create the inner map
        companionMap.emplace(spriteSheetName, new SpriteSheet(texture, spriteUvRects));

        // Return a pointer to the newly created SpriteSheet
        return companionMap.find(spriteSheetName)->second;
    }
    cabbage::SpriteSheet* getSpriteSheet(const std::string& companionName, const std::string& spriteSheetName)
    {
        auto companionIt = m_spriteSheets.find(companionName);
        if (companionIt == m_spriteSheets.end())
        {
            return nullptr; // Companion not found
        }

        auto& spriteSheetMap = companionIt->second;
        auto  spriteSheetIt = spriteSheetMap.find(spriteSheetName);
        if (spriteSheetIt == spriteSheetMap.end())
        {
            return nullptr; // SpriteSheet not found
        }

        return spriteSheetIt->second;
    }
};

} // namespace cabbage
