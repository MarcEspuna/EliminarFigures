#pragma once
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <vector>

class Object;

class CollisionDetector
{
public:
	CollisionDetector(Object* mainObject);
	~CollisionDetector();

	void SetOtherObject(Object* otherObject);

	bool GetStatus();
	void End() { CollisionCheck = false; }


private:
	void UpdateVerticies(const std::vector<float>& originalVertex2D, const glm::mat4& u_Model, std::vector<glm::vec2>& vectorToUpdate);

	Object* MainObject;
	Object* OtherObject;

	std::vector<glm::vec2> updatedVerticesObject;
	std::vector<glm::vec2> updatedVerticesQuad;

	bool CollisionCheck;

	float sign(glm::vec2 p1, glm::vec2 p2, glm::vec2 p3);
	bool PointInTriangle(glm::vec2 pt, glm::vec2 v1, glm::vec2 v2, glm::vec2 v3);
	unsigned int Refresh(size_t index);

};

