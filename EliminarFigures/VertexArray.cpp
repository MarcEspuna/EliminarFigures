#include <vector>

#include "VertexArray.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>



VertexArray::VertexArray()
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
	size_t offset = 0;
	
	Bind();
	vbo.Bind();

	const auto& elements = layout.GetLayouts();

	for (unsigned int i = 0; i < elements.size(); i++)
	{
		glEnableVertexAttribArray(i);
		glVertexAttribPointer(i, elements[i].count, elements[i].type, elements[i].normalized, layout.GetStride(), (void*)offset);
		offset += elements[i].GetSize() * elements[i].count;

	}
}
