#include "Object.h"
#include "ObjParser.h"
#include "VertexArrayLayout.h"
#include <future>


Object::Object()
	: u_Color(1.0f, 1.0f, 1.0f, 1.0f), collision(this), shader("res/Basic.shader"){}

Object::Object(glm::vec4 color)
	: u_Color(color), collision(this), shader("res/Basic.shader"){}

Object::Object(float scale)
	: u_Color(1.0f, 1.0f, 1.0f, 1.0f), collision(this), shader("res/Basic.shader"){}

Object::Object(glm::vec4 color, float scale)
	: u_Color(color), collision(this), shader("res/Basic.shader"){}

Object::Object(glm::vec4 color, float scale, const char* shaderPath)
	: u_Color(color), collision(this), shader(shaderPath){}

Object::Object(glm::vec4 color, glm::vec3 scale)
	: u_Color(color), collision(this), shader("res/Basic.shader") {}


void Object::TrackCollisionWith(Object* otherObject)
{
	collision.SetOtherObject(otherObject);

}

void Object::New(const glm::mat4& u_NewModel, const glm::vec3& movement)
{
	vec_Model.push_back(u_NewModel);
	u_MVP.push_back(glm::mat4(1.0f));
	movementValues.push_back(movement);
}

void Object::Bind() const
{
	shader.Bind();
	vao.Bind();
	vbo.Bind();
	ibo.Bind();
}

std::vector<unsigned int>& Object::GetIndex()
{
	std::vector<unsigned int> random;
	return random;
}

std::vector<float>& Object::GetVertex()
{
	std::vector<float> random;
	return random;
}


