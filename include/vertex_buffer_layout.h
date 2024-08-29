#pragma once

#include <vector>
#include "renderer.h"

namespace cabbage {

	struct VertexBufferElement {
		unsigned int type;
		unsigned int count;
		unsigned char normalized;

		static unsigned int GetSizeOfType(unsigned int type) {
			switch (type) {
				case GL_FLOAT:           return 4;
				case GL_UNSIGNED_INT:    return 4;
				case GL_UNSIGNED_BYTE:   return 1;
			};
			ASSERT(false);
			return 0;
		};
	};

	class VertexBufferLayout {
	private:
		std::vector<VertexBufferElement> m_Elements;
		unsigned int m_Stride;

	public:
		VertexBufferLayout() : m_Stride(0) {}

		template<typename T>
		void Push(unsigned int count) {
			// Causes a compile-time error if an unsupported type is used
			static_assert(sizeof(T) == 0, "Unsupported type in VertexBufferLayout::Push");
		}

		inline const std::vector<VertexBufferElement>& GetElements() const { return m_Elements; }
		inline unsigned int GetStride() const { return m_Stride; }
	};

	template<>
	void VertexBufferLayout::Push<float>(unsigned int count);

	template<>
	void VertexBufferLayout::Push<unsigned int>(unsigned int count);

	template<>
	void VertexBufferLayout::Push<unsigned char>(unsigned int count);

}  // namespace cabbage
