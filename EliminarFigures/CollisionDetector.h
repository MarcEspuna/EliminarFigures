#pragma once
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"


class CollisionDetector
{
public:

	CollisionDetector(float* quad, float* object, unsigned int* indexes, const unsigned int& count);

	unsigned int Refresh(const glm::vec3& quadTranslation, const glm::vec3& objectTranslation);
	inline bool GetStatus() const { return CollisionCheck; }


private:

	float* m_Quad;
	float* m_Object;
	unsigned int* m_Indexes;
	unsigned int m_Count;

	bool CollisionCheck;

	float sign(glm::vec2 p1, glm::vec2 p2, glm::vec2 p3);
	bool PointInTriangle(glm::vec2 pt, glm::vec2 v1, glm::vec2 v2, glm::vec2 v3);

};

