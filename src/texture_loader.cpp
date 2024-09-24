#include "texture_loader.h"
#include "texture.h"
#include <stb_image.h>
#include <string>
namespace cabbage
{

Texture* TextureLoader::load(const std::string& filename)
{
    int            width, height, nrChannels;
    unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nrChannels, 0);
    if (!data)
    {
        return nullptr;
    }
    // TODO: maybe free the data some where after.  stbi_image_free(data);
    return new Texture(width, height, nrChannels, data);
}

} // namespace cabbage
