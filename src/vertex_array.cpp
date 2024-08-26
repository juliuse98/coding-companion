#include "VertexArray.h"

#include "Renderer.h"
#include "VertexBufferLayout.h"
#include <iostream>

namespace cabbage {
	VertexArray::VertexArray() : m_lastAttribArrayId(0)
	{
		GLCall(glGenVertexArrays(1, &m_RendererId));
	}

	VertexArray::~VertexArray()
	{
		GLCall(glDeleteVertexArrays(1, &m_RendererId));
	}

	void VertexArray::AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout)
	{
		Bind();
		vb.Bind();
		const auto& elements = layout.GetElements();

		unsigned int offset = 0;
		for (unsigned int i = 0; i < elements.size(); ++i) {
			const auto& element = elements[i];
			GLCall(glEnableVertexAttribArray(m_lastAttribArrayId + i));
			GLCall(glVertexAttribPointer(m_lastAttribArrayId + i, element.count, element.type, element.normalized, layout.GetStride(), (const void*)offset));
			offset += element.count * VertexBufferElement::GetSizeOfType(element.type);
		}
		m_lastAttribArrayId = m_lastAttribArrayId + elements.size();
	}

	void VertexArray::Bind() const
	{
		GLCall(glBindVertexArray(m_RendererId));
	}

	void VertexArray::Unbind() const
	{
		GLCall(glBindVertexArray(0));
	}
}
