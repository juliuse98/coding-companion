#pragma once

#include "ccobject.h"
#include "texture.h"
#include <array>
#include <cstddef>
#include <vector>
namespace cabbage
{

struct UVCoordinate
{
    float u;
    float v;
    UVCoordinate(float u, float v) : u(u), v(v)
    {
    }
};
struct SpriteUVRect : public UVCoordinate
{
    float width;
    float height;

    SpriteUVRect(float u, float v, float width, float height) : UVCoordinate(u, v), width(width), height(height)
    {
    }
    std::array<float, 8> toUVArray()
    {
        return {u, v + height, u + width, v + height, u, v, u + width, v};
    };
    bool operator==(const SpriteUVRect& other) const
    {
        return (u == other.u && v == other.v && width == other.width && height == other.height);
    }
    bool operator!=(const SpriteUVRect& other) const
    {
        return !(*this == other);
    }
};
struct SpriteRect
{
    int X;
    int Y;
    int Width;
    int Height;

    SpriteRect(int x, int y, int width, int height) : X(x), Y(y), Width(width), Height(height)
    {
    }
};

class SpriteSheet
{
  public:
    Texture*            m_texture;
    std::vector<Sprite> m_sprites;
    SpriteSheet(Texture* texture);
    SpriteSheet(Texture* texutre, std::vector<SpriteUVRect>& spriteUVRects);
    Sprite* getSprite(int spriteId)
    {
        return &m_sprites[spriteId];
    }
    unsigned int SpriteCount()
    {
        return m_sprites.size();
    }
};

class Sprite
{
  private:
    Texture*     m_texture;
    SpriteUVRect m_uvRect;

  public:
    Sprite(Texture* texture = nullptr) : m_texture(texture), m_uvRect(0.0f, 0.0f, 1.0f, 1.0f) {};
    Sprite(SpriteUVRect& uvRect, Texture* texture = nullptr) : m_texture(texture), m_uvRect(uvRect) {};
    SpriteUVRect& GetUVRect()
    {
        return m_uvRect;
    };
    void SetUVRect(const SpriteUVRect& uvRect)
    {
        m_uvRect = uvRect;
    };
    Texture* GetTexutre()
    {
        return m_texture;
    };
};

class SpriteSheet1
{
    std::vector<Sprite> m_sprites;
    SpriteSheet1(Texture* = nullptr);
    SpriteUVRect getSpriteUVRect(unsigned int index);
    Sprite       getSprite(size_t index);
    size_t       SpriteCount()
    {
        return m_sprites.size();
    }
    void addSubSprite(float u, float v, float width, float height);
};

} // namespace cabbage
namespace coco
{
namespace components
{
class SpriteAnimation
{
    CCObject&              m_parent;
    cabbage::SpriteSheet1* m_spriteSheet;

  public:
    SpriteAnimation(CCObject& parent, cabbage::SpriteSheet* spriteSheet);
};
} // namespace components

} // namespace coco
