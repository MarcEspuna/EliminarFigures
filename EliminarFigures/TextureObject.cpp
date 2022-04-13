#include "TextureObject.h"

static std::vector<float> create3DPositions(float positions[16]);

// Not currently used
TextureObject::TextureObject(const std::string& pathTexture)
	: texture(pathTexture)				// Initialize the texture
{
	VertexArrayLayout layout;
	vbo.LoadData(positions, sizeof(float)*16);
	ibo.LoadData(indexes, sizeof(unsigned int) * 6);
	layout.Push<float>(2);
	layout.Push<float>(2);
	vao.AddBuffer(vbo, layout); 

	vec_Model.push_back(glm::mat4(1.0f));
	u_MVP.push_back(glm::mat4(1.0f));
	movementValues.push_back({ 1.0f, 1.0f, 1.0f });
	activeCollider = true;

	m_SquareCollider.Load(glm::mat4(1.0f), create3DPositions(positions));
	m_CollisionView.Load(m_SquareCollider.getShapes(), m_SquareCollider.getIndex());
}

TextureObject::TextureObject(const std::string& pathTexture, const float& scale, glm::mat4 u_Model)
	: Object({1.0f,1.0f,1.0f,1.0f}, "res/TexBasic.shader", scale), texture(pathTexture)	// Initialize the texture 
{
	VertexArrayLayout layout;
	vbo.LoadData(positions, sizeof(float) * 16);
	ibo.LoadData(indexes, sizeof(unsigned int) * 6);
	layout.Push<float>(2);
	layout.Push<float>(2);
	vao.AddBuffer(vbo, layout);

	vec_Model.push_back(u_Model);
	u_MVP.push_back(glm::mat4(1.0f));
	movementValues.push_back({ 1.0f, 1.0f, 1.0f });
	activeCollider = true;

	m_SquareCollider.Load(u_Model, create3DPositions(positions));
	m_CollisionView.Load(m_SquareCollider.getShapes(), m_SquareCollider.getIndex());
}

TextureObject::~TextureObject()
{


}

void TextureObject::Bind() const
{
	texture.Bind(0); 
	vao.Bind(); 
	ibo.Bind(); 
}

void TextureObject::setUniform(size_t objectIndex, const glm::mat4& projection, const glm::mat4& view)
{
	shader.Bind();
	glm::vec3 lightPosition = glm::vec3(0.0f, 0.0f, -2000.0f);
	shader.SetUniform1i("u_Texture", 0);     //Set the color Uniform
	shader.SetUniform4Mat("u_Model", vec_Model[objectIndex]);
	shader.SetUniform4Mat("u_View", view);
	shader.SetUniform4Mat("u_Proj", projection);
}


void TextureObject::OnObjectUpdate(bool deleteObject, const float& deltaTime, ImguiVariables& ImGuiVar)
{
	for (size_t i = 0; i < vec_Model.size(); i++)
	{
		m_CollisionView.Load(m_SquareCollider.getShapes(), m_SquareCollider.getIndex());
		if (hit)
		{
			objectMovement.scale(1.0f - deltaTime / 10.0f, vec_Model[i], m_SquareCollider);

		}
		else if (m_SquareCollider.thereIsCollision() && deleteObject)
		{
			hit = true;
		}
		checkExistance(i);
	}
}

void TextureObject::checkExistance(size_t modelIndex)
{
	if (objectMovement.getScale() < 0.02f && modelIndex < vec_Model.size())
	{
		vec_Model.erase(vec_Model.begin() + modelIndex);
		activeCollider = false;
		hit = false;	
	}
}

//STATIC METHODS OF THE MODULE
static std::vector<float> create3DPositions(float positions[16])
{
	std::vector<float> positions3D =
	{
		positions[0],positions[1],1.0f,
		positions[2],positions[3],1.0f,
		positions[4],positions[5],1.0f,
		positions[6],positions[7],1.0f,
		positions[8],positions[9],1.0f,
		positions[10],positions[11],1.0f,
		positions[12],positions[13],1.0f,
		positions[14],positions[15],1.0f
	};
	return positions3D;
}