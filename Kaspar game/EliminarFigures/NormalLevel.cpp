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

AiInterface Level::Level::aiInterface;

Level::NormalLevel::NormalLevel(const Config::Config& config)
    : cursor({ new BasicObject("res/obj/HLine.obj", glm::vec4(0.7, 0.1, 0.1, 1.0f), glm::vec3(1.0f, 0.4f, 1.0f)),
               new BasicObject("res/obj/VLine.obj", glm::vec4(0.1, 0.2, 0.7, 1.0f), glm::vec3(0.4f, 1.0f, 1.0f)),
               new BasicObject("res/obj/CQuad.obj", glm::vec4(1.0f, 0.96f, 0.22f, 1.0f), glm::vec3(0.4f, 0.4f, 1.0f))}),
               x_AiEnabled(config.aiPlayerX), y_AiEnabled(config.aiPlayerY),
               configuration(config),
               cursorUpdaterX(nullptr),
               cursorUpdaterY(nullptr),
               levelActive(true),
               pressedKeyX(false),
               pressedKeyY(false),
               targetObjectId(0)            // Starting with no target
{
    //Loading the objects of the current level:
    LoadObjectFiles();
    BuildObjects();
    LoadConfig();
    loadCommunications();
    std::cout << "[NORMAL LEVEL]: Default Level created. " << std::endl;
}

Level::NormalLevel::~NormalLevel()
{
    aiInterface.clear();
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
    aiInterface.gameEnd();
    //aiInterface.transmitStatus();                                           // We notify that there are no more objects
    std::cout << "[NORMAL LEVEL]: Default Level destoyed. " << std::endl;
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_BLEND);
}


void Level::NormalLevel::OnUpdate(float deltaTime, bool& testExit)
{
    ImguiVariables random;
    size_t newLevel = 0;

    if (!aiInterface.isConnected()) updateTarget();         // If the ai is not connected the game will set the targets

    for (auto& object : worldBuffer)
    {
        float distance = object->UpdateCollisionWith(cursor.CQuad);
        object->OnObjectUpdate(pressedKeyX && pressedKeyY, deltaTime, random);
        newLevel += object->size();

        if (object->getId() == targetObjectId)
            object->isTarget();
        else
            object->isNotTarget();
    }

    //dataLink.setRemainingFigures(newLevel);
    if (!newLevel) { createNewLevel(); }
    testExit = usrExitKey();                    // We exit the level if the user presses the exit key
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
    rcInterface.setWindow(window);
    loadThread();               // Loads the controls threads based on players selection
}


void Level::NormalLevel::LoadObjectFiles()
{
    Object::init();   //Reset the counter of objects to 0, in order to propoerly set the object ids
    //We define the objects that we want to load:
    std::vector<ObjectArguments> objectArguments = { 
        {configuration.obj.filenames[0], ObjectArguments::getObjectTypeFromFile(configuration.obj.filenames[0]), slot[3][5], (float)configuration.obj.scale[0], {1.0f, 0.0f, 0.0f, 1.0f}},
        {configuration.obj.filenames[1], ObjectArguments::getObjectTypeFromFile(configuration.obj.filenames[1]), slot[1][2], (float)configuration.obj.scale[1], {1.0f, 1.0f, 1.0f, 1.0f}},
        {configuration.obj.filenames[2], ObjectArguments::getObjectTypeFromFile(configuration.obj.filenames[2]), slot[4][0], (float)configuration.obj.scale[2], {0.0f, 0.0f, 1.0f, 1.0f}},
        {configuration.obj.filenames[3], ObjectArguments::getObjectTypeFromFile(configuration.obj.filenames[3]), slot[2][4], (float)configuration.obj.scale[3], {1.0f, 1.0f, 0.0f, 1.0f}},
        {configuration.obj.filenames[4], ObjectArguments::getObjectTypeFromFile(configuration.obj.filenames[4]), slot[0][3], (float)configuration.obj.scale[4], {0.0f, 1.0f, 0.0f, 1.0f}}
    };
    objectReader.loadObjectFiles(objectArguments);
}

/* It will create all the available objects to be rendered in the current level, in other words, we add the objects in the worldbuffer*/
void Level::NormalLevel::BuildObjects()
{
    objectReader.buildObjects(worldBuffer);     // Creating the read objects in the world buffer
    for (auto& object : worldBuffer)            // Updating the configuration settings
    {
        object->setRotationSpeed(configuration.obj.movement.rotation);
        object->setLightDir({ configuration.obj.light.lightDir[0] * 10, configuration.obj.light.lightDir[1] * 10, configuration.obj.light.lightDir[2] * 10 });
        object->setLightParam(configuration.obj.light.ambient, configuration.obj.light.diffuse, configuration.obj.light.specular, configuration.obj.light.shinnines);
    }
}


void Level::NormalLevel::createNewLevel()
{
    slot.clear();
    for (auto& object : worldBuffer)
    {
        object->New(random, slot);
    }
    targetObjectId = 0;                 // unset the target object
}

