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

	glm::mat4 u_Model;
	glm::vec4 u_Color;


	void OnVaoUpdate();
	std::function<void(glm::mat4&, glm::vec4&)> u_ModelUpdate = [&](glm::mat4& model, glm::vec4& color) { };

private:

	unsigned int m_RendererID;

};

