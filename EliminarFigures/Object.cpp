#include "DataLink.h"
#include "Object.h"
#include "ObjParser.h"
#include "VertexArrayLayout.h"
#include "glm/gtc/matrix_transform.hpp"
#include <unordered_map>
#include <future>
#include <mutex>

static std::mutex s_MoveMutex;

unsigned int Object::objectCounter;

Object::Object()
	: Entity(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), "res/Basic.shader"), collision(this), selected(false), hit(false), activeCollider(false), rotationSpeed(50) {
	objectCounter++; id = objectCounter;
}

Object::Object(glm::vec4 color)
	: Entity(color, "res/Basic.shader"), collision(this), selected(false), hit(false), activeCollider(false), rotationSpeed(50) {
	objectCounter++; id = objectCounter;
}

Object::Object(float scale)
	: Entity(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), "res/Basic.shader"), collision(this), selected(false), hit(false), activeCollider(false), rotationSpeed(50) {
	objectCounter++; id = objectCounter;
}

Object::Object(glm::vec4 color, const char* shaderPath)
	: Entity(color, shaderPath), collision(this), selected(false), hit(false), activeCollider(false), rotationSpeed(50) {
	objectCounter++; id = objectCounter;
}

Object::Object(glm::vec4 color, const char* shaderPath, const tinyobj::shape_t& shape, const glm::mat4& u_Model, const float& scale)
	: Entity(color, shaderPath), collision(this), selected(false), hit(false), activeCollider(false), m_SquareCollider(u_Model, shape), m_DefaultScale(scale), rotationSpeed(50) {
	objectCounter++; id = objectCounter;
}

Object::Object(glm::vec4 color, glm::vec3 scale)
	: Entity(color, "res/Basic.shader"), collision(this), selected(false), hit(false), activeCollider(false), rotationSpeed(50) {
	objectCounter++; id = objectCounter;
}


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

void Object::New(const RandomGenerator& random, Slot& slot)
{
	random.Randomize();
	unsigned int rowIdx = std::abs(random.GetValue(0, slot.getSize()));			// 5 rows  range is from [0-4]
	unsigned int colIdx = std::abs(random.GetValue(0, slot.getSizeCol()));		// 7 columns range is from [0-6]

	glm::mat4 model = slot.getModel(rowIdx, colIdx);							// Retreave the model matrix of the randomly generated slot
	
	vec_Model.push_back(glm::scale(model, glm::vec3(m_DefaultScale)));			
	objectMovement.reset(model, 100.0f);
	m_SquareCollider.Update(vec_Model[0]);
	selected = false;
	activeCollider = false;
	hit = false;

}

void Object::moveUP(const float& deltaTime, const float& sensitivity)
{
	//We check if we are inside the boundaries of the screen
	std::lock_guard<std::mutex> lock(s_MoveMutex);
	if (!(vec_Model[0][3][1] > 330.0f))
	{
		vec_Model[0] = glm::translate(vec_Model[0], glm::vec3(0.0f, deltaTime * sensitivity, 0.0f));
		m_SquareCollider.moveUp(deltaTime, sensitivity);
	}

}

void Object::moveDown(const float& deltaTime, const float& sensitivity)
{
	std::lock_guard<std::mutex> lock(s_MoveMutex);
	if (!(vec_Model[0][3][1] < -330.0f))                                                        //Not allowing the cursor to go outside of the screen
	{
		vec_Model[0] = glm::translate(vec_Model[0], glm::vec3(0.0f, -deltaTime * sensitivity, 0.0f));
		m_SquareCollider.moveDown(deltaTime, sensitivity);
	}

}

void Object::moveRight(const float& deltaTime, const float& sensitivity)
{
	std::lock_guard<std::mutex> lock(s_MoveMutex);
	if (!(vec_Model[0][3][0] > 630.0f))
	{
		vec_Model[0] = glm::translate(vec_Model[0], glm::vec3(deltaTime * sensitivity, 0.0f, 0.0f));
		m_SquareCollider.moveRight(deltaTime, sensitivity);
	}

}

void Object::moveLeft(const float& deltaTime, const float& sensitivity)
{
	std::lock_guard<std::mutex> lock(s_MoveMutex);
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

void Object::init()
{
	objectCounter = 0;
}



