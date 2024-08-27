#pragma once

#include "texture.h"
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

}
