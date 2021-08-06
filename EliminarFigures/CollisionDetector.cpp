#include "CollisionDetector.h"
#include <iostream>

CollisionDetector::CollisionDetector(float* quad, float* object, unsigned int* indexes, const unsigned int& count)	//count = values in the index buffer
	: m_Quad(quad), m_Object(object), m_Indexes(indexes) ,m_Count(count), CollisionCheck(false)
{



}

unsigned int CollisionDetector::Refresh(const glm::vec3& quadTranslation, const glm::vec3& objectTranslation)
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
			if (PointInTriangle(glm::vec2(Quad[i], Quad[i + 1]), glm::vec2(m_Object[m_Indexes[e] * 2] + objectTranslation.x, m_Object[m_Indexes[e] * 2 + 1] + objectTranslation.y),
				glm::vec2(m_Object[m_Indexes[e + 1] * 2] + objectTranslation.x, m_Object[m_Indexes[e + 1] * 2 + 1] + objectTranslation.y),
				glm::vec2(m_Object[m_Indexes[e + 2] * 2] + objectTranslation.x, m_Object[m_Indexes[e + 2] * 2 + 1] + objectTranslation.y)))
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