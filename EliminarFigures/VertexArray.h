#pragma once
#include "VertexBuffer.h"
#include "VertexArrayLayout.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"


class VertexArray
{
public:
	VertexArray();

	~VertexArray();

	void Bind() const;
	void Unbind() const;

	void AddBuffer(const VertexBuffer& vbo, const VertexArrayLayout& layout);

	glm::mat4 u_Model;
	void OnVaoUpdate();

private:
	unsigned int m_RendererID;

};

