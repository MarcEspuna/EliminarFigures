#include "ObjectLight.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


ObjectLight::ObjectLight(const char* filePath, glm::vec4 color, float scale, const char* shaderPath)
	: Object(filePath, color, scale, shaderPath)
{
	//m_Data.SetVertexScale(glm::vec3(scale, scale, 1.0f));
	VertexArrayLayout layout;
	VertexArrayLayout layout2;

	vbo.LoadData(m_Data.GetVertices());
	ibo.LoadData(m_Data.GetIndexes());
	ibo.Bind();
	layout.Push<float>(3); //Positions of the 3D object
	vao.AddBuffer(vbo, layout);
	
	vboNormals.LoadData(m_Data.getVertexNormals());	
	layout2.Push<float>(3); //Normals of the 3d object
	vao.AddBuffer(vboNormals, layout2);

	

	vec_Model.push_back(glm::scale(glm::mat4(1.0f), glm::vec3(0.5f,0.5f, 0.5f)));
	u_MVP.push_back(glm::mat4(1.0f));
	movementValues.push_back({ 1.0f, 1.0f, 1.0f });
}

void ObjectLight::OnObjectUpdate(bool deleteObject, const float& deltaTime, ImguiVariables& ImGuiVar)
{
	//vec_Model[0] = glm::scale(vec_Model[0], glm::vec3(1.00001f, 1.00001f, 1.0000f));
	vec_Model[0] = glm::scale(vec_Model[0], glm::vec3(1.00001f, 1.00001f, 1.00001f));


}

void ObjectLight::setUniform(size_t objectIndex, const glm::mat4& projection, const glm::mat4& view)
{	
	glm::vec3 lightPosition = glm::vec3(200.0f, 500.0f, -1.0f); 
	shader.SetUniform4f("u_Color", GetColor());     //Set the color Uniform
	shader.SetUniform4Mat("u_Model", vec_Model[objectIndex]);		
	shader.SetUniform4Mat("u_View", view);
	shader.SetUniform4Mat("u_Proj", projection);
	shader.SetUniform3f("u_LightPos", lightPosition);
	shader.SetUniform3f("u_ambient", glm::vec3(0.15f, 0.15f, 0.1f));
	shader.SetUniform3f("u_diffuse", glm::vec3(0.6f, 0.6f, 0.6f));
	shader.SetUniform3f("u_specular", glm::vec3(0.8f, 0.8f, 0.8f));
	shader.SetUniform1f("u_shininess", 5.0f);
}

