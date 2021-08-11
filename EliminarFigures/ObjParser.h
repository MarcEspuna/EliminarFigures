#pragma once
#include <vector>
#include "glm/glm.hpp"

class ObjParser
{
public:
	ObjParser(const char* filePath);

	inline std::vector<float> GetVertices() 
	{ 
		std::vector<float> vertices;
		for (glm::vec3 vector : temp_vertices)
		{
			vertices.push_back(vector.x);
			vertices.push_back(vector.y);
			vertices.push_back(vector.z);
		}
		return vertices;
	}
	inline std::vector<unsigned int>& GetIndexes() { return vertexIndices; }
	inline std::vector<float>& GetVerticesIn2D() { return vertex2D; }

private:
	FILE* file;
	std::vector<float> vertex2D;

	std::vector< unsigned int > vertexIndices, uvIndices, normalIndices;
	std::vector< glm::vec3 > temp_vertices;
	std::vector< glm::vec2 > temp_uvs;
	std::vector< glm::vec3 > temp_normals;





};

