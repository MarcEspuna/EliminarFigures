#include "IndexBuffer.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>


IndexBuffer::IndexBuffer()
	:m_RendererID(0), m_Count(0)
{
}

IndexBuffer::IndexBuffer(const void* data, size_t size)
	: m_Count(size/sizeof(unsigned int))
{
	glGenBuffers(1, &m_RendererID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
}

IndexBuffer::~IndexBuffer()
{
	glDeleteBuffers(1, &m_RendererID);

}

void IndexBuffer::Bind() const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
}

void IndexBuffer::Unbind() const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void IndexBuffer::LoadData(const void* data, size_t size)
{
	glGenBuffers(1, &m_RendererID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
	m_Count = size / sizeof(unsigned int);
}
