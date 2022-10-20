#include "AiPlayer.h"

template <typename T> 
static int sgn(T val) {
	return (T(0) < val) - (val < T(0));
}


AiPlayer::AiPlayer()
	: m_Cursor(nullptr), m_GoTo(nullptr)
{
}

void AiPlayer::setCursor(const Object* object)
{
	m_Cursor = object->getCollider();
}

void AiPlayer::goTo(const Object* object)
{
	m_GoTo = object->getCollider();
}

glm::vec2 AiPlayer::getAiInput()
{
	glm::vec2 distance = glm::vec2(0.0f);
	if (m_GoTo && m_Cursor) { distance = m_Cursor->checkDistanceVec2(*m_GoTo); }
	if (std::abs(distance.x) < 5.0f) { distance.x = 0.0f; }
	if (std::abs(distance.y) < 5.0f) { distance.y = 0.0f; }
	return glm::vec2(sgn(distance.x), sgn(distance.y));
}
