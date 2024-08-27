#pragma once

#include "vertex_buffer.h"
namespace cabbage {

	class VertexBufferLayout;

		class VertexArray
		{
		private:
		public:
			unsigned int m_RendererId;
			unsigned int m_lastAttribArrayId;
			VertexArray();
			~VertexArray();
			void AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout);

			void Bind() const;
			void Unbind() const;
		};

}
