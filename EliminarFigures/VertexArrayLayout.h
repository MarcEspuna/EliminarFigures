#pragma once
#include <iostream>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

struct VertexArrayDataLayout
{

	VertexArrayDataLayout(unsigned int type, unsigned int normalized, unsigned int count)
		:type(type), normalized(normalized), count(count) {};

	unsigned int type;
	unsigned int normalized;																//The stride is the amount of bytes needed to jump to another Vertex
	unsigned int count;

	unsigned int GetSize() const
	{
		switch (type)
		{
		case 5126:	return 4;
		case 5125:	return 4;
		case 5121:	return 1;
		default:
			std::cout << "[ERROR]: Layout type not supported" << std::endl;
			return 0;
		}
	}

};



class VertexArrayLayout
{
public:
	VertexArrayLayout() 
		:m_Stride(0)
	{

	}
	~VertexArrayLayout()
	{

	}

	inline unsigned int GetStride() const { return m_Stride; } 

	template<typename T>
	void Push(int count)
	{
		std::cout << "[ERROR]: Type not supported" << std::endl;
		static_assert(false);
	}

	template<>
	void Push<float>(int count)
	{
		m_Layout.emplace_back(GL_FLOAT, GL_FALSE, count);
		m_Stride += m_Layout[m_Layout.size() - 1].GetSize() * count;
	}

	template<>
	void Push<unsigned int>(int count)
	{
		m_Layout.emplace_back(GL_FALSE, GL_UNSIGNED_INT, count);
		m_Stride += m_Layout[m_Layout.size() - 1].GetSize() * count;
	}

	template<>
	void Push<unsigned char>(int count)
	{
		m_Layout.emplace_back(GL_FALSE, GL_UNSIGNED_BYTE, count);
		m_Stride += m_Layout[m_Layout.size() - 1].GetSize() * count;
	}

	const std::vector<VertexArrayDataLayout> GetLayouts() const { return m_Layout; }

private:
	std::vector<VertexArrayDataLayout> m_Layout;
	unsigned int m_Stride;

};

