#include "Renderer.h"

#include "HardTest.h"
#include "EasyTest.h"
#include "MediumTest.h"
#include "NormalLevel.h"


int main(void)
{
    GLFWwindow* window;
    Renderer renderer;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(1920, 1080, "Hello World", NULL, NULL);
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
    ImGui::StyleColorsDark();

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 130");

    std::cout << "[GLEW]: Context succesfully created" << std::endl;

    Level::Level* currentTest = nullptr;
    Level::Menu* menu = new Level::Menu(currentTest);
    currentTest = menu; 

    menu->RegisterTest<Level::HardTest>("Hard Difficulty");
    menu->RegisterTest<Level::MediumTest>("Medium Difficulty");
    menu->RegisterTest<Level::EasyTest>("Easy Difficulty");
    menu->RegisterTest<Level::NormalLevel>("KASPAR");

    //La idea sera seleccionar la AI en una casella (un tic)
    //També seleccionarem l'eix al qual la AI es moura

    float deltaTime = 0;
    bool testExit = false;

    glfwSwapInterval(0);                                                                                    //Remove the FPS cap

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        renderer.ClearColor();
        Timer dTime(deltaTime);
        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        currentTest->SaveWindow(window);
        currentTest->OnUpdate(deltaTime, testExit);
        currentTest->OnRender();
        ImGui::Begin("DIFFICULTY/PLAYERS");
        if ((currentTest != menu && ImGui::Button("<-")) || (currentTest != menu && testExit)) // WoW! si el primer es fals el compilador ni mira la següent comprovació. Per això no apareix el boto al menu principal
        {
           delete currentTest;
           currentTest = menu;
           testExit = false;
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


