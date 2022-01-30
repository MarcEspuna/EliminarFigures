#include <vector>

#include "VertexArray.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>



VertexArray::VertexArray()
	: m_AttributesCount(0)
{
	glGenVertexArrays(1, &m_RendererID);
}
	
VertexArray::~VertexArray()
{
	glDeleteVertexArrays(1, &m_RendererID);
}

void VertexArray::Bind() const
{
	glBindVertexArray(m_RendererID);
}

void VertexArray::Unbind() const
{
	glBindVertexArray(0);
}

void VertexArray::AddBuffer(const VertexBuffer& vbo, const VertexArrayLayout& layout)
{
	uint16_t offset = 0;
	
	Bind();
	vbo.Bind();

	const auto& elements = layout.GetLayouts();

	for (const auto& element : elements)
	{
		glEnableVertexAttribArray(m_AttributesCount);
		glVertexAttribPointer(m_AttributesCount, element.count, element.type, element.normalized, layout.GetStride(), (const void*)offset);
		offset += element.GetSize() * element.count;
		m_AttributesCount++;
	}	

}
