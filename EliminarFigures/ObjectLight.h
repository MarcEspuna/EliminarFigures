#pragma once

#include "Object.h"
#include "obj_loader/tiny_obj_loader.h"
class ObjectLight	: public Object
{ 
public:

	ObjectLight(const tinyobj::shape_t& shape, glm::mat4 u_Model, const float& scale = 1.0f);
	ObjectLight(const tinyobj::shape_t& shape, glm::mat4 u_Model, glm::vec4 color, const float& scale = 1.0f);

	void OnObjectUpdate(bool deleteObject, const float& deltaTime, ImguiVariables& ImGuiVar) override;
	void OnObjectUpdate(bool deleteObject, const float& deltaTime, ImguiVariables& ImGuiVar, DataLink& datalink) override;
	void setUniform(size_t objectIndex, const glm::mat4& projection, const glm::mat4& view) override;
	void updateLink(DataLink& datalink) override;
	void setLightDir(glm::vec3 lightdir) override;
	void setLightParam(int ambient, int diffuse, int specular, int shininess) override;
	void Bind() const;

private:

	glm::vec3 lightDir;
	glm::vec3 ambientLight;
	glm::vec3 diffuseLight;
	glm::vec3 specularLight;
	int shininess;

	VertexBuffer vboNormals;
	void checkExistance(size_t modelIndex);
	



};

