#pragma once
//Abstract class to make different Tests
#include <iostream>
#include <functional>
#include <vector>
#include <string>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "PositionsBuffers.h"
#include "RandomGenerator.h"

namespace Level
{
	enum class WinOrLose
	{
		LOST = 1,
		WON = 2
	};

	/*Interface for each level (window in the userspace you cold call it)*/
	class Level
	{
	public:
		Level() {}
		virtual ~Level() {}


		virtual void OnUpdate(float deltaTime, bool& testExit) {}
		virtual void OnRender() {}
		virtual void OnImGuiRender() {}

		virtual void SaveWindow(GLFWwindow* window) {}
		/*
		I didn't make them pure virtual functions because this way we can selectively choose which once we want to override
		*/
	private:


	};


	class Menu : public Level
	{
	public:
		Menu(Level*& currentTest);
		~Menu();

		void OnImGuiRender() override;

		template <typename T>
		void RegisterTest(const std::string& testName)
		{
			std::cout << "Registering test: " << testName << std::endl;
			m_Tests.push_back(std::make_pair(testName, []() { return new T; }));		// LAMDAS!

		}

	private:
		Level*& m_CurrentTest;
		std::vector < std::pair < std::string, std::function<Level* () >>> m_Tests;

	};
}
