#pragma once
#include <vector>
#include "glm/glm.hpp"

class ObjParser
{
public:
	ObjParser(const char* filePath);

	inline std::vector<glm::vec3>& GetVertices() { return temp_vertices; }
	inline std::vector<unsigned int>& GetIndexes() { return vertexIndices; }
	inline std::vector<float>& GetVerticesIn2D() { return vertex2D; }

	void SetVertexScale(glm::vec3 scale);

private:
	FILE* file;
	std::vector<float> vertex2D;
	std::vector< unsigned int > vertexIndices, uvIndices, normalIndices;
	std::vector< glm::vec3 > temp_vertices;
	std::vector< glm::vec2 > temp_uvs;
	std::vector< glm::vec3 > temp_normals;





};

