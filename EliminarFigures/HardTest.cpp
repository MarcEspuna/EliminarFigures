#include "Test.h"
#include "HardTest.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "IndexBuffer.h"
#include "VertexArrayLayout.h"
#include "Shader.h"
#include "Renderer.h"
#include "CollisionDetector.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_opengl3.h"
#include "imgui/imgui_impl_glfw.h"


Test::HardTest::HardTest()
	: m_Proj(glm::ortho(-640.0f, 640.0f, -360.0f, 360.0f)), 
	m_View(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f))),
	u_MVP(glm::mat4(1.0f)), vaoH(), vaoV() ,vaoC(), vaoQuad(), vaoStar(),
	vboH(m_ControlLines.GetPositionsH(), m_ControlLines.GetCountPositions() * sizeof(float)),
	vboV(m_ControlLines.GetPositionsV(), m_ControlLines.GetCountPositions() * sizeof(float))
{
	
	std::cout << "Hard Test created" << std::endl;



}

Test::HardTest::~HardTest()
{
	std::cout << "Hard test destroyed\n";
}

void Test::HardTest::OnUpdate(float deltaTime)
{
}

void Test::HardTest::OnRender()
{
}

void Test::HardTest::OnImGuiRender()
{
}
