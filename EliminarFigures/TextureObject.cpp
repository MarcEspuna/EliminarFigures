#include "TextureObject.h"

TextureObject::TextureObject(const std::string& pathTexture)
	: texture(pathTexture), u_Model(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f)))
{
	VertexArrayLayout layout;
	vbo.LoadData(positions, sizeof(float)*16);
	ibo.LoadData(indexes, sizeof(unsigned int) * 6);
	layout.Push<float>(2);
	layout.Push<float>(2);
	vao.AddBuffer(vbo, layout); 
}

TextureObject::TextureObject(const std::string& pathTexture, const float& scale, glm::mat4 u_Model)
	: texture(pathTexture), u_Model(glm::scale(u_Model, glm::vec3(scale, scale, 0.0f)))
{
	VertexArrayLayout layout;
	vbo.LoadData(positions, sizeof(float) * 16);
	ibo.LoadData(indexes, sizeof(unsigned int) * 6);
	layout.Push<float>(2);
	layout.Push<float>(2);
	vao.AddBuffer(vbo, layout);

}

TextureObject::~TextureObject()
{




}
