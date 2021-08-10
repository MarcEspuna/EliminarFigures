#pragma once
//Abstract class to make different Tests
#include <iostream>
#include <functional>
#include <vector>
#include <string>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "PositionsBuffers.h"

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

		virtual void SaveWindow(GLFWwindow* window) {}
		/*
		I didn't make them pure virtual functions because this way we can selectively choose which once we want to override
		*/
	private:

	};


	class TestMenu : public Test
	{
	public:
		TestMenu(Test*& currentTest);
		~TestMenu();

		void OnImGuiRender() override;

		template <typename T>
		void RegisterTest(const std::string& testName)
		{
			std::cout << "Registering test: " << testName << std::endl;
			m_Tests.push_back(std::make_pair(testName, []() { return new T; }));		// LAMDAS!
		}

	private:
		Test*& m_CurrentTest;
		std::vector < std::pair < std::string, std::function<Test* () >>> m_Tests;
	};
}
