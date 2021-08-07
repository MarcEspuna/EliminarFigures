#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "VertexArrayLayout.h"
#include "Shader.h"
#include "Renderer.h"
#include "CollisionDetector.h"

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

    // Our state
    bool show_demo_window = true;
    bool show_another_window = false;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);


    std::cout << "[GLEW]: Context succesfully created" << std::endl;

    Figures figures;

    glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
    glm::mat4 proj = glm::ortho(-640.0f, 640.0f, -360.0f, 360.0f);
    glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));

    glm::mat4 u_MVP =  proj * view * model;

    //----------------------NOTA:-------------------------
    //EL TRANSLATION VECTOR ES TROBA EN ELS 3 PRIMERS COMPONENTS DE LA ULTIMA COLUMNA DE LA u_MVP
    std::cout << "Translation vector of MVP: " << u_MVP[3][0] << " " << u_MVP[3][1] << " " << u_MVP[3][2] << std::endl;
    std::cout << "Translation vector of Model: " << model[3][0] << " " << model[3][1] << " " << model[3][2] << std::endl;

    VertexArray vaoH;                   //Vertex Array for the Horizontal Line
    VertexArray vaoV;                   //Vertex Array for the Vertical Line
    VertexArray vaoC;                   //Vertex Array for the Contral Quad

    VertexArray vaoQuad;                    //Vertex Array for a random Quad
    VertexArray vaoStar;                    //Vertex Array for a random Star

    VertexBuffer vboH(controlLines.GetPositionsH(), controlLines.GetCountPositions() * sizeof(float));
    VertexBuffer vboV(controlLines.GetPositionsV(), controlLines.GetCountPositions() * sizeof(float));
    VertexBuffer vboC(controlLines.GetPositionsC(), controlLines.GetCountPositions() * sizeof(float));
    VertexBuffer vboQuad(figures.Square, 8 * sizeof(float));
    VertexBuffer vboStar(figures.Star, sizeof(figures.Star));

    VertexArrayLayout layout;
    layout.Push<float>(2);
    vaoH.AddBuffer(vboH, layout);
    vaoV.AddBuffer(vboV, layout);
    vaoC.AddBuffer(vboC, layout);
    vaoQuad.AddBuffer(vboQuad, layout);
    vaoStar.AddBuffer(vboStar, layout);

    IndexBuffer iboH(controlLines.GetIndexH(), controlLines.GetCountIndexes() * sizeof(unsigned int));
    IndexBuffer iboV(controlLines.GetIndexV(), controlLines.GetCountIndexes() * sizeof(unsigned int));
    IndexBuffer iboC(controlLines.GetIndexC(), controlLines.GetCountIndexes() * sizeof(unsigned int));

    IndexBuffer iboQuad(figures.indexQuad, 6 * sizeof(unsigned int));
    IndexBuffer iboStar(figures.indexStar, sizeof(figures.indexStar));

    Shader shader("res/Basic.shader");
    shader.Bind();
    shader.SetUniform4f("u_Color", 1.0f, 0.5f, 1.0f, 1.0f);

    CollisionDetector collision(controlLines.GetPositionsC(), figures.Square, figures.indexQuad, 6);
    CollisionDetector collision2(controlLines.GetPositionsC(), figures.Star, figures.indexStar, 18);


    float x = 1.0f;
    float y = 1.0f;

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
 
        {
            static float f = 0.0f;
            static int counter = 0;

            ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.
            ImGui::SetWindowSize(ImVec2(320, 180), 0);


            ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)

            ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
            ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color
            ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
            ImGui::Checkbox("Another Window", &show_another_window);

            if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
                counter++;
            ImGui::SameLine();
            ImGui::Text("counter = %d", counter);

            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
            ImGui::End();
        }

        ImGui::Render();
        renderer.Clear();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glm::vec3 translationCenterQuad = { controlLines.modelC[3][0], controlLines.modelC[3][1] , controlLines.modelC[3][2] };


        model = glm::translate(model, glm::vec3(y, x, 0.0f));
        if (model[3][1] > 50) x = -1.0f;
        if (model[3][1] < -300.0f) x = 1.0f;
        if (model[3][0] > 0) y = -1.0f;
        if (model[3][0] < -300.0f) y = 1.0f;
        u_MVP = proj * view * model;
        shader.SetUniform4f("u_Color", 0.5f, 0.0f, 0.3f, 1.0f);
        shader.SetUniform4Mat("u_MVP", u_MVP);
        renderer.Draw(vaoStar, iboStar, shader);
        collision2.Refresh(translationCenterQuad, glm::vec3(model[3][0], model[3][1], model[3][2]));

        u_MVP = proj * view * glm::mat4(1.0f);

        shader.SetUniform4f("u_Color", 1.0f, 0.0f, 0.0f, 1.0f);
        shader.SetUniform4Mat("u_MVP", u_MVP);
        renderer.Draw(vaoQuad, iboQuad, shader);
        collision.Refresh(translationCenterQuad, glm::vec3(0, 0, 0));

        u_MVP = proj * view * controlLines.modelH;


        shader.SetUniform4f("u_Color", 0.5f, 0.0f, 0.0f, 1.0f);
        shader.SetUniform4Mat("u_MVP", u_MVP);
        renderer.Draw(vaoH, iboH, shader);

        u_MVP = proj * view * controlLines.modelV;
        shader.SetUniform4Mat("u_MVP", u_MVP);
        shader.SetUniform4f("u_Color", 0.0f, 0.3f, 0.5f, 1.0f);
        renderer.Draw(vaoV, iboV, shader);

        u_MVP = proj * view * controlLines.modelC;
        shader.SetUniform4Mat("u_MVP", u_MVP);
        if (collision.GetStatus() ||collision2.GetStatus())
            shader.SetUniform4f("u_Color", 0.0f, 1.0f, 0.0f, 1.0f);
        else
            shader.SetUniform4f("u_Color", 0.5f, 0.5f, 0.0f, 1.0f);

        renderer.Draw(vaoC, iboC, shader);

        glfwSetKeyCallback(window, key_callback);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }
    Test::HardTest myFirstTest;

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);

    glfwTerminate();
    return 0;
}


