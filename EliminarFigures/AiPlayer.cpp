#include "AiPlayer.h"
#include <algorithm>

std::pair<int, int> AiPlayer::objectSelector()
{
    return std::pair<int, int>();
}

AiPlayer::AiPlayer(const std::vector<Object*>& objects, int* playerInput, Object& centerQuad)
	: objects(objects), otherPlayerInput(playerInput), centerQuad(centerQuad)
{
	for (const auto& vertex : updatedVerticesQuad)
	{
		std::cout << "x coord: " << vertex.x << std::endl;
		std::cout << "y coord: " << vertex.y << std::endl;
	}

}

void AiPlayer::makeNewDesicion()
{
	
	

}

std::tuple<int, int, bool> AiPlayer::getOutput()
{


	return std::tuple<int, int, bool>();
}

void AiPlayer::refreshAiPlayerPosition()
{
	glm::mat4 modelMatrix = centerQuad.GetModels()[0];
	std::vector<glm::vec2> updatedVertices;
	UpdateVerticies(centerQuad.GetVertex(), modelMatrix, updatedVertices);
	glm::vec2 maxX = *std::max_element(updatedVertices.begin(),
		updatedVertices.end(),
		[](const glm::vec2& a, const glm::vec2& b) { return a.x < b.x; });

	glm::vec2 maxY = *std::max_element(updatedVertices.begin(),
		updatedVertices.end(),
		[](const glm::vec2& a, const glm::vec2& b) { return a.y < b.y; });

	glm::vec2 minX = *std::min_element(updatedVertices.begin(),
		updatedVertices.end(),
		[](const glm::vec2& a, const glm::vec2& b) { return a.x < b.x; });

	glm::vec2 minY = *std::min_element(updatedVertices.begin(),
		updatedVertices.end(),
		[](const glm::vec2& a, const glm::vec2& b) { return a.y < b.y; });

	glm::vec2 center = { maxX.x + (maxX.x - minX.x) / 2, maxY.y + (maxY.y - minY.y) / 2 };
}

void AiPlayer::refreshObjectsPositions()
{
	for (Object* object : objects)
	{
		for (const glm::mat4& modelMatrix : object->GetModels())
		{
			glm::mat4 modelMatrix = centerQuad.GetModels()[0];
			std::vector<glm::vec2> updatedVertices;
			UpdateVerticies(centerQuad.GetVertex(), modelMatrix, updatedVertices);
			glm::vec2 maxX = *std::max_element(updatedVertices.begin(),
				updatedVertices.end(),
				[](const glm::vec2& a, const glm::vec2& b) { return a.x < b.x; });

			glm::vec2 maxY = *std::max_element(updatedVertices.begin(),
				updatedVertices.end(),
				[](const glm::vec2& a, const glm::vec2& b) { return a.y < b.y; });

			glm::vec2 minX = *std::min_element(updatedVertices.begin(),
				updatedVertices.end(),
				[](const glm::vec2& a, const glm::vec2& b) { return a.x < b.x; });

			glm::vec2 minY = *std::min_element(updatedVertices.begin(),
				updatedVertices.end(),
				[](const glm::vec2& a, const glm::vec2& b) { return a.y < b.y; });

			glm::vec2 center = { maxX.x + (maxX.x - minX.x) / 2, maxY.y + (maxY.y - minY.y) / 2 };

		}

		//We refresh the object positions.
	}

}



void AiPlayer::UpdateVerticies(const std::vector<float>& originalVertex2D, const glm::mat4& u_Model, std::vector<glm::vec2>& vectorToUpdate)
{
	vectorToUpdate.clear();
	vectorToUpdate.reserve(originalVertex2D.size());
	for (unsigned int i = 0; i < originalVertex2D.size() - 1; i++)
	{
		// first convert each vertex position to a vec4 to allow for matrix multiplication
		glm::vec4 pos(originalVertex2D[i], originalVertex2D[i + 1], 0.0f, 1.0f);
		// multiply vec4 vertex by the model matrix (mul. order is important)
		glm::vec4 modelPos = u_Model * pos;
		// convert back to a vec3
		glm::vec3 newPos(modelPos[0], modelPos[1], modelPos[2]);
		// store it in a new vector
		vectorToUpdate.emplace_back(newPos.x, newPos.y);
		i++;

	}

}