#include "Test.h"
#include "HardTest.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "VertexArrayLayout.h"
#include "Shader.h"
#include "Renderer.h"
#include "CollisionDetector.h"
#include "VertexArrayLayout.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_opengl3.h"
#include "imgui/imgui_impl_glfw.h"

Test::HardTest::HardTest()
    :   m_Donut("res/obj/donut.obj"),
    m_Proj(glm::ortho(-640.0f, 640.0f, -360.0f, 360.0f)),
    m_View(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f))),
    m_Model(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f))),
    u_MVP(glm::mat4(1.0f)), vaoH(), vaoV(), vaoC(), vaoQuad(), vaoStar(), vaoDonut(),
    vboH(m_ControlLines.GetPositionsH(), m_ControlLines.GetCountPositions() * sizeof(float)),
    vboV(m_ControlLines.GetPositionsV(), m_ControlLines.GetCountPositions() * sizeof(float)),
    vboC(m_ControlLines.GetPositionsC(), m_ControlLines.GetCountPositions() * sizeof(float)),
    vboQuad(m_Figures.Square, sizeof(m_Figures.Square)),
    vboStar(m_Figures.Star, sizeof(m_Figures.Star)),
    vboVFigure(m_Figures.V, sizeof(m_Figures.V)),
    vboDonut((float*)&m_Donut.GetVertices()[0], m_Donut.GetVertices().size() * sizeof(glm::vec3)),
    iboH(m_ControlLines.GetIndexH(), m_ControlLines.GetCountIndexes() * sizeof(unsigned int)),
    iboV(m_ControlLines.GetIndexV(), m_ControlLines.GetCountIndexes() * sizeof(unsigned int)),
    iboC(m_ControlLines.GetIndexC(), m_ControlLines.GetCountIndexes() * sizeof(unsigned int)),
    iboQuad(m_Figures.indexQuad, sizeof(m_Figures.indexQuad)),
    iboStar(m_Figures.indexStar, sizeof(m_Figures.indexStar)),
    iboVFigure(m_Figures.indexV, sizeof(m_Figures.indexV)),
    iboDonut((unsigned int*)(&m_Donut.GetIndexes()[0]), m_Donut.GetIndexes().size() * sizeof(unsigned int)),
    shader("res/Basic.shader"),
    collision(m_ControlLines.GetPositionsC(), m_Figures.Square, m_Figures.indexQuad, 6),
    collision2(m_ControlLines.GetPositionsC(), m_Figures.Star, m_Figures.indexStar, 18),
    collision3(m_ControlLines.GetPositionsC(), m_Figures.V, m_Figures.indexV, 6),
    ptr_window(nullptr)
{


    shader.Bind();

    VertexArrayLayout layout;
    VertexArrayLayout layout2;
    layout.Push<float>(2);
    layout2.Push<float>(3);
    vaoH.AddBuffer(vboH, layout);
    vaoV.AddBuffer(vboV, layout);
    vaoC.AddBuffer(vboC, layout);
    vaoQuad.AddBuffer(vboQuad, layout);
    vaoStar.AddBuffer(vboStar, layout);
    vaoVFigure.AddBuffer(vboVFigure, layout);
    vaoDonut.AddBuffer(vboDonut, layout2);
    shader.SetUniform4f("u_Color", 1.0f, 0.5f, 1.0f, 1.0f);

    RegisterWorldBuffer(vaoH, iboH, nullptr);
    RegisterWorldBuffer(vaoV, iboV, nullptr);
    RegisterWorldBuffer(vaoC, iboC, nullptr);
    RegisterWorldBuffer(vaoQuad, iboQuad, &collision);
    RegisterWorldBuffer(vaoStar, iboStar, &collision2);

    std::cout << "Hard Test created" << std::endl;

}


