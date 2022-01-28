#include "Object.h"
#include "ObjParser.h"
#include "VertexArrayLayout.h"
#include <future>


Object::Object(const char* filePath)
	: m_Data(filePath), u_Color(1.0f, 1.0f, 1.0f, 1.0f), collision(this), shader("res/Basic.shader")
{
	VertexArrayLayout layout;
	vbo.LoadData(&m_Data.GetVerticesIn2D()[0], m_Data.GetVerticesIn2D().size() * sizeof(float));
	ibo.LoadData(&m_Data.GetIndexes()[0], m_Data.GetIndexes().size() * sizeof(unsigned int));
	layout.Push<float>(2);
	vao.AddBuffer(vbo, layout);
	vec_Model.push_back(glm::mat4(1.0f));
	u_MVP.push_back(glm::mat4(1.0f));
	movementValues.push_back({ 1.0f, 1.0f, 1.0f });
}

Object::Object(const char* filePath, glm::vec4 color)
	: m_Data(filePath), u_Color(color), collision(this), shader("res/Basic.shader")
{
	VertexArrayLayout layout;
	vbo.LoadData(&m_Data.GetVerticesIn2D()[0], m_Data.GetVerticesIn2D().size() * sizeof(float));
	ibo.LoadData(&m_Data.GetIndexes()[0], m_Data.GetIndexes().size() * sizeof(unsigned int));
	layout.Push<float>(2);
	vao.AddBuffer(vbo, layout);
	vec_Model.push_back(glm::mat4(1.0f));		
	u_MVP.push_back(glm::mat4(1.0f));
	movementValues.push_back({ 1.0f, 1.0f, 1.0f });
}

Object::Object(const char* filePath, float scale)
	: m_Data(filePath), u_Color(1.0f, 1.0f, 1.0f, 1.0f), collision(this), shader("res/Basic.shader")
{
	m_Data.SetVertexScale(glm::vec3(scale, scale, 0.0f));
	VertexArrayLayout layout;
	vbo.LoadData(&m_Data.GetVerticesIn2D()[0], m_Data.GetVerticesIn2D().size() * sizeof(float));
	ibo.LoadData(&m_Data.GetIndexes()[0], m_Data.GetIndexes().size() * sizeof(unsigned int));
	layout.Push<float>(2);
	vao.AddBuffer(vbo, layout);
	vec_Model.push_back(glm::mat4(1.0f));	
	u_MVP.push_back(glm::mat4(1.0f));
	movementValues.push_back({ 1.0f, 1.0f, 1.0f });
}

Object::Object(const char* filePath, glm::vec4 color, float scale)
	: m_Data(filePath), u_Color(color), collision(this), shader("res/Basic.shader")
{
	m_Data.SetVertexScale(glm::vec3(scale, scale, 0.0f));
	VertexArrayLayout layout;
	vbo.LoadData(&m_Data.GetVerticesIn2D()[0], m_Data.GetVerticesIn2D().size() * sizeof(float));
	ibo.LoadData(&m_Data.GetIndexes()[0], m_Data.GetIndexes().size() * sizeof(unsigned int));
	layout.Push<float>(2);
	vao.AddBuffer(vbo, layout);
	vec_Model.push_back(glm::mat4(1.0f));	
	u_MVP.push_back(glm::mat4(1.0f));
	movementValues.push_back({ 1.0f, 1.0f, 1.0f });
}

Object::Object(const char* filePath, glm::vec4 color, glm::vec3 scale)
	: m_Data(filePath), u_Color(color), collision(this), shader("res/Basic.shader")
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
}


Object::~Object()
{




}

void Object::OnObjectUpdate(bool deleteObject,const float& deltaTime, ImguiVariables& ImGuiVar)
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

void Object::TrackCollisionWith(Object* otherObject)
{
	collision.SetOtherObject(otherObject);

}

void Object::New(const glm::mat4& u_NewModel, const glm::vec3& movement)
{
	vec_Model.push_back(u_NewModel);
	u_MVP.push_back(glm::mat4(1.0f));
	movementValues.push_back(movement);
}

void Object::Bind() const
{
	ibo.Bind();
	vbo.Bind();
	shader.Bind();
}

void Object::setUniform(size_t objectIndex, const glm::mat4& projection, const glm::mat4& view)
{
	u_MVP[objectIndex] = projection * view * vec_Model[objectIndex];                                                                                                          //Update Model Matrix and MVP
	shader.SetUniform4f("u_Color", GetColor());     //Set the color Uniform
	shader.SetUniform4Mat("u_MVP", u_MVP[objectIndex]);
}