void Level::NormalLevel::LoadConfig()
{
    glEnable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
}


void Level::NormalLevel::loadCommunications()
{
    aiInterface.setUserPressedKey(&pressedKeyX);
    aiInterface.setUserSelectKey(&userSelectKeyX);
    aiInterface.setObjects(worldBuffer);
    aiInterface.setCursor(cursor.CQuad);
    aiInterface.gameStart(configuration.obj.description);
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

bool Level::NormalLevel::usrExitKey()
{
    int state2 = glfwGetKey(ptr_window, GLFW_KEY_ESCAPE);
    return state2 == GLFW_PRESS;
}

void Level::NormalLevel::updateTarget()
{
    for (const auto& object : worldBuffer)
    {
        if (userSelectKeyX && object->inLineXWith(*cursor.CQuad)) targetObjectId = object->getId();
        if (userSelectKeyY && object->inLineYWith(*cursor.CQuad)) targetObjectId = object->getId();
    }
}

void Level::NormalLevel::doAiXInput()
{
    float deltaTime = 0;
    //We need to wait for the window to be passed int before calling the opengl functions
    while (levelActive)
    {
        Timer time(deltaTime);
        std::string aiInput = aiInterface.getAiInput();
        if (aiInput[0] == 'R')
        {
            cursor.CQuad->moveRight(deltaTime, 6.0f);
            cursor.VLine->moveRight(deltaTime, 6.0f);
        }
        else if (aiInput[0] == 'L')
        {
            cursor.CQuad->moveLeft(deltaTime, 6.0f);
            cursor.VLine->moveLeft(deltaTime, 6.0f);
        }
        if (aiInterface.isConnected())  targetObjectId = aiInput[3] - '0';      // If the ai is connected we update the targetid
        if (aiInput[2] == 'Y') pressedKeyX = true;
    }
}


void Level::NormalLevel::doAiYInput()
{
    float deltaTime = 0;
    while (levelActive)
    {
        Timer time(deltaTime);
        std::string aiInput = aiInterface.getAiInput();           /* AI input: <x movement, y movement, hit, target id >*/
        if (aiInput[1] == 'U')
        {
            cursor.CQuad->moveUP(deltaTime, 6.0f);
            cursor.HLine->moveUP(deltaTime, 6.0f);
            //dataLink.cursorUp();
        }
        else if (aiInput[1] == 'D')
        {
            cursor.CQuad->moveDown(deltaTime, 6.0f);
            cursor.HLine->moveDown(deltaTime, 6.0f);
            //dataLink.cursorDown();
        }
        else
        {
            //dataLink.aiCursorStoped();
        }
        if (aiInterface.isConnected())  targetObjectId = aiInput[3] - '0';      // If the ai is connected we update the targetid
        if (aiInput[2] == 'Y') pressedKeyY = true;
    }
} 

void Level::NormalLevel::doUserXInput()
{
    float deltaTime = 0;
    //We need to wait for the window to be passed int before calling the opengl functions
    while (levelActive)
    {
        Timer time(deltaTime);
        std::string rcInput = rcInterface.getUsrXInput();
        if (rcInput[1] == 'R')
        {
            cursor.CQuad->moveRight(deltaTime, 6.0f);
            cursor.VLine->moveRight(deltaTime, 6.0f);
        }
        else if (rcInput[1] == 'L')
        {
            cursor.CQuad->moveLeft(deltaTime, 6.0f);
            cursor.VLine->moveLeft(deltaTime, 6.0f);
        }
        // Delete and select keys
        if (rcInput[0] == 'Y') pressedKeyX = true;           // TODO: Split into X and Y
        else                   pressedKeyX = false;
        if (rcInput[2] == 'Y') userSelectKeyX = true;
        else                   userSelectKeyX = false;
    }
}


void Level::NormalLevel::doUserYInput()
{
    float deltaTime = 0;
    while (levelActive)
    {
        Timer time(deltaTime);
        std::string rcInput = rcInterface.getUsrYInput();           /* AI input: <x movement, y movement, hit, target id >*/
        if (rcInput[1] == 'U')
        {
            cursor.CQuad->moveUP(deltaTime, 6.0f);
            cursor.HLine->moveUP(deltaTime, 6.0f);
            //dataLink.cursorUp();
        }
        else if (rcInput[1] == 'D')
        {
            cursor.CQuad->moveDown(deltaTime, 6.0f);
            cursor.HLine->moveDown(deltaTime, 6.0f);
            //dataLink.cursorDown();
        }
        else
        {
            //dataLink.aiCursorStoped();
        }
        // Delete and select keys
        if (rcInput[0] == 'Y') pressedKeyY = true;
        else                   pressedKeyY = false;
        if (rcInput[2] == 'Y') userSelectKeyY = true;
        else                   userSelectKeyY = false;

    }
}




