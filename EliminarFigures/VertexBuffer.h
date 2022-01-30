#pragma once
#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

class VertexBuffer
{
public:
	VertexBuffer();
	VertexBuffer(const void* data, size_t size);
	~VertexBuffer();

	void Bind() const;
	void Unbind() const;

	template<typename T>
	void LoadData(const std::vector<T>& data)
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
		glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(T), &data[0], GL_STATIC_DRAW);
	}

	void LoadData(const void* data, size_t size);


private:
	unsigned int m_RendererID;
	const void* data;

};

