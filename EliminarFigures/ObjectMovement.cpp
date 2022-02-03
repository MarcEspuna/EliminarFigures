#include "ObjectMovement.h"

ObjectMovement::ObjectMovement()
	:m_Rotation(0), m_Scale(1.0f), m_Translation(glm::vec3(0.0f,0.0f,0.0f))
{

}


glm::mat4 ObjectMovement::wsMove(const glm::vec3& translatingVector, SquareCollider& collider)
{
	m_Translation += translatingVector;
	collider.move(translatingVector);
	return calculateModelMatrix();
}

glm::mat4 ObjectMovement::wsGoTo(const glm::vec3& position, SquareCollider& collider)
{
	m_Translation = position;
	collider.moveTo(position);
	return calculateModelMatrix();
}

void ObjectMovement::rotateY(const float& angle, glm::mat4& model, SquareCollider& collider)
{
	m_Rotation += angle;
	model = glm::rotate(model, angle, glm::vec3(0.0f, 1.0f, 0.0f));
	collider.Update(model);
}

void ObjectMovement::scale(const float& scale, glm::mat4& model, SquareCollider& collider)
{
	m_Scale = m_Scale * scale;
	model = glm::scale(model, glm::vec3(scale, scale, scale));
	collider.Update(model);
}

glm::mat4 ObjectMovement::calculateModelMatrix()
{
	glm::mat4 id = glm::translate(glm::mat4(1.0f), m_Translation);
	id = glm::rotate(id, m_Rotation, glm::vec3(0.0f, 1.0f, 0.0f));
	id = glm::scale(id, glm::vec3(m_Scale, m_Scale, m_Scale));
	return id;
}
