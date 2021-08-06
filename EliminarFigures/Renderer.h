#pragma once
#include "Shader.h"
#include "VertexArray.h"
#include "IndexBuffer.h"


class Renderer 
{
public:
	Renderer() {}

	void Draw(const VertexArray& vao, const IndexBuffer& ibo, const Shader& shader) const 
	{
		vao.Bind();
		ibo.Bind();
		shader.Bind();
		glDrawElements(GL_TRIANGLES, ibo.GetCount(), GL_UNSIGNED_INT, 0);
	}

	void Clear()
	{
		glClear(GL_COLOR_BUFFER_BIT);
	}

private:




};