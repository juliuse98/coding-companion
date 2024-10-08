#include "texture.h"

#include <GL/glew.h>
#include <cstdlib>

namespace cabbage
{

Texture::Texture(int width, int height, int nrChannels, unsigned char* data)
    : m_width(width), m_height(height), m_nrChannels(nrChannels), m_data(data)
{
    glGenTextures(1, &m_glId);
}
Texture::~Texture()
{
    if (m_data)
    {
        free(m_data);
        m_data = nullptr;
    }
}
void Texture::bind()
{
    glBindTexture(GL_TEXTURE_2D, m_glId);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_data);
}
void Texture::bind(unsigned int textureSlot)
{
    glActiveTexture(GL_TEXTURE0 + textureSlot);
    glBindTexture(GL_TEXTURE_2D, m_glId);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_data);
}
void Texture::unbind()
{
    glBindTexture(GL_TEXTURE_2D, 0);
}

} // namespace cabbage
