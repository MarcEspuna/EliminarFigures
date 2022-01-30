#pragma once
#include "Object.h"
class ObjectLight	: public Object
{
public:

	ObjectLight(const char* filePath, glm::vec4 color, float scale, const char* shaderPath);

	void OnObjectUpdate(bool deleteObject, const float& deltaTime, ImguiVariables& ImGuiVar) override;
	void setUniform(size_t objectIndex, const glm::mat4& projection, const glm::mat4& view) override;

private:
	VertexBuffer vboNormals;
	IndexBuffer iboNormals;

};

