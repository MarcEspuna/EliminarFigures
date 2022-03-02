#include "Level.h"
#include "NormalLevel.h"
#include "DataLink.h"
#include <thread>

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
#include "Slot.h"
#include <condition_variable>

static std::mutex s_XuserMutex;
static std::condition_variable s_Xcv;

Level::NormalLevel::NormalLevel(bool playerXAI, bool playerYAI)
    : cursor({ new BasicObject("res/obj/HLine.obj", glm::vec4(0.7, 0.1, 0.1, 1.0f), glm::vec3(1.0f, 0.4f, 1.0f)),
               new BasicObject("res/obj/VLine.obj", glm::vec4(0.1, 0.2, 0.7, 1.0f), glm::vec3(0.4f, 1.0f, 1.0f)),
               new BasicObject("res/obj/CQuad.obj", glm::vec4(1.0f, 0.96f, 0.22f, 1.0f), glm::vec3(0.4f, 0.4f, 1.0f))}),
               x_AiEnabled(playerXAI), y_AiEnabled(playerYAI),
               cursorUpdaterX(nullptr),
               cursorUpdaterY(nullptr),
               levelActive(true)
{
    //Loading the objects of the current level:
    LoadObjectFiles();
    BuildObjects();
    LoadConfig();
    loadCommunications();
    loadThread();
    std::cout << "[NORMAL LEVEL]: Default Level created. " << std::endl;
}

Level::NormalLevel::~NormalLevel()
{
    levelActive = false;
    for (int i = 0; i < worldBuffer.size(); i++)
    {
        delete worldBuffer[i];
    }
    delete cursor.CQuad;
    delete cursor.HLine;
    delete cursor.VLine;
    cursorUpdaterX->join();
    cursorUpdaterY->join();
    delete cursorUpdaterX;
    delete cursorUpdaterY;
    std::cout << "[NORMAL LEVEL]: Default Level destoyed. " << std::endl;
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_BLEND);
}

