#pragma once
#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

class IndexBuffer
{
public:

	IndexBuffer();
	IndexBuffer(const void* data, size_t size);
	~IndexBuffer();

	void Bind() const;
	void Unbind() const;

	template<typename T>
	void LoadData(const std::vector<T>& data)
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, data.size() * sizeof(T), &data[0], GL_STATIC_DRAW);
		m_Count = data.size();
	}

	void LoadData(const void* data, size_t size);

	inline size_t GetCount() const { return m_Count; }

private:
	unsigned int m_RendererID;
	size_t m_Count;
	

};

