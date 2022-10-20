#include "Entity.h"

Entity::Entity()
	: u_Color(glm::vec4(1.0f,1.0f,1.0f,1.0f)), shader("res/Basic.shader")
{

}

Entity::Entity(glm::vec4 color, const char* shaderPath)
	: u_Color(color), shader(shaderPath)
{

}

Entity::Entity(glm::vec4 color, const char* shaderPath, const tinyobj::shape_t& shape, const glm::mat4& u_Model)
	: u_Color(color), shader(shaderPath)
{

}

Entity::Entity(glm::vec4 color, glm::vec3 scale)
	: u_Color(color), shader("res/Basic.shader")
{

}

Entity::Entity(glm::vec4 color)
	: u_Color(color), shader("res/Basic.shader")
{

}

void Entity::Bind() const
{
	vao.Bind();
	ibo.Bind();
	shader.Bind();
}
