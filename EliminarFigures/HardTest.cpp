#include "Test.h"
#include "HardTest.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Object.h"
#include "Shader.h"
#include "Renderer.h"
#include "VertexArrayLayout.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_opengl3.h"
#include "imgui/imgui_impl_glfw.h"

Test::HardTest::HardTest()
    : Horse("res/obj/donut.obj", {0.8, 0.3, 0.6, 1.0f}, 30.0f),
    HLine("res/obj/HLine.obj", { 0.7, 0.1, 0.1, 1.0f }, glm::vec3(1.0f, 0.4f, 1.0f) ),
    VLine("res/obj/VLine.obj", { 0.1, 0.2, 0.7, 1.0f }, glm::vec3(0.4f, 1.0f, 1.0f)),
    CQuad("res/obj/CQuad.obj", { 1.0f, 0.96f, 0.22f, 1.0f }, glm::vec3(0.4f, 0.4f, 1.0f)),
    Star("res/obj/Star.obj", {0.1f, 0.1f, 1.0f, 1.0f}, 0.5f),
    Rings("res/obj/Rings.obj", { 0.3, 0.6, 0.3, 1.0f }, 40.0f),
    Pointy("res/obj/Pointy.obj",{ 1.0f, 0.6, 0.3, 1.0f }, 40.0f),
    shader("res/Basic.shader")
{
    //Registering all the objects
    RegisterObject(&Pointy);
    RegisterObject(&Rings);
    RegisterObject(&Horse);
    RegisterObject(&Star);
    RegisterObject(&HLine);
    RegisterObject(&VLine);
    RegisterObject(&CQuad);
    Horse.New(glm::translate(glm::mat4(1.0f), glm::vec3(-300.0f, 100.0f, 0.0f)));
    Rings.New(glm::translate(glm::mat4(1.0f), glm::vec3(300.0f, -100.0f, 0.0f)));
    Pointy.New(glm::translate(glm::mat4(1.0f), glm::vec3(-300.0f, -100.0f, 0.0f)));

    //Loading all the lamdas that will define the behaviour of our objects
    LoadVaoUpdateFuntions();                            

    //Setting collision relations:
    Star.TrackCollisionWith(&CQuad);
    Horse.TrackCollisionWith(&CQuad);
    Rings.TrackCollisionWith(&CQuad);
    Pointy.TrackCollisionWith(&CQuad);

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
        HLine.GetModels()[0] = glm::translate(HLine.GetModels()[0], glm::vec3(0.0f, 1.0f, 0.0f));
        CQuad.GetModels()[0] = glm::translate(CQuad.GetModels()[0], glm::vec3(0.0f, 1.0f, 0.0f));
    }

    int state1 = glfwGetKey(ptr_window, GLFW_KEY_S);
    if (state1 == GLFW_PRESS)
    {
        HLine.GetModels()[0] = glm::translate(HLine.GetModels()[0], glm::vec3(0.0f, -1.0f, 0.0f));
        CQuad.GetModels()[0] = glm::translate(CQuad.GetModels()[0], glm::vec3(0.0f, -1.0f, 0.0f));
    }

    int state2 = glfwGetKey(ptr_window, GLFW_KEY_RIGHT);
    if (state2 == GLFW_PRESS)
    {
        VLine.GetModels()[0] = glm::translate(VLine.GetModels()[0], glm::vec3(1.0f, 0.0f, 0.0f));
        CQuad.GetModels()[0] = glm::translate(CQuad.GetModels()[0], glm::vec3(1.0f, 0.0f, 0.0f));
    }

    int state3 = glfwGetKey(ptr_window, GLFW_KEY_LEFT);
    if (state3 == GLFW_PRESS)
    {
        VLine.GetModels()[0] = glm::translate(VLine.GetModels()[0], glm::vec3(-1.0f, 0.0f, 0.0f));
        CQuad.GetModels()[0] = glm::translate(CQuad.GetModels()[0], glm::vec3(-1.0f, 0.0f, 0.0f));
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
}

void Test::HardTest::OnRender()
{
    renderer.Clear();
    for (auto& object : WorldBuffer)
    {
        object->OnObjectUpdate(CatchingObject);
        for (auto& u_Model : object->GetModels()) {
            u_MVP = m_Proj * m_View * u_Model;                                                                                                          //Update Model Matrix and MVP
            shader.SetUniform4f("u_Color", object->GetColor());     //Set the color Uniform
            shader.SetUniform4Mat("u_MVP", u_MVP);
            renderer.Draw(object->GetVao(), object->GetIbo(), shader);
        }
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
    Pointy.f_ModelColorUpdate = [&](glm::mat4& model, glm::vec4& color)
    {
        model = glm::translate(model, glm::vec3(-0.05f, 0.0f, 0.0f));
    };


    Rings.f_ModelColorUpdate = [&](glm::mat4& model, glm::vec4& color)
    {
        model = glm::translate(model, glm::vec3(-0.1f, -0.1f, 0.0f));
    };


    Horse.f_ModelColorUpdate = [&](glm::mat4& model, glm::vec4& color)
    {
        //model = glm::translate(glm::mat4(1.0f), glm::vec3((float)glfwGetTime()*20, 0.0f, 0.0f));
    };

    Star.f_ModelColorUpdate = [&](glm::mat4& model, glm::vec4& color)
    {
        model = glm::rotate(glm::mat4(1.0f), (float)glfwGetTime()/2, glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::translate(model, glm::vec3(450.0f, 0.0f, 0.0f));
        model = glm::rotate(model, -(float)glfwGetTime()*1.5f, glm::vec3(0.0f, 0.0f, 1.0f));

    };

    CQuad.f_ModelColorUpdate = [&](glm::mat4& model, glm::vec4& color)
    {
        if (Star.GetCollisionStatus() || Horse.GetCollisionStatus() || Rings.GetCollisionStatus() || Pointy.GetCollisionStatus())
        {
            color = glm::vec4(1.0f, 0.0f, 0.0f, 0.0f);
        }
        else
        {
            color = glm::vec4(0.0f, 1.0f, 0.7f, 0.0f);
        }

    };

}

void Test::HardTest::RegisterObject(Object* object)
{
    WorldBuffer.push_back(object);
    std::pair<unsigned int, bool> objectTrack((unsigned int)(WorldBuffer.size() - 1), true);
}


