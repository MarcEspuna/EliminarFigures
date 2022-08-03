#pragma once
#include <vector>
#include "glm/glm.hpp"

class ObjParser
{
public:
	ObjParser(const char* filePath);

	inline std::vector<float>& GetVertices() { return temp_vertices; }
	inline std::vector<unsigned int>& GetIndexes() { return vertexIndices; }
	inline std::vector<float>& GetVerticesIn2D() { return vertex2D; }
	inline const std::vector<float>& getVertexNormals() { return temp_normals; }
	inline const std::vector<unsigned int>& getIndexesNormals() { return normalIndices; }

	const std::vector<float> GetVerticesAndNormals();
	const std::vector<unsigned int> GetVerticesAndNormalsIndexes();
	void SetVertexScale(glm::vec3 scale);

private:
	FILE* file;
	std::vector<float> vertex2D;
	std::vector< unsigned int > vertexIndices, uvIndices, normalIndices;
	std::vector< float > temp_vertices;
	std::vector< glm::vec2 > temp_uvs;
	std::vector< float > temp_normals;





};

