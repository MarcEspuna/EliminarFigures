#include "TextureObject.h"

TextureObject::TextureObject(const std::string& pathTexture)
	: texture(pathTexture)
{
	VertexArrayLayout layout;
	vbo.LoadData(positions, sizeof(positions));
	ibo.LoadData(indexes, sizeof(indexes));
	layout.Push<float>(2);
	layout.Push<float>(2);
	vao.AddBuffer(vbo, layout);

}

TextureObject::~TextureObject()
{




}
