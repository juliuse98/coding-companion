#include "texture.h"

#include <GL/glew.h>

namespace cabbage {

	Texture::Texture(int width, int height, int nrChannels, unsigned char* data)
	: m_width(width)
	, m_height(height)
	, m_nrChannels(nrChannels)
	, m_data(data)
	{
		glGenTextures(1, &m_glId);
		glActiveTexture(GL_TEXTURE0);
	};
	void Texture::bind()
	{
		glBindTexture(GL_TEXTURE_2D, m_glId);	
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
	    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_data);
	    glGenerateMipmap(GL_TEXTURE_2D);
	}
	void Texture::unbind()
	{
		glBindTexture(GL_TEXTURE_2D, 0);	
	}

}
