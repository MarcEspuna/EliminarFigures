#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#define TINYOBJLOADER_IMPLEMENTATION
#include "obj_loader/tiny_obj_loader.h"
#include "ObjectLight.h"

ObjectLight::ObjectLight(const tinyobj::shape_t& shape, glm::mat4 u_Model, const float& scale)
	: Object(glm::vec4(1.0f,1.0f,1.0f,1.0f), "res/BasicLight.shader", shape, u_Model, scale)
{
	shader.Bind();
	VertexArrayLayout layout;
	VertexArrayLayout layout2;

	vbo.LoadData(shape.mesh.positions);
	ibo.LoadData(shape.mesh.indices);
	layout.Push<float>(3, "position"); //Positions of the 3D object
	vao.AddBuffer(vbo, layout, shader.getID());

	vboNormals.LoadData(shape.mesh.normals);
	layout2.Push<float>(3, "aNormal"); //Normals of the 3d object
	vao.AddBuffer(vboNormals, layout2, shader.getID());

	vec_Model.push_back(u_Model);
	u_MVP.push_back(glm::mat4(1.0f));
	movementValues.push_back({ 1.0f, 1.0f, 1.0f });
	activeCollider = true;

	m_CollisionView.Load(m_SquareCollider.getShapes(), m_SquareCollider.getIndex());
}

void ObjectLight::OnObjectUpdate(bool deleteObject, const float& deltaTime, ImguiVariables& ImGuiVar)
{
	for (size_t i = 0; i < vec_Model.size(); i++)
	{
		if (hit)
		{
			objectMovement.scale(1.0f - deltaTime / 10.0f, vec_Model[i], m_SquareCollider);
			objectMovement.rotateY(deltaTime / 50.0f, vec_Model[i], m_SquareCollider);

		}
		else if (m_SquareCollider.thereIsCollision() && deleteObject)
		{
			hit = true;
		}
		objectMovement.rotateY(deltaTime / 50.0f, vec_Model[i], m_SquareCollider);
		m_CollisionView.Load(m_SquareCollider.getShapes(), m_SquareCollider.getIndex());
		checkExistance(i);
	}
}

void ObjectLight::setUniform(size_t objectIndex, const glm::mat4& projection, const glm::mat4& view)
{	
	shader.Bind();
	glm::vec3 lightPosition = glm::vec3(2000.0f, 0.0f, -2000.0f); 
	if (m_SquareCollider.thereIsCollision() && !hit)
	{
		shader.SetUniform4f("u_Color", glm::vec4(0.39f, 0.11f, 0.78f, 1.0f));
	}
	else if (hit)
	{
		shader.SetUniform4f("u_Color", glm::vec4(1.0f, 0.0f,0.0f,1.0f));     //Set the color Uniform
	}
	else
	{
		shader.SetUniform4f("u_Color", GetColor());     //Set the color Uniform
	}
	shader.SetUniform4Mat("u_Model", vec_Model[objectIndex]);		
	shader.SetUniform4Mat("u_View", view);
	shader.SetUniform4Mat("u_Proj", projection);
	shader.SetUniform3f("u_LightPos", lightPosition);
	shader.SetUniform3f("u_ambient", glm::vec3(0.05f, 0.05f, 0.05f));
	shader.SetUniform3f("u_diffuse", glm::vec3(0.5f, 0.5f, 0.5f));
	shader.SetUniform3f("u_specular", glm::vec3(1.0f, 1.0f, 1.0f));
	shader.SetUniform1f("u_shininess", 20.0f);
}


void ObjectLight::Bind() const
{
	vao.Bind();
	vboNormals.Bind();
	vbo.Bind();
	ibo.Bind();
	shader.Bind();
}

void ObjectLight::checkExistance(size_t modelIndex)
{
	if (objectMovement.getScale() < 0.02f && modelIndex < vec_Model.size())
	{
		vec_Model.erase(vec_Model.begin() + modelIndex);
		activeCollider = false;
		hit = false;
	}
}


