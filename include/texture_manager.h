#pragma once

#include "texture.h"
#include <GL/glew.h>
#include <unordered_map>
namespace cabbage
{

class TextureManager
{
  public:
    TextureManager()
    {
        glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &m_maxVertexTextureUnits);
    }
    int m_maxVertexTextureUnits = 0;
    std::unordered_map<cabbage::Texture*, int> texturesToTexureSlot;
    int AddTexture(cabbage::Texture* texture)
    {
        auto it = texturesToTexureSlot.find(texture);
        if (it != texturesToTexureSlot.end())
        {
            return it->second;
        }
        int newId = texturesToTexureSlot.size();
        texturesToTexureSlot[texture] = newId;
        return newId;
    };
    int bindTexture(cabbage::Texture* texture)
    {
        int slot = texturesToTexureSlot.find(texture)->second;
        texture->bind(slot);
        return slot;
    }
    int getMaxTextureSlots() { return m_maxVertexTextureUnits; };
};

} // namespace cabbage
