#pragma once

namespace cabbage
{

struct Texture
{
  public:
    Texture(int width, int height, int nrChannels, unsigned char* data);
    ~Texture();
    void bind();
    void bind(unsigned int textureSlot);
    void unbind();
    int  Width()
    {
        return m_width;
    }
    int Height()
    {
        return m_height;
    }

  private:
    int            m_width;
    int            m_height;
    int            m_nrChannels;
    unsigned char* m_data;
    unsigned int   m_glId;
};

} // namespace cabbage
