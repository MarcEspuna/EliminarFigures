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
	~TextureObject();

	inline const VertexArray& GetVAO() const { return vao; }
	inline const IndexBuffer& GetIBO() const { return ibo; }
	inline const Texture& GetTexture() const { return texture; }

private:
	VertexArray vao;
	VertexBuffer vbo;
	IndexBuffer ibo;

	Texture texture;

	float positions[16] =
	{
		-300.0f,  200.0f, -1.0f,  1.0f,				//0
		 300.0f,  200.0f,  1.0f,  1.0f,				//1
		-300.0f, -200.0f, -1.0f, -1.0f,				//2
		 300.0f, -200.0f,  1.0f, -1.0f				//3
	};

	unsigned int indexes[6] =
	{
		0, 1, 3,
		1, 2, 3
	};



};

