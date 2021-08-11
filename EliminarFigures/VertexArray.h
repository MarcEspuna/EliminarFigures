#pragma once
#include "VertexBuffer.h"
#include "VertexArrayLayout.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <algorithm>
#include <functional>

class VertexArray
{
public:
	VertexArray();

	~VertexArray();

	void Bind() const;
	void Unbind() const;

	void AddBuffer(const VertexBuffer& vbo, const VertexArrayLayout& layout);

	glm::mat4 u_Model;				//Canviar a vector per si volem renderitzar el mateix vao en varios llocs
	glm::vec4 u_Color;


	void OnVaoUpdate();
	std::function<void(glm::mat4&, glm::vec4&)> u_ModelUpdate = [&](glm::mat4& model, glm::vec4& color) { };

	float translationX;
	float translationY;
	float rotationAngle;

private:

	unsigned int m_RendererID;

};

