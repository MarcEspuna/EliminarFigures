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
#include <algorithm>


Test::HardTest::HardTest()
    :   m_Donut("res/obj/donut.obj"),
    m_Proj(glm::ortho(-640.0f, 640.0f, -360.0f, 360.0f)),
    m_View(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f))),
    u_MVP(glm::mat4(1.0f)), vaoH(), vaoV(), vaoC(), vaoQuad(), vaoStar(), vaoDonut(),
    vboH(m_ControlLines.GetPositionsH(), m_ControlLines.GetCountPositions() * sizeof(float)),
    vboV(m_ControlLines.GetPositionsV(), m_ControlLines.GetCountPositions() * sizeof(float)),
    vboC(m_ControlLines.GetPositionsC(), m_ControlLines.GetCountPositions() * sizeof(float)),
    vboQuad(m_Figures.Square, sizeof(m_Figures.Square)),
    vboStar(m_Figures.Star, sizeof(m_Figures.Star)),
    vboVFigure(m_Figures.V, sizeof(m_Figures.V)),
    vboDonut( &m_Donut.GetVerticesIn2D()[0], m_Donut.GetVerticesIn2D().size() * sizeof(float)),
    iboH(m_ControlLines.GetIndexH(), m_ControlLines.GetCountIndexes() * sizeof(unsigned int)),
    iboV(m_ControlLines.GetIndexV(), m_ControlLines.GetCountIndexes() * sizeof(unsigned int)),
    iboC(m_ControlLines.GetIndexC(), m_ControlLines.GetCountIndexes() * sizeof(unsigned int)),
    iboQuad(m_Figures.indexQuad, sizeof(m_Figures.indexQuad)),
    iboStar(m_Figures.indexStar, sizeof(m_Figures.indexStar)),
    iboVFigure(m_Figures.indexV, sizeof(m_Figures.indexV)),
    iboDonut((&m_Donut.GetIndexes()[0]), m_Donut.GetIndexes().size() * sizeof(unsigned int)),
    shader("res/Basic.shader"),
    collision(m_ControlLines.GetPositionsC(), m_Figures.Square, m_Figures.indexQuad, 6),
    collision2(m_ControlLines.GetPositionsC(), m_Figures.Star, m_Figures.indexStar, 18),
    collision3(m_ControlLines.GetPositionsC(), m_Figures.V, m_Figures.indexV, 6),
    collision4(m_ControlLines.GetPositionsC(), m_Donut.GetVerticesIn2D(), m_Donut.GetIndexes(), (unsigned int)m_Donut.GetIndexes().size()),
    ptr_window(nullptr),
    IndexTracking(0),
    CatchingObject(false)
{
    vaoDonut.u_Model = glm::translate(vaoDonut.u_Model, glm::vec3(-300.0f, 100.0f, 0.0f));
    vaoDonut.u_Model = glm::scale(vaoDonut.u_Model, glm::vec3(40.0f, 40.0f, 0.0f));
    vaoDonut.u_Color = glm::vec4(0.909f, 0.776f, 0.188f, 1.0f);

    shader.Bind();

    VertexArrayLayout layout;
    layout.Push<float>(2);
    vaoH.AddBuffer(vboH, layout);
    vaoV.AddBuffer(vboV, layout);
    vaoC.AddBuffer(vboC, layout);
    vaoQuad.AddBuffer(vboQuad, layout);
    vaoStar.AddBuffer(vboStar, layout);
    vaoVFigure.AddBuffer(vboVFigure, layout);
    vaoDonut.AddBuffer(vboDonut, layout);

    //Order of Registerig translates to order of drawing
    RegisterWorldBuffer(vaoQuad, iboQuad, &collision);
    RegisterWorldBuffer(vaoStar, iboStar, &collision2);
    RegisterWorldBuffer(vaoDonut, iboDonut, nullptr);
    RegisterWorldBuffer(vaoH, iboH, nullptr);
    RegisterWorldBuffer(vaoV, iboV, nullptr);
    RegisterWorldBuffer(vaoC, iboC, nullptr);

    LoadVaoUpdateFuntions();                            //Loading all the lamdas that will define the behaviour of our objects

    std::cout << "Easy Test created" << std::endl;

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
        vaoH.u_Model = glm::translate(vaoH.u_Model, glm::vec3(0.0f, 1.0f, 0.0f));
        vaoC.u_Model = glm::translate(vaoC.u_Model, glm::vec3(0.0f, 1.0f, 0.0f));
    }

    int state1 = glfwGetKey(ptr_window, GLFW_KEY_S);
    if (state1 == GLFW_PRESS)
    {
        vaoH.u_Model = glm::translate(vaoH.u_Model, glm::vec3(0.0f, -1.0f, 0.0f));
        vaoC.u_Model = glm::translate(vaoC.u_Model, glm::vec3(0.0f, -1.0f, 0.0f));
    }

    int state2 = glfwGetKey(ptr_window, GLFW_KEY_RIGHT);
    if (state2 == GLFW_PRESS)
    {
        vaoV.u_Model = glm::translate(vaoV.u_Model, glm::vec3(1.0f, 0.0f, 0.0f));
        vaoC.u_Model = glm::translate(vaoC.u_Model, glm::vec3(1.0f, 0.0f, 0.0f));
    }

    int state3 = glfwGetKey(ptr_window, GLFW_KEY_LEFT);
    if (state3 == GLFW_PRESS)
    {
        vaoV.u_Model = glm::translate(vaoV.u_Model, glm::vec3(-1.0f, 0.0f, 0.0f));
        vaoC.u_Model = glm::translate(vaoC.u_Model, glm::vec3(-1.0f, 0.0f, 0.0f));
    }

    int state4 = glfwGetKey(ptr_window, GLFW_KEY_K);
    if (state4 == GLFW_PRESS)
    {
        CatchingObject = true;
    }
    else if (state4 == GLFW_RELEASE)
    {
        CatchingObject = false;
    }

    int state5 = glfwGetKey(ptr_window, GLFW_KEY_R);
    if (state5 == GLFW_PRESS)
    {
        DeletedObjects[1] = true;
    }


}

