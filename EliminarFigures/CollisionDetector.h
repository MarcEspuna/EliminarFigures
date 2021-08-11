#pragma once
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <vector>

class CollisionDetector
{
public:

	CollisionDetector(float* quad, std::vector<float> Object, std::vector<unsigned int> Indexes, const unsigned int& count);
	CollisionDetector(float* quad, float* object, unsigned int* indexes, const unsigned int& count);

	unsigned int Refresh(const glm::vec3& quadTranslation);
	unsigned int RefreshObj(const glm::vec3& quadTranslation);
	inline bool GetStatus() const { return CollisionCheck; }
	void UpdateVerticies(glm::mat4 u_Model, const unsigned int& size);
	void UpdateVerticiesObj(glm::mat4 u_Model);

private:

	float* m_Quad;
	float* m_Object;
	unsigned int* m_Indexes;
	unsigned int m_Count;
	std::vector<glm::vec2> updatedVertices;
	std::vector<float> m_ObjectV;
	std::vector<unsigned int> m_IndexesV;

	bool CollisionCheck;

	float sign(glm::vec2 p1, glm::vec2 p2, glm::vec2 p3);
	bool PointInTriangle(glm::vec2 pt, glm::vec2 v1, glm::vec2 v2, glm::vec2 v3);

};

