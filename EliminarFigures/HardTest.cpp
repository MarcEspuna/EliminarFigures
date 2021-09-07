#include "Test.h"
#include "HardTest.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Object.h"
#include "Shader.h"
#include "VertexArrayLayout.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_opengl3.h"
#include "imgui/imgui_impl_glfw.h"

#include "Texture.h"

Test::HardTest::HardTest()
    : Horse("res/obj/donut.obj", {0.8, 0.3, 0.6, 1.0f}, 30.0f),
    HLine("res/obj/HLine.obj", { 0.7, 0.1, 0.1, 1.0f }, glm::vec3(1.0f, 0.4f, 1.0f) ),
    VLine("res/obj/VLine.obj", { 0.1, 0.2, 0.7, 1.0f }, glm::vec3(0.4f, 1.0f, 1.0f)),
    CQuad("res/obj/CQuad.obj", { 1.0f, 0.96f, 0.22f, 1.0f }, glm::vec3(0.4f, 0.4f, 1.0f)),
    Star("res/obj/Star.obj", {0.1f, 0.1f, 1.0f, 1.0f}, 0.5f),
    Rings("res/obj/Rings.obj", { 0.3, 0.6, 0.3, 1.0f }, 40.0f),
    tex_GameOver("res/textures/GameOverTransparent.png"),
    tex_YouLose("res/textures/YouLoseTransparent.png", 0.45f, glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -100.0f, 0.0f))),
    tex_YouWin("res/textures/YouWinTransparent.png", 0.50f, glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -100.0f, 0.0f))),
    shader("res/Basic.shader"),
    TexShader("res/TexBasic.shader")
{
    Rings.GetModels()[0] = glm::translate(glm::mat4(1.0f), glm::vec3(400.0f, -50.0f, 0.0f));
    //Registering all the objects
    RegisterObject(&Rings);
    RegisterObject(&Horse);
    RegisterObject(&Star);
    RegisterObject(&HLine);
    RegisterObject(&VLine);
    RegisterObject(&CQuad);
    RegisterTexture(&tex_GameOver);
    RegisterTexture(&tex_YouLose);
    RegisterTexture(&tex_YouWin);
    Horse.New(glm::translate(glm::mat4(1.0f), glm::vec3(-300.0f, 100.0f, 0.0f)));
    Rings.New(glm::translate(glm::mat4(1.0f), glm::vec3(-500.0f, -300.0f, 0.0f)));
    Star.New(glm::translate(glm::mat4(1.0f), glm::vec3(300.0f, 0.0f, 0.0f)));
    Star.New(glm::translate(glm::mat4(1.0f), glm::vec3(-300.0f, 0.0f, 0.0f)));
    Star.New(glm::translate(glm::mat4(1.0f), glm::vec3(600.0f, 0.0f, 0.0f)));
    //Loading all the lamdas that will define the behaviour of our objects
    LoadObjectUpdateFuntions();                            

    //Setting collision relations:
    Star.TrackCollisionWith(&CQuad);
    Horse.TrackCollisionWith(&CQuad);
    Rings.TrackCollisionWith(&CQuad);

    glEnable(GL_BLEND);
    //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    std::cout << "Hard Test created" << std::endl;

}


static void ayncObjectUpdate(Object* object, float deltaTime, bool CatchingObject, ImguiVariables* imguiVar)
{
    object->OnObjectUpdate(CatchingObject, deltaTime, *imguiVar);
}

Test::HardTest::~HardTest()
{
	std::cout << "Hard test destroyed\n";
}