Test::HardTest::HardTest(GLFWwindow* window)
	: m_Donut("res/obj/donut.obj"),
    m_Proj(glm::ortho(-640.0f, 640.0f, -360.0f, 360.0f)),
    m_View(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f))),
    m_Model(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f))),
    u_MVP(glm::mat4(1.0f)), vaoH(), vaoV(), vaoC(), vaoQuad(), vaoStar(), vaoDonut(),
    vboH(m_ControlLines.GetPositionsH(), m_ControlLines.GetCountPositions() * sizeof(float)),
    vboV(m_ControlLines.GetPositionsV(), m_ControlLines.GetCountPositions() * sizeof(float)),
    vboC(m_ControlLines.GetPositionsC(), m_ControlLines.GetCountPositions() * sizeof(float)),
    vboQuad(m_Figures.Square, sizeof(m_Figures.Square)),
    vboStar(m_Figures.Star, sizeof(m_Figures.Star)),
    vboVFigure(m_Figures.V, sizeof(m_Figures.V)),
    vboDonut((float*)&m_Donut.GetVertices()[0], m_Donut.GetVertices().size() * sizeof(glm::vec3)),
    iboH(m_ControlLines.GetIndexH(), m_ControlLines.GetCountIndexes() * sizeof(unsigned int)),
    iboV(m_ControlLines.GetIndexV(), m_ControlLines.GetCountIndexes() * sizeof(unsigned int)),
    iboC(m_ControlLines.GetIndexC(), m_ControlLines.GetCountIndexes() * sizeof(unsigned int)),
    iboQuad(m_Figures.indexQuad, sizeof(m_Figures.indexQuad)),
    iboStar(m_Figures.indexStar, sizeof(m_Figures.indexStar)),
    iboVFigure(m_Figures.indexV, sizeof(m_Figures.indexV)),
    iboDonut((unsigned int*)(&m_Donut.GetIndexes()[0]), m_Donut.GetIndexes().size() * sizeof(unsigned int)),
    shader("res/Basic.shader"),
    collision(m_ControlLines.GetPositionsC(), m_Figures.Square, m_Figures.indexQuad, 6),
    collision2(m_ControlLines.GetPositionsC(), m_Figures.Star, m_Figures.indexStar, 18),
    collision3(m_ControlLines.GetPositionsC(), m_Figures.V, m_Figures.indexV, 6),
    ptr_window(window)
{
    
    
    
    shader.Bind();

    VertexArrayLayout layout;
    VertexArrayLayout layout2;
    layout.Push<float>(2);
    layout2.Push<float>(3);
    vaoH.AddBuffer(vboH, layout);
    vaoV.AddBuffer(vboV, layout);
    vaoC.AddBuffer(vboC, layout);
    vaoQuad.AddBuffer(vboQuad, layout);
    vaoStar.AddBuffer(vboStar, layout);
    vaoVFigure.AddBuffer(vboVFigure, layout);
    vaoDonut.AddBuffer(vboDonut, layout2);
    shader.SetUniform4f("u_Color", 1.0f, 0.5f, 1.0f, 1.0f);

	std::cout << "Hard Test created" << std::endl;

}

Test::HardTest::~HardTest()
{
	std::cout << "Hard test destroyed\n";
}

void Test::HardTest::OnUpdate(float deltaTime)
{
    int state = glfwGetKey(ptr_window, GLFW_KEY_W);
    if (state == GLFW_PRESS)
    {
        m_ControlLines.modelH = glm::translate(m_ControlLines.modelH, glm::vec3(0.0f, 1.0f, 0.0f));
        m_ControlLines.modelC = glm::translate(m_ControlLines.modelC, glm::vec3(0.0f, 1.0f, 0.0f));
    }

    int state1 = glfwGetKey(ptr_window, GLFW_KEY_S);
    if (state1 == GLFW_PRESS)
    {
        m_ControlLines.modelH = glm::translate(m_ControlLines.modelH, glm::vec3(0.0f, -1.0f, 0.0f));
        m_ControlLines.modelC = glm::translate(m_ControlLines.modelC, glm::vec3(0.0f, -1.0f, 0.0f));
    }

    int state2 = glfwGetKey(ptr_window, GLFW_KEY_RIGHT);
    if (state2 == GLFW_PRESS)
    {
        m_ControlLines.modelV = glm::translate(m_ControlLines.modelV, glm::vec3(1.0f, 00.0f, 0.0f));
        m_ControlLines.modelC = glm::translate(m_ControlLines.modelC, glm::vec3(1.0f, 00.0f, 0.0f));
    }

    int state3 = glfwGetKey(ptr_window, GLFW_KEY_LEFT);
    if (state3 == GLFW_PRESS)
    {
        m_ControlLines.modelV = glm::translate(m_ControlLines.modelV, glm::vec3(-1.0f, 0.0f, 0.0f));
        m_ControlLines.modelC = glm::translate(m_ControlLines.modelC, glm::vec3(-1.0f, 0.0f, 0.0f));
    }

}

