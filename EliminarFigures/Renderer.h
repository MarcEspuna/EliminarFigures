#pragma once
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_opengl3.h"
#include "imgui/imgui_impl_glfw.h"

class Renderer 
{
public:
	Renderer() {}

	void Draw(const VertexArray& vao, const IndexBuffer& ibo, const Shader& shader) const 
	{
		vao.Bind();
		ibo.Bind();
		shader.Bind();
		glDrawElements(GL_TRIANGLES, (GLsizei)ibo.GetCount(), GL_UNSIGNED_INT, 0);
	}

	void DrawQ(const VertexArray& vao, const IndexBuffer& ibo, const Shader& shader) const
	{
		vao.Bind();
		ibo.Bind();
		shader.Bind();
		glDrawElements(GL_QUADS, (GLsizei)ibo.GetCount(), GL_UNSIGNED_INT, 0);
	}

	void Clear()
	{
		glClear(GL_COLOR_BUFFER_BIT);
	}

private:




};