#pragma once
//#include "ObjectLight.h"
//#include "obj_loader/tiny_obj_loader.h"

class ObjectReader
{
public:

	ObjectReader();
	~ObjectReader();

	void loadObjectFiles(const std::vector<std::string>& fileNames, const std::string& basePath);
	//void buildObjects(std::vector<ObjectLight>& worldVector);

private:	

	std::vector<std::string> filePaths;
	//std::vector<tinyobj::shape_t> shapes;
	//std::vector<tinyobj::material_t> materials;


};

