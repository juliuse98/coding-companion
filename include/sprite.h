#pragma once

#include "ccobject.h"
#include "texture.h"
#include <cstddef>
#include <vector>
namespace cabbage {

	struct UVCoordinate
	{
		float u;
		float v;
		UVCoordinate(float u, float v)
			: u(u), v(v) {}
	};
	struct SpriteUVRect : public UVCoordinate
	{
		float width;
		float height;

		SpriteUVRect(float u, float v, float width, float height)
			: UVCoordinate(u, v), width(width), height(height) {}
	};

	class SpriteSheet
	{
	public:
		Texture* m_texture;
		std::vector<SpriteUVRect> m_spriteUVRects;
		SpriteSheet(Texture* texture);
		SpriteSheet(Texture* texutre, std::vector<SpriteUVRect>& spriteUVRects);
		SpriteUVRect getSpriteUVRect(unsigned int index);
		unsigned int SpriteCount(){ return m_spriteUVRects.size(); }
		void addSpriteUVRect(float u, float v, float width, float height);
	};

	class Sprite
	{
		Texture* m_texture;	
		SpriteUVRect m_uvRect;
    public:
        Sprite(Texture* texture = nullptr) : m_texture(texture), m_uvRect(0.0f, 0.0f, 1.0f, 1.0f){};
        Sprite(SpriteUVRect& uvRect, Texture* texture = nullptr) : m_texture(texture), m_uvRect(uvRect){};
	};

    class SpriteSheet1
    {
		std::vector<Sprite> m_sprites;
        SpriteSheet1(Texture* = nullptr);
		SpriteUVRect getSpriteUVRect(unsigned int index);
        Sprite getSprite(size_t index);
		size_t SpriteCount(){ return m_sprites.size(); }
		void addSubSprite(float u, float v, float width, float height);
    };

}
namespace coco{
    namespace components {
        class SpriteAnimation
        {
            CCObject& m_parent;
            cabbage::SpriteSheet1* m_spriteSheet;
        public:
            SpriteAnimation(CCObject& parent, cabbage::SpriteSheet* spriteSheet);
        };
    }

}
