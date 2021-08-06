#pragma once
#include "VertexBuffer.h"
#include "VertexArrayLayout.h"

class VertexArray
{
public:
	VertexArray();

	~VertexArray();

	void Bind() const;
	void Unbind() const;

	void AddBuffer(const VertexBuffer& vbo, const VertexArrayLayout& layout);


private:
	unsigned int m_RendererID;


};

