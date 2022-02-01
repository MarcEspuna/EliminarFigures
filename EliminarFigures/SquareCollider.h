#pragma once
#include "glm/glm.hpp"
#include "obj_loader/tiny_obj_loader.h"
struct SquareCoords
{
	//We need x and xSize
	//we need y and ySize
	float x;
	float xSize;
	float y;
	float ySize;
};

class SquareCollider
{
public:
	SquareCollider();
	SquareCollider(const glm::mat4& u_Model, const tinyobj::shape_t& shape);
	SquareCollider(const glm::mat4& u_Model, const std::vector<float>& positions);
	void checkCollision(const SquareCollider& other);

	void moveUp(const float& deltaTime, const float& sensitivity);
	void moveDown(const float& deltaTime, const float& sensitivity);
	void moveRight(const float& deltaTime, const float& sensitivity);
	void moveLeft(const float& deltaTime, const float& sensitivity);
	
	inline bool thereIsCollision() { return m_Collided; }

private:

	SquareCoords m_SquareColider;
	bool m_Collided;

	const std::vector<float> updateObjectPositions(const std::vector<float>& positions, const glm::mat4& u_Model);
	void updateCollider(const std::vector<float>& positions);

};