void Test::HardTest::OnRender()
{
    renderer.Clear();
    glm::vec3 translationCenterQuad = { m_ControlLines.modelC[3][0], m_ControlLines.modelC[3][1] , m_ControlLines.modelC[3][2] };

    m_Model = glm::translate(m_Model, glm::vec3(y, x, 0.0f));

    if (m_Model[3][1] > 50) x = -1.0f;
    if (m_Model[3][1] < -300.0f) x = 1.0f;
    if (m_Model[3][0] > 0) y = -1.0f;
    if (m_Model[3][0] < -300.0f) y = 1.0f;
    u_MVP = m_Proj * m_View * m_Model;
    shader.SetUniform4f("u_Color", 0.5f, 0.0f, 0.3f, 1.0f);
    shader.SetUniform4Mat("u_MVP", u_MVP);
    renderer.Draw(vaoStar, iboStar, shader);
    collision2.Refresh(translationCenterQuad, glm::vec3(m_Model[3][0], m_Model[3][1], m_Model[3][2]));

    u_MVP = m_Proj * m_View * glm::mat4(1.0f);

    shader.SetUniform4f("u_Color", 1.0f, 0.0f, 0.0f, 1.0f);
    shader.SetUniform4Mat("u_MVP", u_MVP);
    renderer.Draw(vaoVFigure, iboVFigure, shader);
    renderer.Draw(vaoQuad, iboQuad, shader);
    collision.Refresh(translationCenterQuad, glm::vec3(0, 0, 0));
    collision3.Refresh(translationCenterQuad, glm::vec3(0.0f, 0.0f, 0.0f));
    u_MVP = m_Proj * m_View * m_ControlLines.modelH;


    shader.SetUniform4f("u_Color", 0.5f, 0.0f, 0.0f, 1.0f);
    shader.SetUniform4Mat("u_MVP", u_MVP);
    renderer.Draw(vaoH, iboH, shader);

    u_MVP = m_Proj * m_View * m_ControlLines.modelV;
    shader.SetUniform4Mat("u_MVP", u_MVP);
    shader.SetUniform4f("u_Color", 0.0f, 0.3f, 0.5f, 1.0f);
    renderer.Draw(vaoV, iboV, shader);


    u_MVP = m_Proj * m_View * m_ControlLines.modelC;
    shader.SetUniform4Mat("u_MVP", u_MVP);
    if (collision.GetStatus() || collision2.GetStatus() || collision3.GetStatus())
        shader.SetUniform4f("u_Color", 0.0f, 1.0f, 0.0f, 1.0f);
    else
        shader.SetUniform4f("u_Color", 0.5f, 0.5f, 0.0f, 1.0f);

    renderer.Draw(vaoC, iboC, shader);
     
    u_MVP = m_Proj * m_View * glm::scale(glm::translate(glm::mat4(1.0f), glm::vec3(-200.0f, 100.0f, 0.0f)), glm::vec3(60.0f, 60.0f, 0.0f));

    shader.SetUniform4f("u_Color", 0.0f, 1.0f, 0.0f, 1.0f);
    shader.SetUniform4Mat("u_MVP", u_MVP);
    renderer.Draw(vaoDonut, iboDonut, shader);
}

void Test::HardTest::OnImGuiRender()
{
    {
        static float f = 0.0f;
        static int counter = 0;

        ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.
        ImGui::SetWindowSize(ImVec2(320, 180), 0);


        ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)

        ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f

        if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
            counter++;
        ImGui::SameLine();
        ImGui::Text("counter = %d", counter);

        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        ImGui::End();
    }
}

void Test::HardTest::RegisterWorldBuffer(VertexArray& vao, IndexBuffer& ibo, CollisionDetector* cdo)
{
    WorldBuffer.push_back({ vao, ibo, cdo });
}

