#pragma once
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Object.h"
#include "SquareObject.h"

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

	void Draw(Object& object, const glm::mat4& projection, const glm::mat4& view)
	{
		object.Bind();
		for (size_t i = 0; i < object.size(); i++)
		{
			object.setUniform(i, projection, view);
			glDrawElements(GL_TRIANGLES, (GLsizei)object.getIboCount(), GL_UNSIGNED_INT, 0);
		}
		if (object.isColliderActive())
		{
			glDisable(GL_DEPTH_TEST);
			object.BindCollider();
			object.setUniformCollider(0, projection, view);
			glDrawElements(GL_TRIANGLES, (GLsizei)object.getIboCountCollider(), GL_UNSIGNED_INT, 0);
			glEnable(GL_DEPTH_TEST);
		}
	}


	void DrawQ(const VertexArray& vao, const IndexBuffer& ibo, const Shader& shader) const
	{
		vao.Bind();
		ibo.Bind();
		shader.Bind();
		glDrawElements(GL_QUADS, (GLsizei)ibo.GetCount(), GL_UNSIGNED_INT, 0);
	}

	void ClearColor()
	{
		//glClearColor()
		glClear(GL_COLOR_BUFFER_BIT);
	}

	void ClearDepth()
	{
		glClear(GL_DEPTH_BUFFER_BIT);
	}

	void setBackgroundColor(float red, float green, float blue) {
		glClearColor(red, green, blue, 0.0);
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

