#pragma once
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "ObjParser.h"
#include "CollisionDetector.h"
#include "ImguiVariables.h"
#include "Shader.h"
#include "Object.h"
#include <functional>

class BasicObject : public Object
{
public:
	BasicObject(const char* filePath);
	BasicObject(const char* filePath, glm::vec4 color);
	BasicObject(const char* filePath, float scale);
	BasicObject(const char* filePath, glm::vec4 color, float scale);
	BasicObject(const char* filePath, glm::vec4 color, float scale, const char* shaderPath);
	BasicObject(const char* filePath, glm::vec4 color, glm::vec3 scale);

	std::function<void(glm::mat4&, const glm::vec2& ,glm::vec4&, const float&, glm::vec3& movement)> f_ModelColorUpdate =
		[&](glm::mat4& model, const glm::vec2& oneVertex ,glm::vec4& color, const float& deltaTime, glm::vec3& movement) {};

	void OnObjectUpdate(bool deleteObject,const float& deltaTime, ImguiVariables& ImGuiVar) override;
	void setUniform(size_t objectIndex, const glm::mat4& projection, const glm::mat4& view) override;


};

