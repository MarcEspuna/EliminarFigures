#include <iostream>
#include <vector>
#include "ObjectReader.h"

ObjectReader::ObjectReader()
{

}

ObjectReader::~ObjectReader()
{

}

void ObjectReader::loadObjectFiles(const std::vector<std::string>& fileNames, const std::string& basePath)
{
	//std::vector<tinyobj::shape_t> shapes;
	//std::vector<tinyobj::material_t> materials;
	/*
	for (const auto& filename : fileNames)
	{
		std::string errorMessage;
		bool ret = tinyobj::LoadObj(shapes, materials, errorMessage, filename.c_str(), basePath.c_str());
		if (!ret)
		{
			std::cout << "[OBJECT READER]: Error, object " << filename << " could not be loaded" << std::endl;
		} 
		std::string filePath = basePath + filename;
		filePaths.push_back(filePath);
	}
	*/
	//std::string errorMessage;
	//bool ret = tinyobj::LoadObj(shapes, materials, errorMessage, fileNames[0].c_str(), basePath.c_str());

}


/*
void ObjectReader::buildObjects(std::vector<ObjectLight>& worldVector)
{
	for (size_t i = 0; i < filePaths.size(); i++)
	{
		worldVector.emplace_back(filePaths[i].c_str(), shapes[i]);
	}

}
*/



