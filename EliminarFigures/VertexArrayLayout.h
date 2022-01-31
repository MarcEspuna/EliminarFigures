#pragma once
#include <iostream>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

struct VertexArrayDataLayout
{

	VertexArrayDataLayout(unsigned int type, unsigned int normalized, unsigned int count, std::string u_name)
		:type(type), normalized(normalized), count(count), u_name(u_name) {};

	unsigned int type;
	unsigned int normalized;																//The stride is the amount of bytes needed to jump to another Vertex
	unsigned int count;
	std::string u_name;

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

	const std::string& getUniformName() { return u_name; }

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
	void Push(unsigned int count,const std::string& u_name)
	{
		std::cout << "[ERROR]: Type not supported" << std::endl;
		static_assert(false);
	}

	template<>
	void Push<float>(unsigned int count, const std::string& u_name)
	{
		m_Layout.emplace_back(GL_FLOAT, GL_FALSE, count, u_name);
		m_Stride += m_Layout[m_Layout.size() - 1].GetSize() * count;
	}

	template<>
	void Push<unsigned int>(unsigned int count, const std::string& u_name)
	{
		m_Layout.emplace_back(GL_FALSE, GL_UNSIGNED_INT, count, u_name);
		m_Stride += m_Layout[m_Layout.size() - 1].GetSize() * count;
	}

	template<>
	void Push<unsigned char>(unsigned int count, const std::string& u_name)
	{
		m_Layout.emplace_back(GL_FALSE, GL_UNSIGNED_BYTE, count, u_name);
		m_Stride += m_Layout[m_Layout.size() - 1].GetSize() * count;
	}


	template<typename T>
	void Push(unsigned int count)
	{
		std::cout << "[ERROR]: Type not supported" << std::endl;
		static_assert(false);
	}

	template<>
	void Push<float>(unsigned int count)
	{
		m_Layout.emplace_back(GL_FLOAT, GL_FALSE, count, "");
		m_Stride += m_Layout[m_Layout.size() - 1].GetSize() * count;
	}

	template<>
	void Push<unsigned int>(unsigned int count)
	{
		m_Layout.emplace_back(GL_FALSE, GL_UNSIGNED_INT, count, "");
		m_Stride += m_Layout[m_Layout.size() - 1].GetSize() * count;
	}

	template<>
	void Push<unsigned char>(unsigned int count)
	{
		m_Layout.emplace_back(GL_FALSE, GL_UNSIGNED_BYTE, count, "");
		m_Stride += m_Layout[m_Layout.size() - 1].GetSize() * count;
	}


	const std::vector<VertexArrayDataLayout> GetLayouts() const { return m_Layout; }

private:
	std::vector<VertexArrayDataLayout> m_Layout;
	unsigned int m_Stride;

};

