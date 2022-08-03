#include "ObjParser.h"
#include <stdio.h>
#include <iostream>

ObjParser::ObjParser(const char* filePath)
    :file(nullptr)
{

    fopen_s(&file, filePath, "r");
    if (file != NULL)
    {
        while (1) {

            char lineHeader[128];
            // read the first word of the line
            int res = fscanf_s(file, "%s", lineHeader, (unsigned int)sizeof(lineHeader));
            if (res == EOF)
                break; // EOF = End Of File. Quit the loop.

            // else : parse lineHeader

            if (strcmp(lineHeader, "v") == 0)
            {
                glm::vec3 vertex;
                fscanf_s(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
                temp_vertices.push_back(vertex.x);
                temp_vertices.push_back(vertex.y);
                temp_vertices.push_back(vertex.z);
                vertex2D.push_back(vertex.x);
                vertex2D.push_back(vertex.y);
            }
            else if (strcmp(lineHeader, "vt") == 0)
            {
                glm::vec2 uv;
                fscanf_s(file, "%f %f\n", &uv.x, &uv.y);
                temp_uvs.push_back(uv);
            }
            else if (strcmp(lineHeader, "vn") == 0)
            {
                glm::vec3 normal;
                fscanf_s(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
                temp_normals.push_back(normal.x);
                temp_normals.push_back(normal.y);
                temp_normals.push_back(normal.z);
            }
            else if (strcmp(lineHeader, "f") == 0)
            {
                std::string vertex1, vertex2, vertex3;
                unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
                int matches = fscanf_s(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2]);
                if (matches != 9)
                {
                    printf("File can't be read by our simple parser : ( Try exporting with other options\n");
                    break;
                }
                vertexIndices.push_back(vertexIndex[0] - 1);
                vertexIndices.push_back(vertexIndex[1] - 1);
                vertexIndices.push_back(vertexIndex[2] - 1);
                uvIndices.push_back(uvIndex[0] - 1);
                uvIndices.push_back(uvIndex[1] - 1);
                uvIndices.push_back(uvIndex[2] - 1);
                normalIndices.push_back(normalIndex[0] - 1);
                normalIndices.push_back(normalIndex[1] - 1);
                normalIndices.push_back(normalIndex[2] - 1);

            }

        }
        fclose(file);
    }
    else 
    {
        std::cout << "Couldn't Load: " << filePath << std::endl;
    }
}

const std::vector<float> ObjParser::GetVerticesAndNormals()
{
    std::vector<float> verticesAndNormals(temp_vertices.size() + temp_normals.size());
    for (size_t i = 0; i < temp_vertices.size() - 2; i+=3)
    {
        verticesAndNormals.push_back(temp_vertices[i]);
        verticesAndNormals.push_back(temp_vertices[i+1]);
        verticesAndNormals.push_back(temp_vertices[i+2]);
        verticesAndNormals.push_back(temp_normals[i]);
        verticesAndNormals.push_back(temp_normals[i + 1]);
        verticesAndNormals.push_back(temp_normals[i + 2]);
    }
    /*
    for (size_t i = 0; i < temp_normals.size() - 2; i += 3)
    {

    }
    */
    return verticesAndNormals;
}

const std::vector<unsigned int> ObjParser::GetVerticesAndNormalsIndexes()
{
    std::vector<unsigned int> verticesAndNormalIndexes(vertexIndices.size() + normalIndices.size());
    for (size_t i = 0; i < vertexIndices.size() - 2; i += 3)
    {
        verticesAndNormalIndexes.push_back(vertexIndices[i]);
        verticesAndNormalIndexes.push_back(vertexIndices[i + 1]);
        verticesAndNormalIndexes.push_back(vertexIndices[i + 2]);
    }
    for (size_t i = 0; i < normalIndices.size() - 2; i += 3)
    {
        verticesAndNormalIndexes.push_back(normalIndices[i]);
        verticesAndNormalIndexes.push_back(normalIndices[i + 1]);
        verticesAndNormalIndexes.push_back(normalIndices[i + 2]);
    }
    return verticesAndNormalIndexes;
}

void ObjParser::SetVertexScale(glm::vec3 scale)
{
    for (size_t i = 0; i < vertex2D.size()-1; i++)
    {
        vertex2D[i] = vertex2D[i] * scale.x;
        vertex2D[i+1] = vertex2D[i + 1] * scale.y;
        i++;
    }

    for (size_t i = 0; i < temp_vertices.size() - 2; i++)
    {
        temp_vertices[i] = temp_vertices[i] * scale.x;
        temp_vertices[i + 1] = temp_vertices[i + 1] * scale.y;
        temp_vertices[i + 2] = temp_vertices[i + 2] * scale.z;
        i++;
        i++;
    }

}


