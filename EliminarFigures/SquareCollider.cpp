#include "SquareCollider.h"
#include <iostream>
#include <vector>

SquareCollider::SquareCollider()
	: m_SquareColider({0.001f, 1.0f, 0.001f,1.0f }), m_Collided(false)
{
	
}

SquareCollider::SquareCollider(const glm::mat4& u_Model, const tinyobj::shape_t& shape)
	: m_SquareColider({ 0.001f, 1.0f, 0.001f, 1.0f }), m_Collided(false)
{

	const auto& positions = shape.mesh.positions;
	std::vector<float> newPositions = updateObjectPositions(positions, u_Model);
	updateCollider(newPositions);
}

SquareCollider::SquareCollider(const glm::mat4& u_Model, const std::vector<float>& positions)
	: m_SquareColider({0.0f, 0.0f, 0.0f, 0.0f}), m_Collided(false)
{
	auto newPositions = updateObjectPositions(positions, u_Model);
	updateCollider(newPositions);
}


void SquareCollider::moveUp(const float& deltaTime, const float& sensitivity)
{
	m_SquareColider.y += deltaTime * sensitivity;
}

void SquareCollider::moveDown(const float& deltaTime, const float& sensitivity)
{
	m_SquareColider.y -= deltaTime * sensitivity;
}

void SquareCollider::moveRight(const float& deltaTime, const float& sensitivity)
{
	m_SquareColider.x += deltaTime * sensitivity;
}

void SquareCollider::moveLeft(const float& deltaTime, const float& sensitivity)
{
	m_SquareColider.x -= deltaTime * sensitivity;
}

void SquareCollider::moveTo(const glm::vec3& position)
{
	m_SquareColider.x = position.x - m_SquareColider.xSize / 2.0f;
	m_SquareColider.y = position.y - m_SquareColider.ySize / 2.0f;
}

void SquareCollider::move(const glm::vec3& direction)
{
	m_SquareColider.x += direction.x;
	m_SquareColider.y += direction.y;

}

void SquareCollider::scale(const float& scale)
{
	float xRange = m_SquareColider.xSize/2.0f;
	float yRange = m_SquareColider.ySize/2.0f;
	m_SquareColider.x += xRange * (1.0f - scale);
	m_SquareColider.y += yRange * (1.0f - scale);
	m_SquareColider.xSize = m_SquareColider.xSize * scale;
	m_SquareColider.ySize = m_SquareColider.ySize * scale;
}

void SquareCollider::checkCollision(const SquareCollider& other)
{
	// collision x-axis?
	bool collisionX = m_SquareColider.x + m_SquareColider.xSize >= other.m_SquareColider.x &&
		other.m_SquareColider.x + other.m_SquareColider.xSize >= m_SquareColider.x;
	// collision y-axis?
	bool collisionY = m_SquareColider.y + m_SquareColider.ySize >= other.m_SquareColider.y &&
		other.m_SquareColider.y + other.m_SquareColider.ySize >= m_SquareColider.y;
	// collision only if on both axes
	m_Collided = collisionX && collisionY;
}


const std::vector<float> SquareCollider::updateObjectPositions(const std::vector<float>& positions, const glm::mat4& u_Model)
{
	//glm::vec3 scalingFactor = glm::vec3(u_Model[0][0], u_Model[1][1], u_Model[2][2]);
	//glm::vec3 translatingFactor = glm::vec3()

	std::vector<float> updatedPositions(positions.size());
	for (int i = 0; i < positions.size()-2; i+=3)
	{
		glm::vec4 currentPosition = { positions[i], positions[i + 1], 1.0f, 1.0f };
		glm::vec4 newPosition = u_Model * currentPosition;
		updatedPositions.push_back(newPosition.x);
		updatedPositions.push_back(newPosition.y);
		updatedPositions.push_back(newPosition.z);
	}
	return updatedPositions;
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
	m_SquareColider.x = minX;
	m_SquareColider.xSize = maxX - minX;
	m_SquareColider.y = minY;
	m_SquareColider.ySize = maxY - minY;

}
