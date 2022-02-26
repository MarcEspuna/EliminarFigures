#pragma once
#include "glm/glm.hpp"
#include "Object.h"

class AiPlayer
{
public:
	AiPlayer();

	void setCursor(const Object* object);
	void goTo(const Object* object);
	glm::vec2 getAiInput();


private:
	const SquareCollider* m_Cursor;
	const SquareCollider* m_GoTo;
};