void Level::NormalLevel::OnUpdate(float deltaTime, bool& testExit)
{
    ImguiVariables random;
    size_t newLevel = 0;
    float minDistance = 9999999.0f;
    Object* closestObject = nullptr;

    for (auto& object : worldBuffer)
    {
        object->isNotTarget();
        float distance = object->UpdateCollisionWith(cursor.CQuad);
        object->OnObjectUpdate(userHitKey(), deltaTime, random);
        newLevel += object->size();

        if (distance < minDistance && object->size() > 0)
        {
            closestObject = object;
            minDistance = distance;
        }
    }

    if (closestObject) 
    { 
        closestObject->isTarget(); 
        //closestObject->updateLink(dataLink);
        //dataLink.setTargetObject(closestObject->getId());
    }
    //dataLink.setRemainingFigures(newLevel);
    if (!newLevel) { createNewLevel(); }
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

void Level::NormalLevel::OnImGuiRender(GLFWwindow* window)
{
       
}

void Level::NormalLevel::SaveWindow(GLFWwindow* window)
{
    ptr_window = window;
    s_Xcv.notify_one();     //We notify the waiting threads to check it's waiting condition
}


void Level::NormalLevel::LoadObjectFiles()
{
    Object::init();   //Reset the counter of objects to 0, in order to propoerly set the object ids
    //We define the objects that we want to load:
    std::vector<ObjectArguments> objectArguments = { 
        {"teapot.obj", ObjectType::LIGHT_OBJECT, slot[14], 100.0f},
        {"Icosphere.obj", ObjectType::LIGHT_OBJECT, slot[9], 80.0f},
        {"square.obj", ObjectType::LIGHT_OBJECT, slot[11], 35.0f},
        {"bunny.obj", ObjectType::LIGHT_OBJECT, slot[6], 600.0f},
        {"torus.obj", ObjectType::LIGHT_OBJECT, slot[17], 40.0f}
    };
    objectReader.loadObjectFiles(objectArguments, "res/obj/");
}

/* It will create all the available objects to be rendered in the current level, in other words, we add the objects in the worldbuffer*/
void Level::NormalLevel::BuildObjects()
{
    objectReader.buildObjects(worldBuffer);
}

bool Level::NormalLevel::userHitKey()
{
    bool userX = false;
    bool userY = false;

    int state1 = glfwGetKey(ptr_window, GLFW_KEY_ENTER);
    if (state1 == GLFW_PRESS)
    {
        userX = true;
    } 

    int state2 = glfwGetKey(ptr_window, GLFW_KEY_SPACE);
    if (state2 == GLFW_PRESS)
    {
        userY = true;
    }
    userPressedKey = (x_AiEnabled && y_AiEnabled) || (y_AiEnabled && userX) || (x_AiEnabled && userY) || (userX && userY);
    return userPressedKey;
}

void Level::NormalLevel::createNewLevel()
{
    slot.clear();
    for (auto& object : worldBuffer)
    {
        object->New(random, slot);
    }
}

void Level::NormalLevel::LoadConfig()
{
    glEnable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
}


void Level::NormalLevel::doAiXInput()
{
    float deltaTime = 0;
    //We need to wait for the window to be passed int before calling the opengl functions
    while (levelActive)
    {
        Timer time(deltaTime);
        glm::vec3 aiInput = aiInterface.getAiInput();
        if (aiInput.x == 1)
        {
            cursor.CQuad->moveRight(deltaTime, 6.0f);
            cursor.VLine->moveRight(deltaTime, 6.0f);
        }
        else if (aiInput.x == -1)
        {
            cursor.CQuad->moveLeft(deltaTime, 6.0f);
            cursor.VLine->moveLeft(deltaTime, 6.0f);
        }
    }
}


void Level::NormalLevel::doAiYInput()
{
    float deltaTime = 0;
    //We need to wait for the window to be passed int before calling the opengl functions
    std::unique_lock<std::mutex> lk(s_XuserMutex);
    s_Xcv.wait(lk, [&] {if (ptr_window) return true; });
    while (levelActive)
    {
        Timer time(deltaTime);
        glm::vec3 aiInput = aiInterface.getAiInput();
        if (aiInput.y == 1)
        {
            cursor.CQuad->moveUP(deltaTime, 6.0f);
            cursor.HLine->moveUP(deltaTime, 6.0f);
            //dataLink.cursorUp();
        }
        else if (aiInput.y == -1)
        {
            cursor.CQuad->moveDown(deltaTime, 6.0f);
            cursor.HLine->moveDown(deltaTime, 6.0f);
            //dataLink.cursorDown();
        }
        else
        {
            //dataLink.aiCursorStoped();
        }
    }
}


void Level::NormalLevel::doUserXInput()
{
    float deltaTime = 0;
    std::unique_lock<std::mutex> lk(s_XuserMutex);
    s_Xcv.wait(lk, [&] {if (ptr_window) return true;});
    while (levelActive)
    {
        Timer time(deltaTime);
        int state2 = glfwGetKey(ptr_window, GLFW_KEY_RIGHT);
        if (state2 == GLFW_PRESS)
        {
            cursor.CQuad->moveRight(deltaTime, 6.0f);
            cursor.VLine->moveRight(deltaTime, 6.0f);
            //dataLink.cursorRight();
        }

        int state3 = glfwGetKey(ptr_window, GLFW_KEY_LEFT);
        if (state3 == GLFW_PRESS)
        {
            cursor.CQuad->moveLeft(deltaTime, 6.0f);
            cursor.VLine->moveLeft(deltaTime, 6.0f);
            //dataLink.cursorLeft();
        }

        if (state2 != GLFW_PRESS && state3 != GLFW_PRESS)
        {
            //dataLink.usrCursorStoped();
        }
    }
}


void Level::NormalLevel::doUserYInput()
{
    float deltaTime = 0;
    while (levelActive)
    {
        Timer time(deltaTime);
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
    }
}

void Level::NormalLevel::loadCommunications()
{
    aiInterface.setCursor(cursor.CQuad);
    aiInterface.setUserPressedKey(&userPressedKey);
    aiInterface.setObjects(worldBuffer);
}

void Level::NormalLevel::loadThread()
{
    if (x_AiEnabled)
        cursorUpdaterX = new std::thread(&NormalLevel::doAiXInput, this);
    else
        cursorUpdaterX = new std::thread(&NormalLevel::doUserXInput, this);
    if (y_AiEnabled)
        cursorUpdaterY = new std::thread(&NormalLevel::doAiYInput, this);
    else
        cursorUpdaterY = new std::thread(&NormalLevel::doUserYInput, this);
}





