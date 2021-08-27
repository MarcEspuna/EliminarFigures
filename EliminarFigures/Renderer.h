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


class Timer
{
public:
	Timer(float& deltaTime)
	: startingTime((float)glfwGetTime()), deltaTime(deltaTime){

	}
	~Timer() {
		deltaTime = ((float)glfwGetTime() - startingTime)*20;
	}


private:
	float startingTime;
	float& deltaTime;

};

struct ImguiVariables
{
	unsigned int CachedObjects;
	unsigned int RemainingObjects;
	float TimeLeft;
};
