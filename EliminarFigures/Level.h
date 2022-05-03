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
#include "imgui/imgui.h"
#include "Json/json.hpp"
#include "Config.h"
#include "AiInterface.h"
#include "rcInterface.h"
#include "ObjectReader.h"
#include "Renderer.h"

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
		Level(bool playerXAI, bool playerYAI) {}
		virtual ~Level() {}


		virtual void OnUpdate(float deltaTime, bool& testExit) {}
		virtual void OnRender() {}
		virtual void OnImGuiRender(GLFWwindow* window) {}

		virtual void SaveWindow(GLFWwindow* window) {}
		/*
		I didn't make them pure virtual functions because this way we can selectively choose which once we want to override
		*/
	private:


	protected:
		static AiInterface aiInterface;
		static RcInterface rcInterface;
		ObjectReader objectReader;
		std::vector<Object*> worldBuffer;
		Renderer renderer;

		glm::mat4 m_Proj = glm::ortho(640.0f, -640.0f, -360.0f, 360.0f, -640.0f, 640.0f);

		glm::mat4 m_View = glm::lookAt(
			glm::vec3(0.0f, 0.0f, 0.0f),
			glm::vec3(0.0f, 0.0f, 1.0f),
			glm::vec3(0.0f, 1.0f, 0.0f)
		);

	};

	using str_vector_pair = std::vector<std::pair<std::string, bool>>;

	class Menu : public Level
	{
	public:
		Menu(Level*& currentTest);
		~Menu();

		void OnImGuiRender(GLFWwindow* window) override;
		void OnUpdate(float deltaTime, bool& testExit) override;
		void OnRender() override;

		template <typename T>
		void RegisterTest(const std::string& testName)
		{
			std::cout << "Registering test: " << testName << std::endl;
			m_Tests.push_back(std::make_pair(testName, [=]() { return new T(config); }));		// We add a lamda that creates the registering level

		}

	private:
		Level*& m_CurrentTest;																	// Pointer to the current level being executed.
		std::vector < std::pair < std::string, std::function<Level* () >> > m_Tests;			// Function pointer that creates a new level
		bool playerXAI;
		bool playerYAI;

		ImGuiWindowFlags window_flags = 0;		// Flags of the imgui config panel (main panel)
		nlohmann::json jconfig;					// Json object to store current config
		Config::Config config;					// Struct that stores in ram the current config

		std::vector<glm::mat4> modelMatrices;	// Model matrices that keep track of the object position (unscaled). Used to scale the object live through the ui.
		
		void initConfig();						// Reloads the default config into the jconfig attribute 
		
		// UI headers of the main pannel
		void objectsHeader();
		void socketsHeader();
		void loggingHeader();
		void levelsHeader(GLFWwindow* window);
		void playersHeader();

		// Main menu bar on the screen
		void createMenuBar();

		// Json config 
		void loadConfig(std::fstream& file);								// Loads the defaults value stored in the default configuration file. Json file in res/config/default.json
		void createDefaults(std::fstream& file, const char* filepath);		// Used to create a default json file in the config directory when there is not one.

		// Auxiliar windows (separete windows that the user have opened)
		void checkAuxiliarWindows();
		void createFileExplorer(bool* p_open, const char* directory, const char* buttonName, void(Menu::* buttonAction)(const std::string&, bool*));

		// Button action functions (used as function pointers in the file explorer method)
		void saveConfigButton(const std::string& filename, bool* p_open);			
		void loadConfigButton(const std::string& filename, bool* p_open);

		// Objects selection
		void updateAllObjectSelection(const str_vector_pair& objNames, const str_vector_pair& imgNames, const str_vector_pair& pkgNames);
		void updateObjectSelection(std::string*& it, const str_vector_pair& names, const char* directory);

		// UI managing variables
		bool open = false;
		bool save = false;

		// World objects to see the changes in imgui live
		void loadObjectFiles();


	};

}
