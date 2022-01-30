#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#define TINYOBJLOADER_IMPLEMENTATION
#include "obj_loader/tiny_obj_loader.h"
#include "ObjectLight.h"

ObjectLight::ObjectLight(const char* filePath, const tinyobj::shape_t& shape)
	: Object(filePath, glm::vec4(1.0f,1.0f,1.0f,1.0f), 20.0f, "res/BasicLight.shader")
{
	VertexArrayLayout layout;
	VertexArrayLayout layout2;

	vbo.LoadData(shape.mesh.positions);
	ibo.LoadData(shape.mesh.indices);
	layout.Push<float>(3); //Positions of the 3D object
	vao.AddBuffer(vbo, layout);


	vboNormals.LoadData(shape.mesh.normals);
	layout2.Push<float>(3); //Normals of the 3d object
	vao.AddBuffer(vboNormals, layout2);

	vec_Model.push_back(glm::scale(glm::mat4(1.0f), glm::vec3(200.0f, 200.0f, 200.0f)));
	u_MVP.push_back(glm::mat4(1.0f));
	movementValues.push_back({ 1.0f, 1.0f, 1.0f });

}

void ObjectLight::OnObjectUpdate(bool deleteObject, const float& deltaTime, ImguiVariables& ImGuiVar)
{
	//vec_Model[0] = glm::scale(vec_Model[0], glm::vec3(1.00001f, 1.00001f, 1.0000f));
	vec_Model[0] = glm::scale(vec_Model[0], glm::vec3(1.00001f, 1.00001f, 1.00001f));
	vec_Model[0] = glm::rotate(vec_Model[0], 0.0001f, glm::vec3(0.0f, 1.0f, 0.0f));


}

void ObjectLight::setUniform(size_t objectIndex, const glm::mat4& projection, const glm::mat4& view)
{	
	glm::vec3 lightPosition = glm::vec3(2000.0f, 0.0f, -2000.0f); 
	shader.SetUniform4f("u_Color", GetColor());     //Set the color Uniform
	shader.SetUniform4Mat("u_Model", vec_Model[objectIndex]);		
	shader.SetUniform4Mat("u_View", view);
	shader.SetUniform4Mat("u_Proj", projection);
	shader.SetUniform3f("u_LightPos", lightPosition);
	shader.SetUniform3f("u_ambient", glm::vec3(0.05f, 0.05f, 0.05f));
	shader.SetUniform3f("u_diffuse", glm::vec3(0.3f, 0.3f, 0.3f));
	shader.SetUniform3f("u_specular", glm::vec3(1.0f, 1.0f, 1.0f));
	shader.SetUniform1f("u_shininess", 1.0f);
}

