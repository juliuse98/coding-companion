#include "texture_loader.h"
#include "texture.h"
#include <stb_image.h>
namespace cabbage {

	Texture* TextureLoader::load(char const* filename)
	{
		int width, height, nrChannels;
		unsigned char* data = stbi_load(filename, &width, &height, &nrChannels, 0);
		if(!data)
		{
			return nullptr;
		}
	  //TODO: maybe free the data some where after.  stbi_image_free(data);
		return new Texture(width,height,nrChannels,data);
	}

}
