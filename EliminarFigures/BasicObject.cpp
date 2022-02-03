#include "BasicObject.h"
#include "ObjParser.h"
#include "VertexArrayLayout.h"
#include <future>


BasicObject::BasicObject(const char* filePath)
	: Object(), m_Data(filePath)
{
	VertexArrayLayout layout;
	vbo.LoadData(&m_Data.GetVerticesIn2D()[0], m_Data.GetVerticesIn2D().size() * sizeof(float));
	ibo.LoadData(&m_Data.GetIndexes()[0], m_Data.GetIndexes().size() * sizeof(unsigned int));
	layout.Push<float>(2);
	vao.AddBuffer(vbo, layout);
	vec_Model.push_back(glm::mat4(1.0f));
	u_MVP.push_back(glm::mat4(1.0f));
	movementValues.push_back({ 1.0f, 1.0f, 1.0f });
	m_SquareCollider.Load(vec_Model[0], m_Data.GetVertices());
}

BasicObject::BasicObject(const char* filePath, glm::vec4 color)
	: Object(color), m_Data(filePath)
{
	VertexArrayLayout layout;
	vbo.LoadData(&m_Data.GetVerticesIn2D()[0], m_Data.GetVerticesIn2D().size() * sizeof(float));
	ibo.LoadData(&m_Data.GetIndexes()[0], m_Data.GetIndexes().size() * sizeof(unsigned int));
	layout.Push<float>(2);
	vao.AddBuffer(vbo, layout, shader.getID());
	vec_Model.push_back(glm::mat4(1.0f));		
	u_MVP.push_back(glm::mat4(1.0f));
	movementValues.push_back({ 1.0f, 1.0f, 1.0f });
	m_SquareCollider.Load(vec_Model[0], m_Data.GetVertices());
}

BasicObject::BasicObject(const char* filePath, float scale)
	: Object(scale), m_Data(filePath)
{
	m_Data.SetVertexScale(glm::vec3(scale, scale, 1.0f));
	VertexArrayLayout layout;
	vbo.LoadData(&m_Data.GetVerticesIn2D()[0], m_Data.GetVerticesIn2D().size() * sizeof(float));
	ibo.LoadData(&m_Data.GetIndexes()[0], m_Data.GetIndexes().size() * sizeof(unsigned int));
	layout.Push<float>(2);
	vao.AddBuffer(vbo, layout);
	vec_Model.push_back(glm::mat4(1.0f));	
	u_MVP.push_back(glm::mat4(1.0f));
	movementValues.push_back({ 1.0f, 1.0f, 1.0f });
	m_SquareCollider.Load(vec_Model[0], m_Data.GetVertices());
}

BasicObject::BasicObject(const char* filePath, glm::vec4 color, float scale)
	: Object(color), m_Data(filePath)
{
	m_Data.SetVertexScale(glm::vec3(scale, scale, 1.0f));
	VertexArrayLayout layout;
	vbo.LoadData(&m_Data.GetVerticesIn2D()[0], m_Data.GetVerticesIn2D().size() * sizeof(float));
	ibo.LoadData(&m_Data.GetIndexes()[0], m_Data.GetIndexes().size() * sizeof(unsigned int));
	layout.Push<float>(2);
	vao.AddBuffer(vbo, layout);
	vec_Model.push_back(glm::mat4(1.0f));	
	u_MVP.push_back(glm::mat4(1.0f));
	movementValues.push_back({ 1.0f, 1.0f, 1.0f });
	m_SquareCollider.Load(vec_Model[0], m_Data.GetVertices());
}


BasicObject::BasicObject(const char* filePath, glm::vec4 color, float scale, const char* shaderPath)
	: Object(color, shaderPath), m_Data(filePath)
{
	m_Data.SetVertexScale(glm::vec3(scale, scale, 1.0f));
	VertexArrayLayout layout;
	vbo.LoadData(&m_Data.GetVerticesIn2D()[0], m_Data.GetVerticesIn2D().size() * sizeof(float));
	ibo.LoadData(&m_Data.GetIndexes()[0], m_Data.GetIndexes().size() * sizeof(unsigned int));
	layout.Push<float>(2);
	vao.AddBuffer(vbo, layout);
	vec_Model.push_back(glm::mat4(1.0f));
	u_MVP.push_back(glm::mat4(1.0f));
	movementValues.push_back({ 1.0f, 1.0f, 1.0f });
	m_SquareCollider.Load(vec_Model[0], m_Data.GetVertices());
}

BasicObject::BasicObject(const char* filePath, glm::vec4 color, glm::vec3 scale)
	: Object(color, scale), m_Data(filePath)
{
	m_Data.SetVertexScale(scale);
	VertexArrayLayout layout;
	vbo.LoadData(&m_Data.GetVerticesIn2D()[0], m_Data.GetVerticesIn2D().size() * sizeof(float));
	ibo.LoadData(&m_Data.GetIndexes()[0], m_Data.GetIndexes().size() * sizeof(unsigned int));
	layout.Push<float>(2);
	vao.AddBuffer(vbo, layout);
	vec_Model.push_back(glm::mat4(1.0f));
	u_MVP.push_back(glm::mat4(1.0f));
	movementValues.push_back({ 1.0f, 1.0f, 1.0f });
	m_SquareCollider.Load(vec_Model[0], m_Data.GetVertices());
}


BasicObject::BasicObject(const char* filePath, glm::vec4 color, glm::vec3 scale, const glm::mat4& u_Model)
	: Object(color, scale), m_Data(filePath)
{
	m_Data.SetVertexScale(scale);
	VertexArrayLayout layout;
	vbo.LoadData(&m_Data.GetVerticesIn2D()[0], m_Data.GetVerticesIn2D().size() * sizeof(float));
	ibo.LoadData(&m_Data.GetIndexes()[0], m_Data.GetIndexes().size() * sizeof(unsigned int));
	layout.Push<float>(2);
	vao.AddBuffer(vbo, layout);
	vec_Model.push_back(u_Model);
	u_MVP.push_back(glm::mat4(1.0f));
	movementValues.push_back({  1.0f, 1.0f, 1.0f });
	m_SquareCollider.Load(vec_Model[0], m_Data.GetVertices());
}

void BasicObject::OnObjectUpdate(bool deleteObject,const float& deltaTime, ImguiVariables& ImGuiVar)
{
	//Important!: we first Refresh the collision, then do the model color update
	int indexModel = f_CheckCollision();
	if (indexModel > -1 && deleteObject == true)
	{
		ImGuiVar.CachedObjects += 1;
		ImGuiVar.RemainingObjects -= 1;
		vec_Model.erase(vec_Model.begin() + indexModel);
		movementValues.erase(movementValues.begin() + indexModel);
		collision.End();
	}
	else
	{
		for (size_t i = 0; i < vec_Model.size(); i++)
		{
			f_ModelColorUpdate(vec_Model[i], { m_Data.GetVerticesIn2D()[i], m_Data.GetVerticesIn2D()[i+1]}, u_Color, deltaTime, movementValues[i]);
		}

	}

}


void BasicObject::setUniform(size_t objectIndex, const glm::mat4& projection, const glm::mat4& view)
{
	u_MVP[objectIndex] = projection * view * vec_Model[objectIndex];                                                                                                          //Update Model Matrix and MVP
	shader.SetUniform4f("u_Color", GetColor());     //Set the color Uniform
	shader.SetUniform4Mat("u_MVP", u_MVP[objectIndex]);
}

