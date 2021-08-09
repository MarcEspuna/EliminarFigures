#pragma once
#include <vector>
#include "glm/glm.hpp"

class ObjParser
{
public:
	ObjParser(const char* filePath);


private:
	FILE* file;
	bool readingFile(const char* filePath);

	std::vector< unsigned int > vertexIndices, uvIndices, normalIndices;
	std::vector< glm::vec3 > temp_vertices;
	std::vector< glm::vec2 > temp_uvs;
	std::vector< glm::vec3 > temp_normals;





};

