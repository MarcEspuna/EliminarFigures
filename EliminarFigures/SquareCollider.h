#pragma once
#include "glm/glm.hpp"
#include "obj_loader/tiny_obj_loader.h"
#include "SquareObject.h"
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
	~SquareCollider();
	void checkCollision(const SquareCollider& other);
	float checkDistance(const SquareCollider& other) const;
	glm::vec2 checkDistanceVec2(const SquareCollider& other) const;

	void moveUp(const float& deltaTime, const float& sensitivity);
	void moveDown(const float& deltaTime, const float& sensitivity);
	void moveRight(const float& deltaTime, const float& sensitivity);
	void moveLeft(const float& deltaTime, const float& sensitivity);
	void moveTo(const glm::vec3& position);
	void move(const glm::vec3& direction);
	void Load(const glm::mat4& u_Model, const std::vector<float>& positions);
	void Update(const glm::mat4& u_Model);

	inline bool thereIsCollision() { return m_Collided; }
	const float* getShapes();
	const unsigned int* getIndex();

private:

	std::vector<float> m_Positions;
	SquareCoords m_SquareCoords;
	bool m_Collided;

	void updateObjectPositions(std::vector<float>& positions, const glm::mat4& u_Model);
	void updateCollider(const std::vector<float>& positions);

	float shapes[16] = {
		-300.0f,  200.0f, -1.0f,  1.0f,				//0			  0  ____  1
		 300.0f,  200.0f,  1.0f,  1.0f,				//1				|    |
		-300.0f, -200.0f, -1.0f, -1.0f,				//2			    |____|
		 300.0f, -200.0f,  1.0f, -1.0f				//3			  2	       3
	};

	const unsigned int index[6] =
	{
		0, 1, 3,
		2, 3, 0
	};

};
