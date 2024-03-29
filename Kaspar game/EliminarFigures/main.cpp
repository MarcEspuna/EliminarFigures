#include "Renderer.h"

#include "HardTest.h"
#include "EasyTest.h"
#include "MediumTest.h"
#include "NormalLevel.h"

static uint64_t allocations = 0;
static uint64_t deletions = 0;

void* operator new(size_t size)
{
    allocations += size;
    return malloc(size);
}

void operator delete(void* memory, size_t size)
{
    deletions += size;
    free(memory);
}

int main(void)
{
    {
        GLFWwindow* window = nullptr;
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
        ImGui_ImplGlfw_InitForOpenGL(window, true);     // I believe this will link the ImGui events
        ImGui_ImplOpenGL3_Init("#version 130");

        std::cout << "[GLEW]: Context succesfully created" << std::endl;

        Level::Level* currentTest = nullptr;
        Level::Menu* menu = new Level::Menu(currentTest);
        currentTest = menu; 

        menu->RegisterTest<Level::HardTest>("Hard Difficulty");
        menu->RegisterTest<Level::MediumTest>("Medium Difficulty");
        menu->RegisterTest<Level::EasyTest>("Easy Difficulty");
        menu->RegisterTest<Level::NormalLevel>("KASPAR");

        io.Fonts->AddFontFromFileTTF("res/fonts/Roboto-Medium.ttf", 16.0f);      // Default font for the text UI of the app

        //La idea sera seleccionar la AI en una casella (un tic)
        //Tamb� seleccionarem l'eix al qual la AI es moura

        float deltaTime = 0;
        bool testExit = false;

        glfwSwapInterval(0);                                                    //Remove the FPS cap                                                      

        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window))
        {
            renderer.ClearColor();
            Timer dTime(deltaTime);
            // Start the Dear ImGui frame
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            if (currentTest != menu && testExit)
            {
               delete currentTest;
               currentTest = menu;
               testExit = false;
            }

            currentTest->OnUpdate(deltaTime, testExit);
            currentTest->OnRender();

            currentTest->OnImGuiRender(window);
        
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
    }
    
    std::cout << "allocations: " << allocations << std::endl;
    std::cout << "deletions: " << deletions << std::endl;

    return 0;
}


