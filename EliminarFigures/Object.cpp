#include "Object.h"
#include "ObjParser.h"
#include "VertexArrayLayout.h"

Object::Object(const char* filePath)
	: m_Data(filePath), u_Color(1.0f, 1.0f, 1.0f, 1.0f), collision(this)
{
	VertexArrayLayout layout;
	vbo.LoadData(&m_Data.GetVerticesIn2D()[0], m_Data.GetVerticesIn2D().size() * sizeof(float));
	ibo.LoadData(&m_Data.GetIndexes()[0], m_Data.GetIndexes().size() * sizeof(unsigned int));
	layout.Push<float>(2);
	vao.AddBuffer(vbo, layout);
	vec_Model.push_back(glm::mat4(1.0f));												
}

Object::Object(const char* filePath, glm::vec4 color)
	: m_Data(filePath), u_Color(color), collision(this)
{
	VertexArrayLayout layout;
	vbo.LoadData(&m_Data.GetVerticesIn2D()[0], m_Data.GetVerticesIn2D().size() * sizeof(float));
	ibo.LoadData(&m_Data.GetIndexes()[0], m_Data.GetIndexes().size() * sizeof(unsigned int));
	layout.Push<float>(2);
	vao.AddBuffer(vbo, layout);
	vec_Model.push_back(glm::mat4(1.0f));												
}

Object::Object(const char* filePath, float scale)
	: m_Data(filePath), u_Color(1.0f, 1.0f, 1.0f, 1.0f), collision(this)
{
	m_Data.SetVertexScale(glm::vec3(scale, scale, 0.0f));
	VertexArrayLayout layout;
	vbo.LoadData(&m_Data.GetVerticesIn2D()[0], m_Data.GetVerticesIn2D().size() * sizeof(float));
	ibo.LoadData(&m_Data.GetIndexes()[0], m_Data.GetIndexes().size() * sizeof(unsigned int));
	layout.Push<float>(2);
	vao.AddBuffer(vbo, layout);
	vec_Model.push_back(glm::mat4(1.0f));													
}

Object::Object(const char* filePath, glm::vec4 color, float scale)
	: m_Data(filePath), u_Color(color), collision(this)
{
	m_Data.SetVertexScale(glm::vec3(scale, scale, 0.0f));
	VertexArrayLayout layout;
	vbo.LoadData(&m_Data.GetVerticesIn2D()[0], m_Data.GetVerticesIn2D().size() * sizeof(float));
	ibo.LoadData(&m_Data.GetIndexes()[0], m_Data.GetIndexes().size() * sizeof(unsigned int));
	layout.Push<float>(2);
	vao.AddBuffer(vbo, layout);
	vec_Model.push_back(glm::mat4(1.0f));													
}

Object::Object(const char* filePath, glm::vec4 color, glm::vec3 scale)
	: m_Data(filePath), u_Color(color), collision(this)
{
	m_Data.SetVertexScale(scale);
	VertexArrayLayout layout;
	vbo.LoadData(&m_Data.GetVerticesIn2D()[0], m_Data.GetVerticesIn2D().size() * sizeof(float));
	ibo.LoadData(&m_Data.GetIndexes()[0], m_Data.GetIndexes().size() * sizeof(unsigned int));
	layout.Push<float>(2);
	vao.AddBuffer(vbo, layout);
	vec_Model.push_back(glm::mat4(1.0f));
}


Object::~Object()
{




}

void Object::OnObjectUpdate(bool deleteObject,const float& deltaTime)
{
	//Important!: we first Refresh the collision, then do the model color update
	int indexModel = f_CheckCollision();
	if (indexModel > -1 && deleteObject == true)
	{
		vec_Model.erase(vec_Model.begin() + indexModel);
		collision.End();
	}
	else
	{
		for (auto& u_Model : vec_Model)
		{
			f_ModelColorUpdate(u_Model, u_Color, deltaTime);
		}
	}

}

void Object::TrackCollisionWith(Object* otherObject)
{
	collision.SetOtherObject(otherObject);

}

void Object::New(glm::mat4 u_NewModel)
{
	vec_Model.push_back(u_NewModel);
}

