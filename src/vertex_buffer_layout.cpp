#include "vertex_buffer_layout.h"

namespace cabbage {

	template<>
	void VertexBufferLayout::Push<float>(unsigned int count, unsigned int divisor) {
		m_Elements.push_back(VertexBufferElement{ GL_FLOAT, count, GL_FALSE, divisor });
		m_Stride += VertexBufferElement::GetSizeOfType(GL_FLOAT) * count;
	}

	template<>
	void VertexBufferLayout::Push<unsigned int>(unsigned int count, unsigned int divisor) {
		m_Elements.push_back(VertexBufferElement{ GL_UNSIGNED_INT, count, GL_FALSE, divisor });
		m_Stride += VertexBufferElement::GetSizeOfType(GL_UNSIGNED_INT) * count;
	}

	template<>
	void VertexBufferLayout::Push<int>(unsigned int count, unsigned int divisor) {
		m_Elements.push_back(VertexBufferElement{ GL_INT, count, GL_TRUE, divisor });
		m_Stride += VertexBufferElement::GetSizeOfType(GL_INT) * count;
	}

	template<>
	void VertexBufferLayout::Push<unsigned char>(unsigned int count, unsigned int divisor) {
		m_Elements.push_back(VertexBufferElement{ GL_UNSIGNED_BYTE, count, GL_TRUE, divisor });
		m_Stride += VertexBufferElement::GetSizeOfType(GL_UNSIGNED_BYTE) * count;
	}

}  // namespace cabbage

