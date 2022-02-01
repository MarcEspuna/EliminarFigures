#include "Level.h"
#include "NormalLevel.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Shader.h"
#include "VertexArrayLayout.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_opengl3.h"
#include "imgui/imgui_impl_glfw.h"

#include "Texture.h"
#include <tuple>

Level::NormalLevel::NormalLevel()
    : cursor({ new BasicObject("res/obj/HLine.obj", glm::vec4(0.7, 0.1, 0.1, 1.0f), glm::vec3(1.0f, 0.4f, 1.0f)),
               new BasicObject("res/obj/VLine.obj", glm::vec4(0.1, 0.2, 0.7, 1.0f), glm::vec3(0.4f, 1.0f, 1.0f)),
               new BasicObject("res/obj/CQuad.obj", glm::vec4(1.0f, 0.96f, 0.22f, 1.0f), glm::vec3(0.4f, 0.4f, 1.0f))})
{
    //Loading the objects of the current level:
    LoadObjectFiles();
    BuildObjects();
    glEnable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);    
    glDepthFunc(GL_LESS);
    std::cout << "[NORMAL LEVEL]: Default Level created. " << std::endl;
}


Level::NormalLevel::~NormalLevel()
{
    for (int i = 0; i < worldBuffer.size(); i++)
    {
        delete worldBuffer[i];
    }
    delete cursor.CQuad;
    delete cursor.HLine;
    delete cursor.VLine;
    std::cout << "[NORMAL LEVEL]: Default Level destoyed. " << std::endl;
    glDisable(GL_DEPTH_TEST);
}

void Level::NormalLevel::OnUpdate(float deltaTime, bool& testExit)
{
    ImguiVariables random;
    for (auto& object : worldBuffer)
    {
        object->OnObjectUpdate(false, 0, random);
        object->UpdateCollisionWith(cursor.CQuad);
    }

    updateCursor(deltaTime);

}

void Level::NormalLevel::OnRender()
{
    renderer.ClearDepth();
    //Render all the world space objects:
    for (auto& object : worldBuffer)
    {
        renderer.Draw(*object, m_Proj, m_View);                  //Each pass will draw all the objects of the current shape object
    }
    glDisable(GL_DEPTH_TEST);
    //Render the user cursor:
    renderer.Draw(*cursor.VLine, m_Proj, m_View);
    renderer.Draw(*cursor.HLine, m_Proj, m_View);
    renderer.Draw(*cursor.CQuad, m_Proj, m_View);
    glEnable(GL_DEPTH_TEST);
}

void Level::NormalLevel::OnImGuiRender()
{
       
}


void Level::NormalLevel::LoadObjectFiles()
{
    //We define the objects that we want to load:
    std::vector<ObjectArguments> objectArguments = { 
        {"teapot.obj", ObjectType::LIGHT_OBJECT, glm::scale(glm::mat4(1.0f), glm::vec3(100.0f,100.0f,100.0f))},
        {"Icosphere.obj", ObjectType::LIGHT_OBJECT, glm::scale(glm::translate(glm::mat4(1.0f), glm::vec3(-400.0f,0.0f,0.0f)),  glm::vec3(80.0f,80.0f,80.0f))},
        {"square.obj", ObjectType::LIGHT_OBJECT, glm::scale(glm::translate(glm::mat4(1.0f), glm::vec3(400.0f,0.0f,0.0f)),  glm::vec3(25.0f,25.0f,25.0f))},
        {"bunny.obj", ObjectType::LIGHT_OBJECT, glm::scale(glm::translate(glm::mat4(1.0f), glm::vec3(400.0f,200.0f,0.0f)), glm::vec3(600.0f,600.0f,600.0f))},
        {"torus.obj", ObjectType::LIGHT_OBJECT, glm::scale(glm::translate(glm::mat4(1.0f), glm::vec3(-400.0f,200.0f,0.0f)),  glm::vec3(40.0f,40.0f,40.0f))}
    };
    objectReader.loadObjectFiles(objectArguments, "res/obj/");
}

/* It will create all the available objects to be rendered in the current level, in other words, we add the objects in the worldbuffer*/
void Level::NormalLevel::BuildObjects()
{
    objectReader.buildObjects(worldBuffer);
}

void Level::NormalLevel::updateCursor(const float& deltaTime)
{
    int state = glfwGetKey(ptr_window, GLFW_KEY_W);
    if (state == GLFW_PRESS)
    {
        cursor.CQuad->moveUP(deltaTime, 6.0f);
        cursor.HLine->moveUP(deltaTime, 6.0f);
    }

    int state1 = glfwGetKey(ptr_window, GLFW_KEY_S);
    if (state1 == GLFW_PRESS)
    {
        cursor.CQuad->moveDown(deltaTime, 6.0f);
        cursor.HLine->moveDown(deltaTime, 6.0f);
    }

    int state2 = glfwGetKey(ptr_window, GLFW_KEY_RIGHT);
    if (state2 == GLFW_PRESS)
    {
        cursor.CQuad->moveRight(deltaTime, 6.0f);
        cursor.VLine->moveRight(deltaTime, 6.0f);
    }

    int state3 = glfwGetKey(ptr_window, GLFW_KEY_LEFT);
    if (state3 == GLFW_PRESS)
    {
        cursor.CQuad->moveLeft(deltaTime, 6.0f);
        cursor.VLine->moveLeft(deltaTime, 6.0f);
    }

}




