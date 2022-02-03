#pragma once
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

static struct Slot
{
	//18 slots in total
	glm::mat4 operator[](size_t index)
	{
		unsigned int column = index / 6 + 1;
		unsigned int row = index % 6 + 1;
		float xOrigin = -640.0f;
		float yOrigin = -480.0f;
		xOrigin += xSpacing * row;
		yOrigin += ySpacing * column;
		return glm::translate(glm::mat4(1.0f), glm::vec3(xOrigin, yOrigin, 0.0f));
	}

private:
	const float maxX = 640.0f;
	const float maxY = 480.0f;
	const float xSpacing = (maxX*2.0f)/7.0f;
	const float ySpacing = (maxY*2.0f)/4.0f;


};

