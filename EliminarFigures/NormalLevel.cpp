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

Level::NormalLevel::NormalLevel()
    
{
    //Loading the objects of the current level:
    LoadObjectFiles();
    BuildObjects();

    glEnable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);    
    glDepthFunc(GL_LESS);
    //glEnable(GL_CULL_FACE);
    //glCullFace(GL_BACK);

    //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    std::cout << "[NORMAL LEVEL]: Default Level created. " << std::endl;

    

}


Level::NormalLevel::~NormalLevel()
{
    std::cout << "[NORMAL LEVEL]: Default Level destoyed. " << std::endl;
}

void Level::NormalLevel::OnUpdate(float deltaTime, bool& testExit)
{
    ImguiVariables random;
    for (auto& object : worldBuffer)
    {
        object.OnObjectUpdate(false, 0, random);
    }
}

void Level::NormalLevel::OnRender()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    for (auto& object : worldBuffer)
    {
        renderer.Draw(object, m_Proj, m_View);                  //Each pass will draw all the objects of the current shape object
    }
}

void Level::NormalLevel::OnImGuiRender()
{
       
}


void Level::NormalLevel::LoadObjectFiles()
{
    //We define the objects that we want to load:
    std::vector<std::string> filenames = { "teapot.obj" };
    //objectReader.loadObjectFiles(filenames, "/res/obj/");

    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string errorMessage;

    std::string basepath = "res/obj/";
    std::string inputfile = basepath + "teapot.obj";

    bool ret = tinyobj::LoadObj(shapes, materials, errorMessage, inputfile.c_str(), basepath.c_str());
    if (!ret) std::cout << "error: " << errorMessage << std::endl;
    std::cout << "shapes size: " << shapes.size();
    worldBuffer.emplace_back("/res/obj/teapot.obj", shapes[0]);

}

/* It will create all the available objects to be rendered in the current level, directly in the world vector*/
void Level::NormalLevel::BuildObjects()
{
    //objectReader.buildObjects(worldBuffer);
}

