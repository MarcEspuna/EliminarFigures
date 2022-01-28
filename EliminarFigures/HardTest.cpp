#include "Level.h"
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

Level::HardTest::HardTest()
    : Horse("res/obj/donut.obj", {0.8, 0.3, 0.6, 1.0f}, 20.0f),
    HLine("res/obj/HLine.obj", { 0.7, 0.1, 0.1, 1.0f }, glm::vec3(1.0f, 0.4f, 1.0f) ),
    VLine("res/obj/VLine.obj", { 0.1, 0.2, 0.7, 1.0f }, glm::vec3(0.4f, 1.0f, 1.0f)),
    CQuad("res/obj/CQuad.obj", { 1.0f, 0.96f, 0.22f, 1.0f }, glm::vec3(0.4f, 0.4f, 1.0f)),
    Star("res/obj/Star.obj", {0.1f, 0.1f, 1.0f, 1.0f}, 0.5f),
    Rings("res/obj/Rings.obj", { 0.3, 0.6, 0.3, 1.0f }, 38.0f),
    Covid("res/obj/Covid.obj", { 0.6, 0.9, 0.6, 1.0f }, 25.0f),
    Satellite("res/obj/Satellite.obj", { 0.5, 0.6, 0.9, 1.0f }, 15.0f),
    tex_GameOver("res/textures/GameOverTransparent.png"),
    tex_YouLose("res/textures/YouLoseTransparent.png", 0.45f, glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -100.0f, 0.0f))),
    tex_YouWin("res/textures/YouWinTransparent.png", 0.50f, glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -100.0f, 0.0f))),
    shader("res/Basic.shader"),
    TexShader("res/TexBasic.shader")
{
    Horse.GetModels()[0] = glm::translate(glm::mat4(1.0f), glm::vec3(random.GetValue(0, 550), random.GetValue(0, 300), 0.0f));
    Rings.GetModels()[0] = glm::translate(glm::mat4(1.0f), glm::vec3(random.GetValue(0, 550), random.GetValue(0, 300), 0.0f));
    Covid.GetModels()[0] = glm::translate(glm::mat4(1.0f), glm::vec3(random.GetValue(0, 550), random.GetValue(0, 300), 0.0f));
    Satellite.GetModels()[0] = glm::translate(glm::mat4(1.0f), glm::vec3(random.GetValue(0, 550), random.GetValue(0, 300), 0.0f));

    //Registering all the objects
    RegisterObject(&Rings);
    RegisterObject(&Horse);
    RegisterObject(&Star);
    RegisterObject(&Satellite);
    RegisterObject(&Covid);

    //After that register cursor
    RegisterObject(&HLine);
    RegisterObject(&VLine);
    RegisterObject(&CQuad);

    //Register textures
    RegisterTexture(&tex_GameOver);
    RegisterTexture(&tex_YouLose);
    RegisterTexture(&tex_YouWin);

    //Adding more of the same already registered objects
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
    Covid.TrackCollisionWith(&CQuad);
    Satellite.TrackCollisionWith(&CQuad);

    glEnable(GL_BLEND);
    //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    std::cout << "Hard Test created" << std::endl;

    

}


static void ayncObjectUpdate(Object* object, float deltaTime, bool CatchingObject, ImguiVariables* imguiVar)
{
    object->OnObjectUpdate(CatchingObject, deltaTime, *imguiVar);
}

Level::HardTest::~HardTest()
{
	std::cout << "Hard test destroyed\n";
}

