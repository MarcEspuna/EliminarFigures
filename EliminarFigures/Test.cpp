#include "Test.h"
#include "imgui/imgui.h"

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
		ImGui::Button(test.first.c_str());
		m_CurrentTest = test.second();
	}


}
