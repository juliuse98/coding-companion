#include "sprite.h"
#include "texture.h"
#include <cassert>
#include <vector>

namespace cabbage
{

SpriteSheet::SpriteSheet(Texture* texture, std::vector<SpriteUVRect>& spriteUVRects) : m_texture(texture)
{
    for (auto spriteUVRect : spriteUVRects)
    {
        m_sprites.emplace_back(spriteUVRect, texture);
    }
}
} // namespace cabbage
