#pragma once

#include "ccobject.h"
#include <vector>
namespace cabbage {
    class Scene
    {
    public:
        Scene();
        coco::CCObject* root; 
        std::vector<coco::CCObject> m_objectsInScene;
        
    };
}
