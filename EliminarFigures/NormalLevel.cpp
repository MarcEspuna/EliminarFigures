#include "Level.h"
#include "NormalLevel.h"

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

Level::NormalLevel::NormalLevel()
    : shader("res/Basic.shader"), TexShader("res/TexBasic.shader")
{
    //Registering shapes in the current level
    RegisterObjects();

    glEnable(GL_BLEND);
    //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    std::cout << "[NORMAL LEVEL]: Default Level created. " << std::endl;

    

}


Level::NormalLevel::~NormalLevel()
{
    std::cout << "[NORMAL LEVEL]: Default Level destoyed. " << std::endl;
}

void Level::NormalLevel::OnUpdate(float deltaTime, bool& testExit)
{
    
}

void Level::NormalLevel::OnRender()
{
    for (auto& object : worldBuffer)
    {
        renderer.Draw(object, m_Proj, m_View);                  //Each pass will draw all the objects of the current shape object
    }
}

void Level::NormalLevel::OnImGuiRender()
{

}

/* It will create all the available objects to be rendered in the current level, directly in the world vector*/
void Level::NormalLevel::RegisterObjects()
{
    worldBuffer.reserve(5);
    worldBuffer.emplace_back("res/obj/Star.obj", glm::vec4(0.1f, 0.1f, 1.0f, 1.0f), 0.5f);          //Triangle_3D
    worldBuffer.emplace_back("res/obj/Covid.obj", glm::vec4(0.1f, 0.1f, 1.0f, 1.0f), 0.5f);         //Square_3D
    worldBuffer.emplace_back("res/obj/Star.obj", glm::vec4(0.1f, 0.1f, 1.0f, 1.0f), 0.5f);          //Octahedron_3D
    worldBuffer.emplace_back("res/obj/Star.obj", glm::vec4(0.1f, 0.1f, 1.0f, 1.0f), 0.5f);          //Dodecahedron_3D
    worldBuffer.emplace_back("res/obj/Star.obj", glm::vec4(0.1f, 0.1f, 1.0f, 1.0f), 0.5f);          //Icosahedron_3D
}

