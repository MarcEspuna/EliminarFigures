#pragma once
#include "Texture.h"
#include "ObjParser.h"
#include "VertexBuffer.h"
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "VertexArrayLayout.h"

class TextureObject : public Object
{ 
public:
	TextureObject(const std::string& pathTexture);
	TextureObject(const std::string& pathTexture, const float& scale, glm::mat4 u_Model);
	~TextureObject();

	inline const VertexArray& GetVao() const { return vao; }
	inline const IndexBuffer& GetIbo() const { return ibo; }
	inline Texture& GetTexture() { return texture; }
	inline glm::mat4 GetModel() const { return vec_Model[0]; }

	void setUniform(size_t objectIndex, const glm::mat4& projection, const glm::mat4& view) override;
	void OnObjectUpdate(bool deleteObject, const float& deltaTime, ImguiVariables& ImGuiVar) override;
	void Bind() const override; 

private:

	Texture texture;

	float positions[16] =
	{
		-120.0f,  90.0f,  0.0f,  1.0f,				//0
		 120.0f,  90.0f,  1.0f,  1.0f,				//1
		-120.0f, -90.0f,  0.0f,  0.0f,				//2
		 120.0f, -90.0f,  1.0f,  0.0f				//3
	};

	unsigned int indexes[6] =
	{
		0, 1, 3,
		2, 3, 0
	};

	void checkExistance(size_t modelIndex);

};

