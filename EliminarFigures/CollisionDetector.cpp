#include "CollisionDetector.h"
#include "Object.h"
#include <iostream>

CollisionDetector::CollisionDetector(Object* mainObject)
	: MainObject(mainObject), OtherObject(nullptr), CollisionCheck(false)
{

}

CollisionDetector::~CollisionDetector()
{
}

void CollisionDetector::SetOtherObject(Object* otherObject)
{
	OtherObject = otherObject;
}

bool CollisionDetector::GetStatus()
{
	for (size_t i = 0; i < MainObject->GetModels().size(); i++)
	{
		if (Refresh(i))
			break;
	}
	return CollisionCheck;
}




unsigned int CollisionDetector::Refresh(size_t index)
{
	if (OtherObject)
	{
		const glm::mat4& OtherModel = OtherObject->GetModels()[0];
		const glm::mat4& MainModel = MainObject->GetModels()[index];
		const std::vector<float>& OtherVertex = OtherObject->GetVertex();
		const std::vector<float>& MainVertex = MainObject->GetVertex();

		UpdateVerticies(OtherVertex, OtherModel, updatedVerticesQuad);
		UpdateVerticies(MainVertex, MainModel, updatedVerticesObject);

		for (const auto& updatedVertexQuad : updatedVerticesQuad)							//Each quad point 
		{
			for (size_t e = 0; e < MainObject->GetIndexSize(); )				//Each triangle in the object
			{
				unsigned int& index1 = MainObject->GetIndex()[e];
				unsigned int& index2 = MainObject->GetIndex()[e + 1];
				unsigned int& index3 = MainObject->GetIndex()[e + 2];

				glm::vec2& vertex1 = updatedVerticesObject[index1];
				glm::vec2& vertex2 = updatedVerticesObject[index2];
				glm::vec2& vertex3 = updatedVerticesObject[index3];

				if (PointInTriangle(updatedVertexQuad, vertex1, vertex2, vertex3))
				{
					CollisionCheck = true;
					return 1;
				}
				e += 3;
			}
		}
		CollisionCheck = false;
		return 0;
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

void CollisionDetector::UpdateVerticies(const std::vector<float>& originalVertex2D, const glm::mat4& u_Model, std::vector<glm::vec2>& vectorToUpdate)
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