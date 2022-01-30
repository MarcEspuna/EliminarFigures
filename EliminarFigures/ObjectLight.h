#pragma once

#include "Object.h"
#include "obj_loader/tiny_obj_loader.h"
class ObjectLight	: public Object
{
public:

	ObjectLight(const char* filePath, const tinyobj::shape_t& shape);

	void OnObjectUpdate(bool deleteObject, const float& deltaTime, ImguiVariables& ImGuiVar) override;
	void setUniform(size_t objectIndex, const glm::mat4& projection, const glm::mat4& view) override;

private:
	VertexBuffer vboNormals;

};

