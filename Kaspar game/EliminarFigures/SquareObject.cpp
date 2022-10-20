#include "SquareObject.h"

SquareObject::SquareObject()
	: Entity(glm::vec4(0.0f, 1.0f, 0.0f, 1.0f), "res/Square.shader")
{

}

SquareObject::SquareObject(const float* shape, const unsigned int* indexes)
	: Entity(glm::vec4(0.0f, 1.0f, 0.0f, 1.0f), "res/Square.shader")
{
	VertexArrayLayout layout;
	vbo.LoadData(shape, sizeof(float) * 16);
	ibo.LoadData(indexes, sizeof(unsigned int) * 6);
	layout.Push<float>(2);
	layout.Push<float>(2);
	vao.AddBuffer(vbo, layout);
}

void SquareObject::Load(const float* shape, const unsigned int* indexes)
{
	VertexArrayLayout layout;
	vbo.LoadData(shape, sizeof(float) * 16);
	ibo.LoadData(indexes, sizeof(unsigned int) * 6);
	layout.Push<float>(2);
	layout.Push<float>(2);
	vao.AddBuffer(vbo, layout);
}


void SquareObject::setUniform(size_t objectIndex, const glm::mat4& projection, const glm::mat4& view)
{
	shader.Bind();
	shader.SetUniform4Mat("u_Model", glm::mat4(1.0f));
	shader.SetUniform4Mat("u_View", view);
	shader.SetUniform4Mat("u_Proj", projection);
	shader.SetUniform4f("u_Color", GetColor());
	shader.SetUniform1f("thickness", 0.98f);
}