void Test::HardTest::OnRender()
{
    renderer.Clear();
    glm::vec3 translationCenterQuad = { m_ControlLines.modelC[3][0], m_ControlLines.modelC[3][1] , m_ControlLines.modelC[3][2] };
    
    IndexTracking = 0;
    for (auto& object : WorldBuffer)
    {
        if (DeletedObjects[IndexTracking]) {
            std::get<0>(object).OnVaoUpdate();
            u_MVP = m_Proj * m_View * std::get<0>(object).u_Model;                                                                                                          //Update Model Matrix and MVP
            shader.SetUniform4f("u_Color", std::get<0>(object).u_Color.x, std::get<0>(object).u_Color.y, std::get<0>(object).u_Color.z, std::get<0>(object).u_Color.t);     //Set the color Uniform
            shader.SetUniform4Mat("u_MVP", u_MVP);
            renderer.Draw(std::get<0>(object), std::get<1>(object), shader);
        }
        IndexTracking++;
    }
}

void Test::HardTest::OnImGuiRender()
{
    {
        static int counter = 0;

        ImGui::Begin("Statistics Window!");                         // Create a window called "Hello, world!" and append into it.
        ImGui::SetWindowSize(ImVec2(320, 180), 0);


        ImGui::Text("Cached objects: %d", 0);                       // Display some text (you can use a format strings too)
        ImGui::Text("Objects left: %d", 7);                         // Display some text (you can use a format strings too)

        if (ImGui::Button("Button"))                                // Buttons return true when clicked (most widgets return true when edited/activated)
            counter++;
        ImGui::SameLine();
        ImGui::Text("counter = %d", counter);

        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        ImGui::End();
    }
}

void Test::HardTest::LoadVaoUpdateFuntions()
{

    vaoDonut.u_ModelUpdate = [&](glm::mat4& model, glm::vec4& color) 
    { 
        model = glm::translate(model, glm::vec3(0.001f, 0.0, 0.0f));
        collision4.UpdateVerticiesObj(model);
        collision4.RefreshObj({ vaoC.u_Model[3][0], vaoC.u_Model[3][1] ,vaoC.u_Model[3][2] });
        if (collision4.GetStatus() && CatchingObject)
        {
            collision4.End();
            DeletedObjects[IndexTracking] = false;
        }
    
    };

    vaoStar.u_ModelUpdate = [&](glm::mat4& model, glm::vec4& color)
    {
        vaoStar.rotationAngle = 0.005f;
        if (model[3][0] > 200) vaoStar.translationX = -1.0f;
        if (model[3][0] < -200) vaoStar.translationX = 1.0f;
        color = glm::vec4(0.0f, 0.2f, 0.8f, 1.0f);
        model = glm::rotate(glm::mat4(1.0f), (float)glfwGetTime()/2, glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::translate(model, glm::vec3(400.0f, 0, 0.0f));
        model = glm::rotate(model, -(float)glfwGetTime()*1.5f, glm::vec3(0.0f, 0.0f, 1.0f));
        collision2.UpdateVerticies(vaoStar.u_Model, sizeof(m_Figures.Star) / sizeof(float));
        collision2.Refresh({ vaoC.u_Model[3][0], vaoC.u_Model[3][1] , vaoC.u_Model[3][2] });
        if (collision2.GetStatus() && CatchingObject)
        {
            collision2.End();
            DeletedObjects[IndexTracking] = false;
        }


    };

    vaoH.u_ModelUpdate = [&](glm::mat4& model, glm::vec4& color)
    {
        color = glm::vec4(0.8f, 0.2f, 0.1f, 1.0f);
    };

    vaoV.u_ModelUpdate = [&](glm::mat4& model, glm::vec4& color)
    {
        color = glm::vec4(0.1f, 0.2f, 0.8f, 1.0f);
    };

    vaoC.u_ModelUpdate = [&](glm::mat4& model, glm::vec4& color)
    {
        color = { 0.5f, 0.1f, 0.4f, 1.0f };
        if (collision2.GetStatus() || collision4.GetStatus() || collision.GetStatus())
        {
            color = { 0.0f, 1.0f, 0.0f, 1.0f };
        }
        else
        {
            color = { 0.5f, 0.1f, 0.4f, 1.0f };
        }
    };

    vaoQuad.u_ModelUpdate = [&](glm::mat4& model, glm::vec4& color)
    {
        color = glm::vec4(0.8f, 0.3f, 0.6f, 1.0f);
        collision.UpdateVerticies(model, sizeof(m_Figures.Square) / sizeof(float));
        collision.Refresh({ vaoC.u_Model[3][0], vaoC.u_Model[3][1] , vaoC.u_Model[3][2] });
        if (collision.GetStatus() && CatchingObject)
        {
            collision.End();
            DeletedObjects[IndexTracking] = false;
        }
    };

}

void Test::HardTest::RegisterWorldBuffer(VertexArray& vao, IndexBuffer& ibo, CollisionDetector* cdo)
{
    WorldBuffer.push_back({ vao, ibo, cdo });
}