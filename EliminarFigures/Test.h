#pragma once
//Abstract class to make different Tests
#include <functional>
#include <vector>
#include <string>

namespace Test
{
	class Test
	{
	public:
		Test() {}
		virtual ~Test() {}


		virtual void OnUpdate(float deltaTime) {}	
		virtual void OnRender() {}
		virtual void OnImGuiRender() {}

		/*
		I didn't make them pure virtual functions because this way we can selectively choose which once we want to override
		*/

	};


	class TestMenu : public Test
	{
	public:
		TestMenu(Test*& currentTest);
		~TestMenu();

		void OnImGuiRender() override;


	private:
		Test*& m_CurrentTest;
		std::vector < std::pair < std::string, std::function<Test* () >> > m_Tests;


	};


}
