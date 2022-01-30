#include "Object.h"
#include "ObjParser.h"
#include "VertexArrayLayout.h"
#include <future>


Object::Object(const char* filePath)
	: m_Data(filePath), u_Color(1.0f, 1.0f, 1.0f, 1.0f), collision(this), shader("res/Basic.shader"){}

Object::Object(const char* filePath, glm::vec4 color)
	: m_Data(filePath), u_Color(color), collision(this), shader("res/Basic.shader"){}

Object::Object(const char* filePath, float scale)
	: m_Data(filePath), u_Color(1.0f, 1.0f, 1.0f, 1.0f), collision(this), shader("res/Basic.shader"){}

Object::Object(const char* filePath, glm::vec4 color, float scale)
	: m_Data(filePath), u_Color(color), collision(this), shader("res/Basic.shader"){}

Object::Object(const char* filePath, glm::vec4 color, float scale, const char* shaderPath)
	: m_Data(filePath), u_Color(color), collision(this), shader(shaderPath){}

Object::Object(const char* filePath, glm::vec4 color, glm::vec3 scale)
	: m_Data(filePath), u_Color(color), collision(this), shader("res/Basic.shader") {}


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
	vao.Bind();
	ibo.Bind();
	shader.Bind();
}


