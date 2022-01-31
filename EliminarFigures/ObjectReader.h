#pragma once
#include "ObjectLight.h"

struct ObjectArguments
{
	std::string name;
	ObjectType objectType;
	glm::mat4 u_Model;
};

class ObjectReader
{
public:

	ObjectReader();
	~ObjectReader();

	void loadObjectFiles(const std::vector<ObjectArguments>& arguments, const std::string& basePath);
	void buildObjects(std::vector<Object*>& worldVector);

private:	

	std::vector<ObjectArguments> inputArguments;
	std::string basePath;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;


};

