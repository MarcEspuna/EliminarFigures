#include "Object.h"
#include "ObjParser.h"
#include "VertexArrayLayout.h"
#include <future>


Object::Object()
	: Entity(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), "res/Basic.shader"), collision(this), selected(false), hit(false), activeCollider(false) {}

Object::Object(glm::vec4 color)
	: Entity(color, "res/Basic.shader"), collision(this), selected(false), hit(false), activeCollider(false) {}

Object::Object(float scale)
	: Entity(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), "res/Basic.shader"), collision(this), selected(false), hit(false), activeCollider(false) {}

Object::Object(glm::vec4 color, const char* shaderPath)
	: Entity(color, shaderPath), collision(this), selected(false), hit(false), activeCollider(false) {}

Object::Object(glm::vec4 color, const char* shaderPath, const tinyobj::shape_t& shape, const glm::mat4& u_Model)
	: Entity(color, shaderPath), collision(this), selected(false), hit(false), activeCollider(false), m_SquareCollider(u_Model, shape) {}

Object::Object(glm::vec4 color, glm::vec3 scale)
	: Entity(color, "res/Basic.shader"), collision(this), selected(false), hit(false), activeCollider(false) {}


void Object::TrackCollisionWith(Object* otherObject)
{
	collision.SetOtherObject(otherObject);

}

float Object::UpdateCollisionWith(Object* other)
{
	m_SquareCollider.checkCollision(other->m_SquareCollider);
	return m_SquareCollider.checkDistance(other->m_SquareCollider);
}

void Object::New(const glm::mat4& u_NewModel, const glm::vec3& movement)
{
	vec_Model.push_back(u_NewModel);
	u_MVP.push_back(glm::mat4(1.0f));
	movementValues.push_back(movement);
}

void Object::New(const RandomGenerator& random)
{
	random.Randomize();

}

void Object::moveUP(const float& deltaTime, const float& sensitivity)
{
	//We check if we are inside the boundaries of the screen
	if (!(vec_Model[0][3][1] > 330.0f))
	{
		vec_Model[0] = glm::translate(vec_Model[0], glm::vec3(0.0f, deltaTime * sensitivity, 0.0f));
		m_SquareCollider.moveUp(deltaTime, sensitivity);
	}

}

void Object::moveDown(const float& deltaTime, const float& sensitivity)
{
	if (!(vec_Model[0][3][1] < -330.0f))                                                        //Not allowing the cursor to go outside of the screen
	{
		vec_Model[0] = glm::translate(vec_Model[0], glm::vec3(0.0f, -deltaTime * sensitivity, 0.0f));
		m_SquareCollider.moveDown(deltaTime, sensitivity);
	}

}

void Object::moveRight(const float& deltaTime, const float& sensitivity)
{
	if (!(vec_Model[0][3][0] > 630.0f))
	{
		vec_Model[0] = glm::translate(vec_Model[0], glm::vec3(deltaTime * sensitivity, 0.0f, 0.0f));
		m_SquareCollider.moveRight(deltaTime, sensitivity);
	}

}

void Object::moveLeft(const float& deltaTime, const float& sensitivity)
{
	if (!(vec_Model[0][3][0] < -630.0f))
	{
		vec_Model[0] = glm::translate(vec_Model[0], glm::vec3(-deltaTime * sensitivity, 0.0f, 0.0f));
		m_SquareCollider.moveLeft(deltaTime, sensitivity);
	}
}

void Object::setUniformCollider(size_t objectIndex, const glm::mat4& projection, const glm::mat4& view)
{
	m_CollisionView.setUniform(objectIndex, projection, view);
}


void Object::BindCollider() const
{
	m_CollisionView.Bind();
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


