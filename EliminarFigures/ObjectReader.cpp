#include <iostream>
#include <vector>
#include "ObjectReader.h"
#include "BasicObject.h"

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
		std::string errorMessage;
		std::string inputfile = basePath + argument.name;
		std::vector<tinyobj::shape_t> shapesAux;
		bool ret = tinyobj::LoadObj(shapesAux, materials, errorMessage, inputfile.c_str(), basePath.c_str());
		if (!ret)
		{
			std::cout << "[OBJECT READER]: Error loading file: " << argument.name << std::endl;
			std::cout << "[OBJECT READER]: Error message: " << errorMessage << std::endl;

		}
		this->inputArguments.push_back(argument);
		this->shapes.push_back(shapesAux[0]);
	}
	std::cout << "Shapes count: " << shapes.size() << std::endl;

}



void ObjectReader::buildObjects(std::vector<Object*>& worldVector)
{

	for (size_t i = 0; i < inputArguments.size(); i++)
	{
		switch (inputArguments[i].objectType)
		{
			case ObjectType::LIGHT_OBJECT:
			{
				worldVector.push_back(new ObjectLight(shapes[i], glm::scale(inputArguments[i].slot, glm::vec3(inputArguments[i].scale)), inputArguments[i].color, inputArguments[i].scale));
				break;
			}
			case ObjectType::BASIC_OBJECT:
			{
				std::string filePath = basePath + inputArguments[i].name;
				worldVector.push_back(new BasicObject(filePath.c_str(), glm::vec4(0.2f, 0.3f, 0.4f, 1.0f)));
				break;
			}
			default:
				std::cout << "[OBJECT READER]: Can't build object, object not suported." << std::endl;
				break;
		}
	}

}




