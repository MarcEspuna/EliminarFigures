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
    std::cout << "[NORMAL LEVEL]: Default Level destoyed. " << std::endl;
    glDisable(GL_DEPTH_TEST);
}

void Level::NormalLevel::OnUpdate(float deltaTime, bool& testExit)
{
    ImguiVariables random;
    for (auto& object : worldBuffer)
    {
        object->OnObjectUpdate(false, 0, random);
    }
}

void Level::NormalLevel::OnRender()
{
    renderer.ClearDepth();
    for (auto& object : worldBuffer)
    {
        renderer.Draw(*object, m_Proj, m_View);                  //Each pass will draw all the objects of the current shape object
    }
}

void Level::NormalLevel::OnImGuiRender()
{
       
}


void Level::NormalLevel::LoadObjectFiles()
{
    //We define the objects that we want to load:
    std::vector<std::string> filenames = { "teapot.obj" , "Icosphere.obj" };
    std::vector<ObjectArguments> objectArguments = { 
        {"teapot.obj", ObjectType::LIGHT_OBJECT, glm::scale(glm::mat4(1.0f), glm::vec3(100.0f,100.0f,100.0f))},
        {"Icosphere.obj", ObjectType::LIGHT_OBJECT, glm::translate(glm::scale(glm::mat4(1.0f), glm::vec3(80.0f,80.0f,80.0f)), glm::vec3(-5.0f,0.0f,0.0f))},
        {"square.obj", ObjectType::LIGHT_OBJECT, glm::translate(glm::scale(glm::mat4(1.0f), glm::vec3(25.0f,25.0f,25.0f)), glm::vec3(20.0f,0.0f,0.0f))},
        {"bunny.obj", ObjectType::LIGHT_OBJECT, glm::translate(glm::scale(glm::mat4(1.0f), glm::vec3(600.0f,600.0f,600.0f)), glm::vec3(0.25f,0.25f,0.0f))},
        {"monkey.obj", ObjectType::LIGHT_OBJECT, glm::translate(glm::scale(glm::mat4(1.0f), glm::vec3(60.0f,60.0f,60.0f)), glm::vec3(0.0f,-3.0f,0.0f))},
        {"torus.obj", ObjectType::LIGHT_OBJECT, glm::translate(glm::scale(glm::mat4(1.0f), glm::vec3(40.0f,40.0f,40.0f)), glm::vec3(-5.0f,3.0f,0.0f))}
    };

    objectReader.loadObjectFiles(objectArguments, "res/obj/");

}

/* It will create all the available objects to be rendered in the current level, in other words, we add the objects in the worldbuffer*/
void Level::NormalLevel::BuildObjects()
{
    objectReader.buildObjects(worldBuffer);
}

