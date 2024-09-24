#pragma once

#include "texture.h"
#include <string>

namespace cabbage
{

class TextureLoader
{
  public:
    static Texture* load(const std::string& filename);
};

} // namespace cabbage
