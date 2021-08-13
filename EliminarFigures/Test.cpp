#include "Test.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_opengl3.h"
#include "imgui/imgui_impl_glfw.h"
#include <iostream>

Test::TestMenu::TestMenu(Test*& currentTest)
	:m_CurrentTest(currentTest)
{


}

Test::TestMenu::~TestMenu()
{


}

void Test::TestMenu::OnImGuiRender()
{

	for (auto& test : m_Tests)
	{
		if (ImGui::Button(test.first.c_str())) 
		{
			m_CurrentTest = test.second();
		}
	}

}

