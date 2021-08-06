#include "Test.h"
#include "HardTest.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


Test::HardTest::HardTest()
	: m_Proj(glm::ortho(-640.0f, 640.0f, -360.0f, 360.0f)), 
	m_View(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f))),
	u_MVP(glm::mat4(1.0f))
{
	VertexBuffer vboH(m_ControlLines.GetPositionsH(), m_ControlLines.GetCountPositions() * sizeof(float));
	//FIX



}

Test::HardTest::~HardTest()
{
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
