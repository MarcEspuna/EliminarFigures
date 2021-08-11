#include "CollisionDetector.h"
#include <iostream>

CollisionDetector::CollisionDetector(float* quad, float* object, unsigned int* indexes, const unsigned int& count)	//count = values in the index buffer
	: m_Quad(quad), m_Object(object), m_Indexes(indexes) ,m_Count(count), CollisionCheck(false)
{



}

CollisionDetector::CollisionDetector(float* quad, std::vector<float> Object, std::vector<unsigned int> Indexes, const unsigned int& count)
	: m_Quad(quad), m_ObjectV(Object), m_IndexesV(Indexes), m_Count(count), CollisionCheck(false), m_Object(nullptr), m_Indexes(nullptr)
{



}

unsigned int CollisionDetector::Refresh(const glm::vec3& quadTranslation)
{
	float Quad[8];
	Quad[0] = m_Quad[0] + quadTranslation.x;
	Quad[1] = m_Quad[1] + quadTranslation.y;
	Quad[2] = m_Quad[2] + quadTranslation.x;
	Quad[3] = m_Quad[3] + quadTranslation.y;
	Quad[4] = m_Quad[4] + quadTranslation.x;
	Quad[5] = m_Quad[5] + quadTranslation.y;
	Quad[6] = m_Quad[6] + quadTranslation.x;
	Quad[7] = m_Quad[7] + quadTranslation.y;

	for (size_t i = 0; i < 7; i++)							//Each quad point 
	{
		for (size_t e = 0; e < m_Count; )				//Each triangle in the object
		{
			unsigned int index1 = m_Indexes[e];
			unsigned int index2 = m_Indexes[e+1];
			unsigned int index3 = m_Indexes[e+2];

			glm::vec2 vertex1 = updatedVertices[index1];
			glm::vec2 vertex2 = updatedVertices[index2];
			glm::vec2 vertex3 = updatedVertices[index3];

			if (PointInTriangle(glm::vec2(Quad[i], Quad[i + 1]), vertex1, vertex2, vertex3))
			{
				CollisionCheck = true;
				return 1;
			}
			e += 3;
		}
		i++;
	}

	CollisionCheck = false;
	return 0;
}

unsigned int CollisionDetector::RefreshObj(const glm::vec3& quadTranslation)
{
	float Quad[8];
	Quad[0] = m_Quad[0] + quadTranslation.x;
	Quad[1] = m_Quad[1] + quadTranslation.y;
	Quad[2] = m_Quad[2] + quadTranslation.x;
	Quad[3] = m_Quad[3] + quadTranslation.y;
	Quad[4] = m_Quad[4] + quadTranslation.x;
	Quad[5] = m_Quad[5] + quadTranslation.y;
	Quad[6] = m_Quad[6] + quadTranslation.x;
	Quad[7] = m_Quad[7] + quadTranslation.y;

	for (size_t i = 0; i < 7; i++)							//Each quad point 
	{
		for (size_t e = 0; e < m_Count; )				//Each triangle in the object
		{
			unsigned int index1 = m_IndexesV[e];
			unsigned int index2 = m_IndexesV[e + 1];
			unsigned int index3 = m_IndexesV[e + 2];

			glm::vec2 vertex1 = updatedVertices[index1];
			glm::vec2 vertex2 = updatedVertices[index2];
			glm::vec2 vertex3 = updatedVertices[index3];

			if (PointInTriangle(glm::vec2(Quad[i], Quad[i + 1]), vertex1, vertex2, vertex3))
			{
				CollisionCheck = true;
				return 1;
			}
			e += 3;
		}
		i++;
	}

	CollisionCheck = false;
	return 0;
}

float CollisionDetector::sign(glm::vec2 p1, glm::vec2 p2, glm::vec2 p3)
{
	return (p1.x - p3.x) * (p2.y - p3.y) - (p2.x - p3.x) * (p1.y - p3.y);
}

bool CollisionDetector::PointInTriangle(glm::vec2 pt, glm::vec2 v1, glm::vec2 v2, glm::vec2 v3)
{
	float d1, d2, d3;
	bool has_neg, has_pos;

	d1 = sign(pt, v1, v2);
	d2 = sign(pt, v2, v3);
	d3 = sign(pt, v3, v1);

	has_neg = (d1 < 0) || (d2 < 0) || (d3 < 0);
	has_pos = (d1 > 0) || (d2 > 0) || (d3 > 0);

	return !(has_neg && has_pos);
}

void CollisionDetector::UpdateVerticiesObj(glm::mat4 u_Model)
{
	updatedVertices.erase(updatedVertices.begin(), updatedVertices.end());
	updatedVertices.reserve(m_ObjectV .size()/2);
	for (unsigned int i = 0; i < m_ObjectV.size()-1; i++)
	{
		// first convert each vertex position to a vec4 to allow for matrix multiplication
		glm::vec4 pos(m_ObjectV[i], m_ObjectV[i+1], 0.0f, 1.0f);
		// multiply vec4 vertex by the model matrix (mul. order is important)
		glm::vec4 modelPos = u_Model * pos;
		// convert back to a vec3
		glm::vec3 newPos(modelPos[0], modelPos[1], modelPos[2]);
		// store it in a new vector
		updatedVertices.emplace_back(newPos.x,newPos.y);
		i++;
	}
}

void CollisionDetector::UpdateVerticies(glm::mat4 u_Model, const unsigned int& size)
{
	updatedVertices.erase(updatedVertices.begin(), updatedVertices.end());
	updatedVertices.reserve(size / 2);
	for (unsigned int i = 0; i < size - 1; i++)
	{
		// first convert each vertex position to a vec4 to allow for matrix multiplication
		glm::vec4 pos(m_Object[i], m_Object[i + 1], 0.0f, 1.0f);
		// multiply vec4 vertex by the model matrix (mul. order is important)
		glm::vec4 modelPos = u_Model * pos;
		// convert back to a vec3
		glm::vec3 newPos(modelPos[0], modelPos[1], modelPos[2]);
		// store it in a new vector
		updatedVertices.emplace_back(newPos.x, newPos.y);
		i++;
	}
}