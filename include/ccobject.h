#pragma once

#include "transform.h"
#include <vector>
namespace cabbage
{
    class Scene;
    class Sprite;
}
namespace coco {

	class CCObject
	{
    public:
        CCObject();
        void SetSprite(cabbage::Sprite* sprite);
    cabbage::Sprite* GetSprite();
        void setScene(cabbage::Scene* scene);
        void addChild(CCObject* object);
        std::vector<CCObject*> GetChildren();
    protected:
    private:
        bool m_enabled;
        cabbage::Scene* m_scene;
        cabbage::Transform m_transform;
        cabbage::Sprite* m_sprite;
        std::vector<CCObject*> m_children;
	};

}
