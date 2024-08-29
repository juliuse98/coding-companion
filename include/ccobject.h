#pragma once

#include "transform.h"
#include <vector>
namespace cabbage
{
    class Sprite;
}
namespace coco {

	class CCObject
	{
    public:
        CCObject();
        void SetSprite(cabbage::Sprite* sprite);
    private:
        bool m_enabled;
        cabbage::Transform m_transform;
        cabbage::Sprite* m_sprite;
        std::vector<CCObject*> m_children;
	};

}
