#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "SquareCollider.h"

class ObjectMovement
{
public:
	ObjectMovement();

	glm::mat4 wsMove(const glm::vec3& translatingVector, SquareCollider& collider);
	glm::mat4 wsGoTo(const glm::vec3& position, SquareCollider& collider);
	void rotateY(const float& angle, glm::mat4& model);
	void scale(const float& scale, glm::mat4& model, SquareCollider& collider);
	inline const float& getScale() const { return m_Scale; }

private:
	
	glm::vec3 m_Translation;
	float m_Scale;
	float m_Rotation;
	glm::mat4 calculateModelMatrix();


};

