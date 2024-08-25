#include "sprite.h"
#include "texture.h"
#include <cassert>
#include <vector>

SpriteSheet::SpriteSheet(Texture* texture)
: m_texture(texture)
, m_spriteUVRects(std::vector<SpriteUVRect>())
{}

SpriteSheet::SpriteSheet(Texture* texutre, std::vector<SpriteUVRect>& spriteUVRects)
: m_texture(texutre)
, m_spriteUVRects(spriteUVRects)
{}

SpriteUVRect SpriteSheet::getSpriteUVRect(unsigned int index)
{
	assert(index < m_spriteUVRects.size());
	return m_spriteUVRects.at(index);
}


void SpriteSheet::addSpriteUVRect(float u, float v, float width, float height)
{
	m_spriteUVRects.push_back(SpriteUVRect(u, v, width, height));
}
