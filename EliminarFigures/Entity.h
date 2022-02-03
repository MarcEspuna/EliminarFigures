#pragma once
#include "glm/glm.hpp"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "obj_loader/tiny_obj_loader.h"
#include <vector>

class Entity
{
public:
	Entity();
	Entity(glm::vec4 color);
	Entity(glm::vec4 color, const char* shaderPath);
	Entity(glm::vec4 color, const char* shaderPath, const tinyobj::shape_t& shape, const glm::mat4& u_Model);
	Entity(glm::vec4 color, glm::vec3 scale);

	virtual void setUniform(size_t objectIndex, const glm::mat4& projection, const glm::mat4& view) = 0;
	void Bind() const;

	//virtual void moveUP(const float& deltaTime, const float& sensitivity);
	//virtual void moveDown(const float& deltaTime, const float& sensitivity);
	//virtual void moveRight(const float& deltaTime, const float& sensitivity);
	//virtual void moveLeft(const float& deltaTime, const float& sensitivity);

	inline const glm::vec4& GetColor() { return u_Color; }
	inline size_t getIboCount() const { return ibo.GetCount(); };


protected:

	VertexArray vao;
	VertexBuffer vbo;
	IndexBuffer ibo;
	Shader shader;
	glm::vec4 u_Color;									//Fixed color for all the objects

	


};

