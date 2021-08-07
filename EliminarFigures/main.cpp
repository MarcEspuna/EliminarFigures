#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


#include "VertexBuffer.h"
#include "VertexArray.h"
#include "VertexArrayLayout.h"
#include "Renderer.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_opengl3.h"
#include "imgui/imgui_impl_glfw.h"

#include "HardTest.h"

enum class Difficulty {
    EASY = 1,
    NORMAL = 2,
    HARD = 3
};


ControlLines controlLines;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{


    if (key == GLFW_KEY_W && (action == GLFW_PRESS || action == GLFW_REPEAT)) 
    {
        controlLines.modelH = glm::translate(controlLines.modelH, glm::vec3(0.0f, 20.0f, 0.0f));
        controlLines.modelC = glm::translate(controlLines.modelC, glm::vec3(0.0f, 20.0f, 0.0f));
    }

    if (key == GLFW_KEY_S && (action == GLFW_PRESS || action == GLFW_REPEAT))
    {
        controlLines.modelH = glm::translate(controlLines.modelH, glm::vec3(0.0f, -20.0f, 0.0f));
        controlLines.modelC = glm::translate(controlLines.modelC, glm::vec3(0.0f, -20.0f, 0.0f));
    }

    if (key == GLFW_KEY_RIGHT && (action == GLFW_PRESS || action == GLFW_REPEAT))
    {
        controlLines.modelV = glm::translate(controlLines.modelV, glm::vec3(20.0f, 00.0f, 0.0f));
        controlLines.modelC = glm::translate(controlLines.modelC, glm::vec3(20.0f, 00.0f, 0.0f));
    }
    
    if (key == GLFW_KEY_LEFT && (action == GLFW_PRESS || action == GLFW_REPEAT))
    {
        controlLines.modelV = glm::translate(controlLines.modelV, glm::vec3(-20.0f, 0.0f, 0.0f));
        controlLines.modelC = glm::translate(controlLines.modelC, glm::vec3(-20.0f, 0.0f, 0.0f));
    }


}

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
    if (GLEW_OK != glewInit()) {
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

    Test::HardTest myFirstTest;

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        renderer.Clear();

        myFirstTest.OnUpdate(0.0f);
        
        myFirstTest.OnRender();

        glfwSetKeyCallback(window, key_callback);
        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        myFirstTest.OnImGuiRender();

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


