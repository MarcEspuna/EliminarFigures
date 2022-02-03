#pragma once
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <unordered_map>
struct Slot
{
	//18 slots in total
	glm::mat4 operator[](unsigned int index)
	{
		unsigned int column = index / 6 + 1;
		unsigned int row = index % 6 + 1;
		float xOrigin = -640.0f;
		float yOrigin = -480.0f;
		xOrigin += xSpacing * row;
		yOrigin += ySpacing * column;
		availableSlots[index] = false;
		return glm::translate(glm::mat4(1.0f), glm::vec3(xOrigin, yOrigin, 0.0f));
	}
	bool isAvailable(unsigned int index)
	{
		return availableSlots[index];
	}
	void clear()
	{
		availableSlots = {
			{0,true}, {1,true},{2,true},{3,true},{4,true},{5,true},{6,true},{7,true},{8,true},{9,true},{10,true},
			{11,true},{12,true},{13,true},{14,true},{15,true},{16,true},{17,true}
		};
	}

private:
	const float maxX = 640.0f;
	const float maxY = 480.0f;
	const float xSpacing = (maxX*2.0f)/7.0f;
	const float ySpacing = (maxY*2.0f)/4.0f;
	std::unordered_map<unsigned int, bool> availableSlots =
	{
		{0,true}, {1,true},{2,true},{3,true},{4,true},{5,true},{6,true},{7,true},{8,true},{9,true},{10,true},
		{11,true},{12,true},{13,true},{14,true},{15,true},{16,true},{17,true}
	};

};

