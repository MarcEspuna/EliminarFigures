#include "SquareCollider.h"
#include <iostream>
#include <vector>
#include <cmath>

SquareCollider::SquareCollider()
	: m_SquareCoords({0.001f, 1.0f, 0.001f,1.0f }), m_Collided(false)
{

}

SquareCollider::SquareCollider(const glm::mat4& u_Model, const tinyobj::shape_t& shape)
	: m_SquareCoords({ 0.001f, 1.0f, 0.001f, 1.0f }), m_Collided(false), m_Positions(shape.mesh.positions)
{
	std::vector<float> updatedPositions;
	updateObjectPositions(updatedPositions, u_Model);
	updateCollider(updatedPositions);
}

SquareCollider::SquareCollider(const glm::mat4& u_Model, const std::vector<float>& positions)
	: m_SquareCoords({0.0f, 0.0f, 0.0f, 0.0f}), m_Collided(false), m_Positions(positions)
{
	std::vector<float> updatedPositions;
	updateObjectPositions(updatedPositions, u_Model);
	updateCollider(updatedPositions);
}

void SquareCollider::Load(const glm::mat4& u_Model, const std::vector<float>& positions)
{
	m_Positions = positions;
	std::vector<float> updatedPositions;
	updateObjectPositions(updatedPositions, u_Model);
	updateCollider(updatedPositions);
}

void SquareCollider::Update(const glm::mat4& u_Model)
{
	std::vector<float> currentPositions;
	updateObjectPositions(currentPositions, u_Model);
	updateCollider(currentPositions);
}

SquareCollider::~SquareCollider()
{

}

void SquareCollider::moveUp(const float& deltaTime, const float& sensitivity)
{
	m_SquareCoords.y += deltaTime * sensitivity;
}

void SquareCollider::moveDown(const float& deltaTime, const float& sensitivity)
{
	m_SquareCoords.y -= deltaTime * sensitivity;
}

void SquareCollider::moveRight(const float& deltaTime, const float& sensitivity)
{
	m_SquareCoords.x += deltaTime * sensitivity;
}

void SquareCollider::moveLeft(const float& deltaTime, const float& sensitivity)
{
	m_SquareCoords.x -= deltaTime * sensitivity;
}

void SquareCollider::moveTo(const glm::vec3& position)
{
	m_SquareCoords.x = position.x - m_SquareCoords.xSize / 2.0f;
	m_SquareCoords.y = position.y - m_SquareCoords.ySize / 2.0f;
}

void SquareCollider::move(const glm::vec3& direction)
{
	m_SquareCoords.x += direction.x;
	m_SquareCoords.y += direction.y;

}

const float* SquareCollider::getShapes()
{
	shapes[0] = m_SquareCoords.x;
	shapes[1] = m_SquareCoords.y + m_SquareCoords.ySize;
	shapes[4] = m_SquareCoords.x + m_SquareCoords.xSize;
	shapes[5] = m_SquareCoords.y + m_SquareCoords.ySize;
	shapes[8] = m_SquareCoords.x;
	shapes[9] = m_SquareCoords.y;
	shapes[12] = m_SquareCoords.x + m_SquareCoords.xSize;
	shapes[13] = m_SquareCoords.y;
	return &shapes[0];
}

const unsigned int* SquareCollider::getIndex()
{
	return &index[0];
}

void SquareCollider::checkCollision(const SquareCollider& other)
{
	// collision x-axis?
	bool collisionX = m_SquareCoords.x + m_SquareCoords.xSize >= other.m_SquareCoords.x &&
		other.m_SquareCoords.x + other.m_SquareCoords.xSize >= m_SquareCoords.x;
	// collision y-axis?
	bool collisionY = m_SquareCoords.y + m_SquareCoords.ySize >= other.m_SquareCoords.y &&
		other.m_SquareCoords.y + other.m_SquareCoords.ySize >= m_SquareCoords.y;
	// collision only if on both axes
	m_Collided = collisionX && collisionY;
}

float SquareCollider::checkDistance(const SquareCollider& other) const
{
	float distanceX = std::abs(m_SquareCoords.x + m_SquareCoords.xSize/2.0f - other.m_SquareCoords.x - other.m_SquareCoords.xSize/2.0f);
	float distanceY = std::abs(m_SquareCoords.y + m_SquareCoords.ySize/2.0f - other.m_SquareCoords.y - other.m_SquareCoords.ySize/2.0f);
	return std::sqrt(std::pow(distanceX,2) + std::pow(distanceY,2));
}

glm::vec2 SquareCollider::checkDistanceVec2(const SquareCollider& other) const
{
	float distanceX = other.m_SquareCoords.x + other.m_SquareCoords.xSize / 2.0f - m_SquareCoords.x - m_SquareCoords.xSize / 2.0f;
	float distanceY = other.m_SquareCoords.y + other.m_SquareCoords.ySize / 2.0f - m_SquareCoords.y - m_SquareCoords.ySize / 2.0f;
	return glm::vec2(distanceX, distanceY);
}

void SquareCollider::updateObjectPositions(std::vector<float>& positions, const glm::mat4& u_Model)
{
	positions.clear();
	for (int i = 0; i < m_Positions.size()-2; i+=3)
	{
		glm::vec4 currentPosition = { m_Positions[i], m_Positions[i + 1], m_Positions[i+2], 1.0f};
		glm::vec4 newPosition = u_Model * currentPosition;
		positions.push_back(newPosition.x);
		positions.push_back(newPosition.y);
		positions.push_back(0.0f);
	}
}


void SquareCollider::updateCollider(const std::vector<float>& positions)
{
	float maxX = -9999999.0f;
	float minX = 9999999.0f;
	float maxY = -9999999.0f;
	float minY = 9999999.0f;
	for (int i = 0; i < positions.size()-2; i += 3)
	{
		float currentX = positions[i];
		float currentY = positions[i + 1];
		if (currentX != 0.0f && currentY != 0.0f)
		{
			if (currentX > maxX) { maxX = currentX; }
			if (currentX < minX) { minX = currentX; }
			if (currentY > maxY) { maxY = currentY; }
			if (currentY < minY) { minY = currentY; }
		}
	}
	m_SquareCoords.x = minX;
	m_SquareCoords.xSize = maxX - minX;
	m_SquareCoords.y = minY;
	m_SquareCoords.ySize = maxY - minY;

}
