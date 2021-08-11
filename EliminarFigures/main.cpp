/*
Timer class, Deleating objects with time(?) No with key
vector of model matrices so we can duplicate same object
*/

#include <fstream>
#include <sstream>
#include <string>

#include "VertexBuffer.h"
#include "VertexArray.h"
#include "VertexArrayLayout.h"
#include "Renderer.h"

#include "HardTest.h"
#include "EasyTest.h"
#include "ObjParser.h"

enum class Difficulty {
    EASY = 1,
    NORMAL = 2,
    HARD = 3
};

float deltaTime()
{
    return 0;
}


int main(void)
{

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);


    GLFWwindow* window;
    Renderer renderer;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(1280, 720, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    if (GLEW_OK != glewInit()) 
    {
        std::cout << "Glew could not create a valid context!" << std::endl;
    }
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.DisplaySize = ImVec2(100, 600);
    ImGui::StyleColorsDark();

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 130");

    std::cout << "[GLEW]: Context succesfully created" << std::endl;

    Test::Test* currentTest = nullptr;
    Test::TestMenu* menu = new Test::TestMenu(currentTest);
    currentTest = menu; 

    menu->RegisterTest<Test::HardTest>("Hard Difficulty");
    menu->RegisterTest<Test::HardTest>("Medium Difficulty");
    menu->RegisterTest<Test::EasyTest>("Easy Difficulty");

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        renderer.Clear();

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        currentTest->SaveWindow(window);
        currentTest->OnUpdate(0.0f);
        currentTest->OnRender();
        ImGui::Begin("HARD DIFFICULTY");
        if (currentTest != menu && ImGui::Button("<-"))
        {
           delete currentTest;
           currentTest = menu;
        }
        currentTest->OnImGuiRender();
        ImGui::End();
        
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());


        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);

    glfwTerminate();
    return 0;
}


