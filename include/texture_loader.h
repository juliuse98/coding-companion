#pragma once

#include "texture.h"

namespace cabbage {

	class TextureLoader {
	public:
		static Texture* load(char const* filename);
		
	};

}