void Level::HardTest::OnUpdate(float deltaTime, bool& testExit)
{
    Time += deltaTime / 20;                                                                                  //We keep track of the time passed during this test                             
    TimeLeft -= deltaTime / 20;

    int state = glfwGetKey(ptr_window, GLFW_KEY_W);
    if (state == GLFW_PRESS)
    {
        if (!(HLine.GetModels()[0][3][1] > 330.0f)) 
        {
            HLine.GetModels()[0] = glm::translate(HLine.GetModels()[0], glm::vec3(0.0f, deltaTime * 5.5f, 0.0f));
            CQuad.GetModels()[0] = glm::translate(CQuad.GetModels()[0], glm::vec3(0.0f, deltaTime * 5.5f, 0.0f));
        }
    }

    int state1 = glfwGetKey(ptr_window, GLFW_KEY_S);
    if (state1 == GLFW_PRESS)
    {
        if (!(HLine.GetModels()[0][3][1] < -330.0f))                                                        //Not allowing the cursor to go outside of the screen
        {
            HLine.GetModels()[0] = glm::translate(HLine.GetModels()[0], glm::vec3(0.0f, -deltaTime * 5.5f, 0.0f));
            CQuad.GetModels()[0] = glm::translate(CQuad.GetModels()[0], glm::vec3(0.0f, -deltaTime * 5.5f, 0.0f));
        }

    }

    int state2 = glfwGetKey(ptr_window, GLFW_KEY_RIGHT);
    if (state2 == GLFW_PRESS)
    {
        if (!(VLine.GetModels()[0][3][0] > 630.0f))
        {
            VLine.GetModels()[0] = glm::translate(VLine.GetModels()[0], glm::vec3(deltaTime * 5.5f, 0.0f, 0.0f));
            CQuad.GetModels()[0] = glm::translate(CQuad.GetModels()[0], glm::vec3(deltaTime * 5.5f, 0.0f, 0.0f));
        }
    }

    int state3 = glfwGetKey(ptr_window, GLFW_KEY_LEFT);
    if (state3 == GLFW_PRESS)
    {
        if (!(VLine.GetModels()[0][3][0] < -630.0f))
        {
            VLine.GetModels()[0] = glm::translate(VLine.GetModels()[0], glm::vec3(-deltaTime * 5.5f, 0.0f, 0.0f));
            CQuad.GetModels()[0] = glm::translate(CQuad.GetModels()[0], glm::vec3(-deltaTime * 5.5f, 0.0f, 0.0f));
        }
    }

    int state4 = glfwGetKey(ptr_window, GLFW_KEY_SPACE);
    int state5 = glfwGetKey(ptr_window, GLFW_KEY_ENTER);
    if (state4 == GLFW_PRESS && state5 == GLFW_PRESS)
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

void Level::HardTest::OnRender()
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

void Level::HardTest::OnImGuiRender()
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

void Level::HardTest::LoadObjectUpdateFuntions()
{


    Rings.f_ModelColorUpdate = [&](glm::mat4& model, const glm::vec2& oneVertex, glm::vec4& color, const float& deltaTime, glm::vec3& movement)
    {
        glm::vec4 pos(oneVertex[0], oneVertex[1], 0.0f, 1.0f);
        glm::vec4 updatedOnePos = model * pos;
        if (updatedOnePos[0] < -600.0f) movement.x = 1.0f;
        if (updatedOnePos[0] > 600.0f) movement.x = -1.0f;
        if (updatedOnePos[0] < 120.0f && updatedOnePos[0] > 0.0f) movement.x = 1.0f;
        if (updatedOnePos[0] > -120.0f && updatedOnePos[0] < 0.0f) movement.x = -1.0f;
        if ((updatedOnePos[1] < 100.0f) && (updatedOnePos[1] > 0.0f)) movement.y = 1.0f;
        if (updatedOnePos[1] > 320.0f) movement.y = -1.0f;
        if (updatedOnePos[1] > -100.0f && updatedOnePos[1] < 0.0f) movement.y = -1.0f;
        if (updatedOnePos[1] < -320.0f) movement.y = 1.0f;
        model = glm::translate(model, glm::vec3(deltaTime * movement.x * 2, deltaTime * movement.y * 2, 0.0f));
    };


    Horse.f_ModelColorUpdate = [&](glm::mat4& model, const glm::vec2& oneVertex, glm::vec4& color, const float& deltaTime, glm::vec3& movement)
    {

        if (movement.z == 0.0f) movement.z = 1 / (1 + deltaTime / 120.0f);
        glm::vec4 pos(oneVertex[0], oneVertex[1], 0.0f, 1.0f);
        glm::vec4 updatedOnePos = model * pos;
        if (updatedOnePos[0] < -600.0f) movement.x = 1.0f;
        if (updatedOnePos[0] > 600.0f) movement.x = -1.0f;
        if (updatedOnePos[0] < 120.0f && updatedOnePos[0] > 0.0f) movement.x = 1.0f;
        if (updatedOnePos[0] > -120.0f && updatedOnePos[0] < 0.0f) movement.x = -1.0f;
        if ((updatedOnePos[1] < 100.0f) && (updatedOnePos[1] > 0.0f)) movement.y = 1.0f;
        if (updatedOnePos[1] > 320.0f) movement.y = -1.0f;
        if (updatedOnePos[1] > -100.0f && updatedOnePos[1] < 0.0f) movement.y = -1.0f;
        if (updatedOnePos[1] < -320.0f) movement.y = 1.0f;
        float scalingFactor = sqrt(model[0][0] * model[0][0] + model[1][0] * model[1][0] + model[2][0] * model[2][0]);
        if (scalingFactor > 1.5) movement.z = 1/(1 + deltaTime/120.0f);
        if (scalingFactor < 0.5) movement.z = (1 + deltaTime/150.0f);
        model = glm::translate(model, glm::vec3(deltaTime* movement.x * 2, deltaTime * movement.y * 2, 0.0f));
        model = glm::scale(model, glm::vec3(movement.z, movement.z, 1.0f));
    };

    Star.f_ModelColorUpdate = [&](glm::mat4& model, const glm::vec2& oneVertex, glm::vec4& color, const float& deltaTime, glm::vec3& movement)
    {

        model = glm::translate(model, glm::vec3(0.0f, deltaTime * 7, 0.0f));
        model = glm::rotate(model, deltaTime/25, glm::vec3(0.0f, 0.0f, 1.0f));

    };

    Satellite.f_ModelColorUpdate = [&](glm::mat4& model, const glm::vec2& oneVertex, glm::vec4& color, const float& deltaTime, glm::vec3& movement)
    {
        glm::vec4 pos(oneVertex[0], oneVertex[1], 0.0f, 1.0f);
        glm::vec4 updatedOnePos = model * pos;
        if (updatedOnePos[0] < -600.0f) movement.x = 1.0f;
        if (updatedOnePos[0] > 600.0f) movement.x = -1.0f;
        if (updatedOnePos[0] < 120.0f && updatedOnePos[0] > 0.0f) movement.x = 1.0f;
        if (updatedOnePos[0] > -120.0f && updatedOnePos[0] < 0.0f) movement.x = -1.0f;
        if ((updatedOnePos[1] < 100.0f) && (updatedOnePos[1] > 0.0f)) movement.y = 1.0f;
        if (updatedOnePos[1] > 320.0f) movement.y = -1.0f;
        if (updatedOnePos[1] > -100.0f && updatedOnePos[1] < 0.0f) movement.y = -1.0f;
        if (updatedOnePos[1] < -320.0f) movement.y = 1.0f;
        model = glm::translate(model, glm::vec3(deltaTime * movement.x * 2, deltaTime * movement.y * 2, 0.0f));

    };

    Covid.f_ModelColorUpdate = [&](glm::mat4& model, const glm::vec2& oneVertex, glm::vec4& color, const float& deltaTime, glm::vec3& movement)
    {
        glm::vec4 pos(oneVertex[0], oneVertex[1], 0.0f, 1.0f);
        glm::vec4 updatedOnePos = model * pos;
        if (updatedOnePos[0] < -600.0f) movement.x = 1.0f;
        if (updatedOnePos[0] > 600.0f) movement.x = -1.0f;
        if (updatedOnePos[0] < 120.0f && updatedOnePos[0] > 0.0f) movement.x = 1.0f;
        if (updatedOnePos[0] > -120.0f && updatedOnePos[0] < 0.0f) movement.x = -1.0f;
        if ((updatedOnePos[1] < 100.0f) && (updatedOnePos[1] > 0.0f)) movement.y = 1.0f;
        if (updatedOnePos[1] > 320.0f) movement.y = -1.0f;
        if (updatedOnePos[1] > -100.0f && updatedOnePos[1] < 0.0f) movement.y = -1.0f;
        if (updatedOnePos[1] < -320.0f) movement.y = 1.0f;
        model = glm::translate(model, glm::vec3(deltaTime * movement.x * 2, deltaTime * movement.y * 2, 0.0f));

    };


    CQuad.f_ModelColorUpdate = [&](glm::mat4& model, const glm::vec2& oneVertex, glm::vec4& color, const float& deltaTime, glm::vec3& movement)
    {
        if (Star.GetCollisionStatus() || Horse.GetCollisionStatus() || Rings.GetCollisionStatus() || Covid.GetCollisionStatus() || Satellite.GetCollisionStatus())
        {
            color = glm::vec4(1.0f, 0.0f, 0.0f, 0.0f);
        }
        else
        {
            color = glm::vec4(0.0f, 1.0f, 0.7f, 0.0f);
        }

    };

}

void Level::HardTest::RegisterObject(Object* object)
{
    WorldBuffer.push_back(object);
}



void Level::HardTest::RegisterTexture(TextureObject* Texture)
{
    TextureBuffer.push_back(Texture);
}

void Level::HardTest::LoadNewObjects(const float& TimeLeft)
{
    if (TimeLeft < 65.0f && newObjectsSelector[0])
    {
        random.Randomize();
        Rings.New(glm::translate(glm::mat4(1.0f), glm::vec3(random.GetValue(0, 550), random.GetValue(0, 300), 0.0f)), { random.GetValue(1, 2), random.GetValue(1, 2), 1.0f });
        Horse.New(glm::translate(glm::mat4(1.0f), glm::vec3(random.GetValue(0, 550), random.GetValue(0, 300), 0.0f)), { random.GetValue(1, 2), random.GetValue(1, 2), 0.0f });
        Satellite.New(glm::translate(glm::mat4(1.0f), glm::vec3(random.GetValue(0, 550), random.GetValue(0, 300), 0.0f)), { random.GetValue(0, 2), random.GetValue(0, 2), 1.0f });
        newObjectsSelector[0] = false;
    }


    if (TimeLeft < 55.0f && newObjectsSelector[1])
    {
        random.Randomize();
        Rings.New(glm::translate(glm::mat4(1.0f), glm::vec3(random.GetValue(0, 550), random.GetValue(0, 300), 0.0f)), { random.GetValue(1, 2), random.GetValue(1, 2), 1.0f});
        Covid.New(glm::translate(glm::mat4(1.0f), glm::vec3(random.GetValue(0, 550), random.GetValue(0, 300), 0.0f)), { random.GetValue(0, 2), random.GetValue(0, 2), 1.0f });
        Covid.New(glm::translate(glm::mat4(1.0f), glm::vec3(random.GetValue(0, 550), random.GetValue(0, 300), 0.0f)), { random.GetValue(0, 2), random.GetValue(0, 2), 1.0f });
        Covid.New(glm::translate(glm::mat4(1.0f), glm::vec3(random.GetValue(0, 550), random.GetValue(0, 300), 0.0f)), { random.GetValue(0, 2), random.GetValue(0, 2), 1.0f });
        newObjectsSelector[1] = false;
    }

    if (TimeLeft < 45.0f && newObjectsSelector[2])
    {
        random.Randomize();
        Rings.New(glm::translate(glm::mat4(1.0f), glm::vec3(random.GetValue(0, 550), random.GetValue(0, 300), 0.0f)), { random.GetValue(0, 2), random.GetValue(1, 2), 1.0f });
        Rings.New(glm::translate(glm::mat4(1.0f), glm::vec3(random.GetValue(0, 550), random.GetValue(0, 300), 0.0f)), { random.GetValue(0, 2), random.GetValue(1, 2), 1.0f });
        newObjectsSelector[2] = false;
    }

    if (TimeLeft < 35.0f && newObjectsSelector[3])
    {
        random.Randomize();
        Rings.New(glm::translate(glm::mat4(1.0f), glm::vec3(random.GetValue(0, 550), random.GetValue(0, 300), 0.0f)), { random.GetValue(0, 2), random.GetValue(1, 2), 1.0f });
        Rings.New(glm::translate(glm::mat4(1.0f), glm::vec3(random.GetValue(0, 550), random.GetValue(0, 300), 0.0f)), { random.GetValue(1, 2), random.GetValue(0, 2), 1.0f });
        newObjectsSelector[3] = false;
    }

    if (TimeLeft < 25.0f && newObjectsSelector[4])
    {
        random.Randomize();
        Rings.New(glm::translate(glm::mat4(1.0f), glm::vec3(random.GetValue(0, 550), random.GetValue(0, 300), 0.0f)), { random.GetValue(0, 2), random.GetValue(0, 2), 1.0f });
        Horse.New(glm::translate(glm::mat4(1.0f), glm::vec3(random.GetValue(0, 550), random.GetValue(0, 300), -200.0f)), { random.GetValue(0, 2), random.GetValue(0, 2), 0.0f });
        Horse.New(glm::translate(glm::mat4(1.0f), glm::vec3(random.GetValue(0, 550), random.GetValue(0, 300), 200.0f)), { random.GetValue(0, 2), random.GetValue(0, 2), 0.0f });
        Star.New(glm::translate(glm::mat4(1.0f), glm::vec3(random.GetValue(0, 300), 0.0f, 0.0f)));
        newObjectsSelector[4] = false;
    }

    if (TimeLeft < 15.0f && newObjectsSelector[5])
    {
        random.Randomize();
        Star.New(glm::translate(glm::mat4(1.0f), glm::vec3(random.GetValue(0, 300), 0.0f, 0.0f)), { 1.0f, 1.0f, 1.0f });
        Horse.New(glm::translate(glm::mat4(1.0f), glm::vec3(random.GetValue(0, 550), random.GetValue(0, 300), 0.0f)), { random.GetValue(0, 2), random.GetValue(0, 2), 0.0f });
        newObjectsSelector[5] = false;
    }

    if (TimeLeft < 13.0f && newObjectsSelector[6])
    {
        random.Randomize();
        Horse.New(glm::translate(glm::mat4(1.0f), glm::vec3(random.GetValue(0, 550), random.GetValue(0, 300), 0.0f)), { random.GetValue(0, 2), random.GetValue(0, 2), 0.0f });
        newObjectsSelector[6] = false;
    }

}