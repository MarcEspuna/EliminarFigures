#include <iostream>
#include <vector>
#include "ObjectReader.h"
#include "BasicObject.h"
#include "TextureObject.h"

ObjectReader::ObjectReader()
{

}

ObjectReader::~ObjectReader()
{

}

void ObjectReader::loadObjectFiles(const std::vector<ObjectArguments>& arguments, const std::string& basePath)
{
	for (const auto argument : arguments)
	{
		std::string inputfile = basePath + argument.name;
		switch (argument.objectType)
		{
			case ObjectType::LIGHT_OBJECT:
			{
				std::string errorMessage;
				std::vector<tinyobj::shape_t> shapesAux;
				bool ret = tinyobj::LoadObj(shapesAux, materials, errorMessage, inputfile.c_str(), basePath.c_str());
				if (!ret)
				{
					std::cout << "[OBJECT READER]: Error loading file: " << argument.name << std::endl;
					std::cout << "[OBJECT READER]: Error message: " << errorMessage << std::endl;

				}
				this->shapes.push_back(shapesAux[0]);
				break;
			}
			case ObjectType::PICTURE_OBJECT:
			{

				break;
			}

			default:
			{
				std::cout << "[OBJECT READER]: Error, unsupported object type (or file type)" << std::endl;
				break;
			}
		}
		this->inputArguments.push_back(argument);
	}
}



void ObjectReader::buildObjects(std::vector<Object*>& worldVector)
{
	auto shapes_it = std::begin(shapes);
	for (const auto& [name, objectType, slot, scale, color] : inputArguments)
	{
		switch (objectType)
		{
			case ObjectType::LIGHT_OBJECT:
			{
				worldVector.push_back(new ObjectLight(*shapes_it++, glm::scale(slot, glm::vec3(scale)), color, scale));
				break;
			}
			case ObjectType::BASIC_OBJECT:
			{
				std::string filePath = basePath + name;
				worldVector.push_back(new BasicObject(filePath.c_str(), glm::vec4(0.2f, 0.3f, 0.4f, 1.0f)));
				break;
			}
			case ObjectType::PICTURE_OBJECT:
			{
				worldVector.push_back(new TextureObject(name, scale, glm::scale(slot, glm::vec3(scale))));
				break;
			}
			default:
				std::cout << "[OBJECT READER]: Can't build object, object not suported." << std::endl;
				break;
		}
	}
}

void ObjectReader::clear()
{
	shapes.clear();
	inputArguments.clear();
}

ObjectType ObjectArguments::getObjectTypeFromFile(std::string filename)
{
	if (filename.find(".obj") != std::string::npos)		return ObjectType::LIGHT_OBJECT;
	if (filename.find(".png") != std::string::npos 
		|| filename.find(".jpg") != std::string::npos 
		|| filename.find(".jpeg") != std::string::npos)	return ObjectType::PICTURE_OBJECT;
	
	return ObjectType::UNSUPORTED_OBJECT;
}
