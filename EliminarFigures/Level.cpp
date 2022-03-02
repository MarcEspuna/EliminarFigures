#include "Level.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_opengl3.h"
#include "imgui/imgui_impl_glfw.h"
#include <iostream>

Level::Menu::Menu(Level*& currentTest)
	:m_CurrentTest(currentTest), playerXAI(false), playerYAI(false)
{


}

Level::Menu::~Menu()
{


}

void Level::Menu::OnImGuiRender(GLFWwindow* window)
{
	for (auto& test : m_Tests)
	{
		if (ImGui::Button(test.first.c_str())) 
		{
			m_CurrentTest = test.second();
			m_CurrentTest->SaveWindow(window);
		}
	}

	ImGui::Checkbox("Player X AI", &playerXAI);
	ImGui::Checkbox("Player Y AI", &playerYAI);

}

