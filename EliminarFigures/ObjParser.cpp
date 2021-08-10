#include "ObjParser.h"
#include <stdio.h>
#include <iostream>

ObjParser::ObjParser(const char* filePath)
    :file(nullptr)
{
    if (readingFile(filePath))
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
                temp_vertices.push_back(vertex);
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
                temp_normals.push_back(normal);
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
                vertexIndices.push_back(vertexIndex[0]-1);
                vertexIndices.push_back(vertexIndex[1]-1);
                vertexIndices.push_back(vertexIndex[2]-1);
                uvIndices.push_back(uvIndex[0]-1);
                uvIndices.push_back(uvIndex[1]-1);
                uvIndices.push_back(uvIndex[2]-1);
                normalIndices.push_back(normalIndex[0]-1);
                normalIndices.push_back(normalIndex[1]-1);
                normalIndices.push_back(normalIndex[2]-1);
            }

        }
        //Allocating a buffer for 2D
        for (glm::vec3 vertice : temp_vertices)
        {
            vertex2D.push_back(vertice.x);
            vertex2D.push_back(vertice.y);
        }
    }
}

bool ObjParser::readingFile(const char* filePath)
{
    fopen_s(&file, filePath, "r");
    if (file == NULL) {
        printf("Impossible to open the file !\n");
        return false;
    }
    else 
    {
        return true;
    }
}