void Test::HardTest::OnUpdate(float deltaTime, bool& testExit)
{
    Time += deltaTime / 20;                                                                                  //We keep track of the time passed during this test                             
    TimeLeft -= deltaTime / 20;

    int state = glfwGetKey(ptr_window, GLFW_KEY_W);
    if (state == GLFW_PRESS)
    {
        HLine.GetModels()[0] = glm::translate(HLine.GetModels()[0], glm::vec3(0.0f, deltaTime * 5, 0.0f));
        CQuad.GetModels()[0] = glm::translate(CQuad.GetModels()[0], glm::vec3(0.0f, deltaTime * 5, 0.0f));
    }

    int state1 = glfwGetKey(ptr_window, GLFW_KEY_S);
    if (state1 == GLFW_PRESS)
    {
        HLine.GetModels()[0] = glm::translate(HLine.GetModels()[0], glm::vec3(0.0f, -deltaTime * 5, 0.0f));
        CQuad.GetModels()[0] = glm::translate(CQuad.GetModels()[0], glm::vec3(0.0f, -deltaTime * 5, 0.0f));
    }

    int state2 = glfwGetKey(ptr_window, GLFW_KEY_RIGHT);
    if (state2 == GLFW_PRESS)
    {
        VLine.GetModels()[0] = glm::translate(VLine.GetModels()[0], glm::vec3(deltaTime*5, 0.0f, 0.0f));
        CQuad.GetModels()[0] = glm::translate(CQuad.GetModels()[0], glm::vec3(deltaTime*5, 0.0f, 0.0f));
    }

    int state3 = glfwGetKey(ptr_window, GLFW_KEY_LEFT);
    if (state3 == GLFW_PRESS)
    {
        VLine.GetModels()[0] = glm::translate(VLine.GetModels()[0], glm::vec3(-deltaTime*5, 0.0f, 0.0f));
        CQuad.GetModels()[0] = glm::translate(CQuad.GetModels()[0], glm::vec3(-deltaTime*5, 0.0f, 0.0f));
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

#define ASYNC 0
#if ASYNC
    m_Futures.clear();
    for (auto& object : WorldBuffer)
    {
        m_Futures.push_back(std::async(std::launch::async, ayncObjectUpdate, object, deltaTime, CatchingObject, &m_Imgui));
    }
    //We shoud make the main thred to wait for the small threads to finish or OnRender make the main thread to check if the object has been updated
    
#else

    for (auto& object : WorldBuffer)
    {
        object->OnObjectUpdate(CatchingObject, deltaTime, m_Imgui);
    }
#endif


    if (Time > 10 && !newTest)
    {
        Rings.New(glm::translate(glm::mat4(1.0f), glm::vec3(300.0f, 100.0f, 0.0f)));
        Horse.New(glm::translate(glm::mat4(1.0f), glm::vec3(-300.0f, 100.0f, 0.0f)));
        newTest = 1;
    }

    if (TimeLeft <= -5.0f)
    {
        testExit = true;
    }

    LoadNewObjects(TimeLeft);

    if (TimeLeft < 1.0f && TimeLeft > -1.0f)
    {
        if (m_Imgui.RemainingObjects == 0)
        {
            winOrLose = WinOrLose::WON;
        }
    }
}

void Test::HardTest::OnRender()
{
    renderer.Clear();
    //A for loop for rendering the texture objects:

    for (auto& object : WorldBuffer)
    {
        for (auto& u_Model : object->GetModels()) {
            u_MVP = m_Proj * m_View * u_Model;                                                                                                          //Update Model Matrix and MVP
            shader.SetUniform4f("u_Color", object->GetColor());     //Set the color Uniform
            shader.SetUniform4Mat("u_MVP", u_MVP);
            renderer.Draw(object->GetVao(), object->GetIbo(), shader);
        }
    }
    if (TimeLeft <= 0.0f)
    {
        u_MVP = m_Proj * m_View * glm::mat4(1.0f);                                                                                                          //Update Model Matrix and MVP
        TexShader.SetUniform4Mat("u_MVP", u_MVP);
        TexShader.SetUniform1i("u_Texture", 0);     //Set the color Uniform
        TextureBuffer[0]->Bind();
        renderer.Draw(TextureBuffer[0]->GetVao(), TextureBuffer[0]->GetIbo(), TexShader);
    }
    if (TimeLeft <= -2.0f)
    {
        u_MVP = m_Proj * m_View * TextureBuffer[(int)winOrLose]->GetModel();                                                                                                          //Update Model Matrix and MVP
        TexShader.SetUniform4Mat("u_MVP", u_MVP);
        TexShader.SetUniform1i("u_Texture", 0);     //Set the color Uniform
        TextureBuffer[(int)winOrLose]->Bind();
        renderer.Draw(TextureBuffer[(int)winOrLose]->GetVao(), TextureBuffer[(int)winOrLose]->GetIbo(), TexShader);
    }

}

void Test::HardTest::OnImGuiRender()
{

    ImGui::Begin("Statistics Window!");                         // Create a window called "Hello, world!" and append into it.
    ImGui::SetWindowPos({ 1600.0f ,860.0f }, 0);
    ImGui::SetWindowSize(ImVec2(320, 120), 0);


    ImGui::Text("Cached objects: %d", m_Imgui.CachedObjects);                       // Display some text (you can use a format strings too)
    ImGui::Text("Objects left: %d", m_Imgui.RemainingObjects);                      // Display some text (you can use a format strings too)
    ImGui::Text("Time left: %.0f sec", TimeLeft);

    ImGui::Text("\n\nApplication average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    ImGui::End();

}

void Test::HardTest::LoadObjectUpdateFuntions()
{


    Rings.f_ModelColorUpdate = [&](glm::mat4& model, const glm::vec2& oneVertex, glm::vec4& color, const float& deltaTime, glm::vec3& movement)
    {
        glm::vec4 pos(oneVertex[0], oneVertex[1], 0.0f, 1.0f);
        glm::vec4 updatedOnePos = model * pos;
        if (updatedOnePos[0] < -600.0f) movement.x = 1.0f;
        else if (updatedOnePos[0] > 600.0f) movement.x = -1.0f;
        if (updatedOnePos[1] < -320.0f) movement.y = 1.0f;
        else if (updatedOnePos[1] > 320.0f) movement.y = -1.0f;
        model = glm::translate(model, glm::vec3(deltaTime * movement.x * 2, deltaTime * movement.y * 2, 0.0f));
    };


    Horse.f_ModelColorUpdate = [&](glm::mat4& model, const glm::vec2& oneVertex, glm::vec4& color, const float& deltaTime, glm::vec3& movement)
    {
        glm::vec4 pos(oneVertex[0], oneVertex[1], 0.0f, 1.0f);
        glm::vec4 updatedOnePos = model * pos;
        if (updatedOnePos[0] < -580.0f) movement.x = 1.0f;
        else if (updatedOnePos[0] > 580.0f) movement.x = -1.0f;
        if (updatedOnePos[1] < -300.0f) movement.y = 1.0f;
        else if (updatedOnePos[1] > 300.0f) movement.y = -1.0f;
        model = glm::translate(model, glm::vec3(deltaTime* movement.x * 2, deltaTime * movement.y * 2, 0.0f));
    };

    Star.f_ModelColorUpdate = [&](glm::mat4& model, const glm::vec2& oneVertex, glm::vec4& color, const float& deltaTime, glm::vec3& movement)
    {
        //movement.z += deltaTime/25;
        model = glm::translate(model, glm::vec3(0.0f, deltaTime * 7, 0.0f));
        model = glm::rotate(model, deltaTime/25, glm::vec3(0.0f, 0.0f, 1.0f));

    };

    CQuad.f_ModelColorUpdate = [&](glm::mat4& model, const glm::vec2& oneVertex, glm::vec4& color, const float& deltaTime, glm::vec3& movement)
    {
        if (Star.GetCollisionStatus() || Horse.GetCollisionStatus() || Rings.GetCollisionStatus())
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
}



void Test::HardTest::RegisterTexture(TextureObject* Texture)
{
    TextureBuffer.push_back(Texture);
}

void Test::HardTest::LoadNewObjects(const float& TimeLeft)
{
    if (TimeLeft < 55.0f && newObjectsSelector[0])
    {
        Rings.New(glm::translate(glm::mat4(1.0f), glm::vec3(-300.0f, -100.0f, 0.0f)), {-1.0f, -1.0f, 1.0f});
        newObjectsSelector[0] = false;
    }

    if (TimeLeft < 45.0f && newObjectsSelector[1])
    {
        Rings.New(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -300.0f, 0.0f)));
        Rings.New(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -200.0f, 0.0f)),{ -1.0f, 1.0f, 1.0f });
        newObjectsSelector[1] = false;
    }

    if (TimeLeft < 35.0f && newObjectsSelector[2])
    {
        Rings.New(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -300.0f, 0.0f)));
        Rings.New(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -300.0f, 0.0f)));
        Star.New(glm::translate(glm::mat4(1.0f), glm::vec3(50.0f, -100.0f, 0.0f)), { -1.0f, 1.0f, 1.0f });
        newObjectsSelector[2] = false;
    }

    if (TimeLeft < 25.0f && newObjectsSelector[3])
    {
        Rings.New(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -300.0f, 0.0f)));
        Horse.New(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 300.0f, -200.0f)));
        Horse.New(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 000.0f, 200.0f)));
        Star.New(glm::translate(glm::mat4(1.0f), glm::vec3(-300.0f, 0.0f, 0.0f)));
        newObjectsSelector[3] = false;
    }

    if (TimeLeft < 15.0f && newObjectsSelector[4])
    {
        Horse.New(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -300.0f, 0.0f)));
        newObjectsSelector[4] = false;
    }

    if (TimeLeft < 13.0f && newObjectsSelector[5])
    {
        Horse.New(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -200.0f, 0.0f)));
        newObjectsSelector[5] = false;
    }

}