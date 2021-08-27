#pragma once
#include "Texture.h"
#include "ObjParser.h"
#include "VertexBuffer.h"
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "VertexArrayLayout.h"


class TextureObject
{
public:
	TextureObject(const std::string& pathTexture);
	TextureObject(const std::string& pathTexture, const float& scale, glm::mat4 u_Model);
	~TextureObject();

	inline const VertexArray& GetVao() const { return vao; }
	inline const IndexBuffer& GetIbo() const { return ibo; }
	inline Texture& GetTexture() { return texture; }
	inline glm::mat4 GetModel() const { return u_Model; }
	inline const int Bind() { texture.Bind(0); vao.Bind(); ibo.Bind(); return 0; }


private:
	VertexArray vao;
	VertexBuffer vbo;
	IndexBuffer ibo;

	glm::mat4 u_Model;
	Texture texture;

	float positions[16] =
	{
		-300.0f,  200.0f,  0.0f,  1.0f,				//0
		 300.0f,  200.0f,  1.0f,  1.0f,				//1
		-300.0f, -200.0f,  0.0f,  0.0f,				//2
		 300.0f, -200.0f,  1.0f,  0.0f				//3
	};

	unsigned int indexes[6] =
	{
		0, 1, 3,
		2, 3, 0
	};



};

