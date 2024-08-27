#pragma once

namespace cabbage {

	struct Texture {
	public:
		Texture(int width, int height, int nrChannels, unsigned char* data);
		void bind();
		void unbind();

	private:
		int m_width;
		int m_height;
		int m_nrChannels;
		unsigned char* m_data;
		unsigned int m_glId;
	};

}
