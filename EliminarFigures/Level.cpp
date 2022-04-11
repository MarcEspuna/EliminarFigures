#include "Level.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_opengl3.h"
#include "imgui/imgui_impl_glfw.h"
#include <iostream>
#include <fstream>
#include <filesystem>
#include <vector>

#define	CONFIG_FILEPATH	"res/config/default.json"
#define CONFIG_DIRECTORY "res/config/"
#define OBJECTS_DIRECTORY "res/obj/obj/"
#define IMAGES_DIRECTORY "res/obj/images/"

static std::string parseString(std::string str, const std::string& delimiter);
static std::vector<std::string> getFiles(std::string directory);

Level::Menu::Menu(Level*& currentTest)
	:m_CurrentTest(currentTest), playerXAI(false), playerYAI(false)
{
	window_flags |= ImGuiWindowFlags_NoResize;
	
	initConfig();

	config = jconfig.get<Config::Config>();	

}

Level::Menu::~Menu()
{
	
}

void Level::Menu::OnImGuiRender(GLFWwindow* window)
{

	createMenuBar();
	checkAuxiliarWindows();

	// MAIN CONFIG PANEL:
	// We specify the window size and position
	ImGui::SetNextWindowPos(ImVec2(700,100));			
	ImGui::SetNextWindowSize(ImVec2(500, 800));

	ImGui::Begin("CONFIG PANEL", nullptr, window_flags);  // Begin the main config panel

	ImGui::Spacing();

	levelsHeader(window);

	ImGui::Spacing();

	playersHeader();

	ImGui::Spacing();

	objectsHeader();

	ImGui::Spacing();

	socketsHeader();

	ImGui::Spacing();

	loggingHeader();

    ImGui::End();
	
	if (!aiInterface.isActive() && config.socket.aiEnable) aiInterface.init(config.socket.aiPort);
	if (aiInterface.isActive() && !config.socket.aiEnable) { aiInterface.fini(); }

}
/// <summary>
/// Initializes the configuration with the default values of the default json file. If it dosen't exist, it creates a default config json file
/// </summary>
void Level::Menu::initConfig()
{
	std::fstream file(CONFIG_FILEPATH, std::fstream::in);
	if (file.is_open()) {
		loadConfig(file);
		file.close();
	}
	else {
		createDefaults(file, CONFIG_FILEPATH);
	}
}

void Level::Menu::objectsHeader()
{
	if (ImGui::CollapsingHeader("Objects"))
	{
		if (ImGui::TreeNode("Load Objects"))
		{
			ImGui::Spacing();
			ImGui::Spacing();
			ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_None;
			if (ImGui::BeginTabBar("Object types", tab_bar_flags))
			{
				if (ImGui::BeginTabItem("Png Objects"))
				{
					ImGui::Selectable("banana.png");
					ImGui::Selectable("potato.png");
					ImGui::Selectable("apple.png");
					ImGui::Selectable("carrot.png");
					ImGui::EndTabItem();
				}
				if (ImGui::BeginTabItem("3D objects"))
				{
					ImGui::Selectable("bunny.obj");
					ImGui::Selectable("teapot.obj");
					ImGui::Selectable("Icosphere.obj");
					ImGui::Selectable("torus.obj");
					ImGui::Selectable("square.obj");
					ImGui::Spacing();
					ImGui::EndTabItem();
				}
				if (ImGui::BeginTabItem("Object packages"))
				{
					ImGui::Selectable("Animals");
					ImGui::Selectable("Food");
					ImGui::Selectable("Birds");
					ImGui::Spacing();
					ImGui::EndTabItem();

				}
				ImGui::Spacing();
				ImGui::Spacing();
				ImGui::EndTabBar();
			}

			ImGui::TreePop();
		}

		if (ImGui::TreeNode("Lighting"))
		{
			ImGui::Spacing();
			ImGui::Spacing();
			ImGui::SliderFloat("X position", &config.obj.light.lightDir[0], 0.0f, 100.0f, "%.3f");
			ImGui::SliderFloat("Y position", &config.obj.light.lightDir[1], 0.0f, 100.0f, "%.3f");
			ImGui::SliderFloat("Z position", &config.obj.light.lightDir[2], 0.0f, 100.0f, "%.3f");

			ImGui::Spacing();
			ImGui::Spacing();

			ImGui::SliderFloat("Ambient light", &config.obj.light.ambient, 0.0f, 100.0f, "%.3f");
			ImGui::SliderFloat("Diffuse light", &config.obj.light.diffuse, 0.0f, 100.0f, "%.3f");
			ImGui::SliderFloat("Specular light", &config.obj.light.specular, 0.0f, 100.0f, "%.3f");
			ImGui::Spacing();
			ImGui::Spacing();
			ImGui::TreePop();
		}

		if (ImGui::TreeNode("Movement"))
		{
			ImGui::SliderFloat("Rotation speed: ", &config.obj.movement.rotation, -50.0f, 50.0f, "%.3f");
			ImGui::TreePop();
		}

		if (ImGui::TreeNode("Positions"))		// TODO: linking to the config struct
		{
			static int positioning = 0;
			ImGui::Spacing();
			ImGui::RadioButton("Random positioning", &positioning, 1);
			ImGui::SameLine();
			ImGui::RadioButton("Manual positioning", &positioning, 2);
			ImGui::Spacing();

			if (positioning == 2)
			{
				ImGui::Text("Manual positionning is selected.");
			}

			ImGui::TreePop();
		}

	}
}

void Level::Menu::socketsHeader()
{
	if (ImGui::CollapsingHeader("Sockets"))
	{
		if (ImGui::TreeNode("Robot connection"))
		{
			// User port
			ImGui::Text("User port number: ");
			ImGui::SameLine(); 
			ImGui::InputInt("##edit1", &config.socket.userPort);

			// Ai port
			ImGui::Text("Ai port number: \t");
			ImGui::SameLine();
			ImGui::InputInt("##edit2", &config.socket.aiPort);

			ImGui::TreePop();
		}

		if (ImGui::TreeNode("Server"))
		{
			ImGui::Checkbox("Enable Ai server", &config.socket.aiEnable);
			if (config.socket.aiEnable)
			{

				ImGui::Text("Ai socket is : ");
				ImGui::SameLine();
				if (aiInterface.isConnected())	ImGui::Text("connected.");
				else ImGui::Text("listenning.");
			}

			ImGui::Spacing();
			ImGui::Checkbox("Enable remote contols", &config.socket.userEnable);
			if (config.socket.userEnable)
			{
				ImGui::Text("User controls are : listenning.");
			}
			ImGui::TreePop();
		}
	}
}

void Level::Menu::loggingHeader()
{
	if (ImGui::CollapsingHeader("Log"))
	{
		ImGui::Spacing();
		static float arr[6] = { 1.0f, 2.0f, 0.55f, 4.0f, 3.0f, 2.0f };
		ImGui::PlotLines("Lines", arr, IM_ARRAYSIZE(arr), 0, NULL, 0.0f, 5.0f, ImVec2(0, 80.0f));
		ImGui::PlotHistogram("Histogram", arr, IM_ARRAYSIZE(arr), 0, NULL, 0.0f, 5.0f, ImVec2(0, 80.0f));
	}
}

void Level::Menu::levelsHeader(GLFWwindow* window)
{
	if (ImGui::CollapsingHeader("Lavels"))
	{
		for (auto& test : m_Tests)
		{
			if (ImGui::Button(test.first.c_str()))
			{
				m_CurrentTest = test.second();
				m_CurrentTest->SaveWindow(window);
			}
		}
	}
}

void Level::Menu::playersHeader()
{
	if (ImGui::CollapsingHeader("Players"))
	{
		ImGui::Checkbox("Player X AI", &config.aiPlayerX);
		ImGui::Checkbox("Player Y AI", &config.aiPlayerY);
	}
}

void Level::Menu::createMenuBar()
{
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Open", "Ctrl+A"))	open = true;
			if (ImGui::MenuItem("Save", "Ctrl+S"))	save = true;
			if (ImGui::MenuItem("Restore defaults", "Ctrl+D"))
			{
				initConfig();	// Reload the default values to the jconfig object.
				config = jconfig.get<Config::Config>();	// Reload the configuration struct.
			}
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}
}

void Level::Menu::loadConfig(std::fstream& file)
{
	file >> jconfig;
	std::cout << "[JSON]: Config loaded." << std::endl;
}

/// <summary>
/// Creates a json file with the programmed default parameters. Used in case no default json file is found.
/// </summary>
/// <param name="file"> file that will be used to create the json file. It should already be opened. </param>
void Level::Menu::createDefaults(std::fstream& file, const char* filepath)
{
	file.open(filepath, std::fstream::out);	
	// Specify all the programmed default values:
	jconfig["AiPlayerX"] = false;
	jconfig["AiPlayerY"] = false;
	nlohmann::json obj;
	obj["filenames"] = {
		"bunny.obj",
		"teapot.obj",
		"square.obj",
		"Icosphere.obj",
		"torus.obj"
	};
	obj["lighting"] = {
		{"light direction", { 0.0f, 0.0f, -100.0f}},
		{"ambient", 1.0f},
		{"diffuse", 4.0f},
		{"specular", 20.0f}
	};
	obj["movement"] = {
		{"rotation", 20.0f},
		{"scale", 1.0f},
		{"random positioning", true}
	};

	jconfig["object"] = obj;

	jconfig["sockets"] = {
		{"aiPort", 10000},
		{"userPort", 8000},
		{"aiEnable", true},
		{"userEnable", true},
	};

	// We create and write the default config
	file << std::setw(4) << jconfig;
	file.close();
	std::cout << "[JSON]: New default json config file created." << std::endl;
}

void Level::Menu::checkAuxiliarWindows()
{
	if (open)			createFileExplorer(&open, CONFIG_DIRECTORY, "Load config", &Menu::loadConfigButton);		
	else if (save)		createFileExplorer(&save, CONFIG_DIRECTORY, "Save config", &Menu::saveConfigButton);
}


void Level::Menu::createFileExplorer(bool* p_open, const char* directory, const char* buttonName, void(Menu::*buttonAction)(const std::string&, bool*))
{
	static ImGuiWindowFlags flags = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize;	// No resizing of the 
	static std::vector<std::string> filepaths = getFiles(CONFIG_DIRECTORY);				// Vector where we will store the filepaths
	static int item_current_idx = 0; // Here we store our selection data as an index.
	//TODO: position and size
	ImGui::Begin("File Explorer", p_open, flags);
	ImGui::Text("Files in the directory: "); ImGui::SameLine(); ImGui::Text(CONFIG_DIRECTORY);
	ImGui::Spacing();
	if (ImGui::BeginListBox("##listbox 2", ImVec2(-FLT_MIN, 5 * ImGui::GetTextLineHeightWithSpacing())))
	{
		for (int n = 0; n < filepaths.size(); n++)
		{
			const bool is_selected = (item_current_idx == n);
			if (ImGui::Selectable(filepaths[n].c_str(), is_selected))
				item_current_idx = n;

			// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
			if (is_selected)
				ImGui::SetItemDefaultFocus();
		}

		ImGui::EndListBox();
		ImGui::Spacing();

		ImGui::Text("Path: "); 
		ImGui::SameLine(); 
		ImGui::TextColored({0.6f, 0.7f, 0.8f, 1.0f}, (CONFIG_DIRECTORY + filepaths[item_current_idx]).c_str());

		if (ImGui::Button(buttonName))	// We call the action button when we press it
		{
			(this->*buttonAction)(filepaths[item_current_idx], p_open);
		}
	}
	ImGui::End();
}

void Level::Menu::saveConfigButton(const std::string& filename, bool* p_open)
{
	//TODO
	*p_open = false;
}

void Level::Menu::loadConfigButton(const std::string& filename, bool* p_open)
{
	std::fstream file(CONFIG_DIRECTORY + filename, std::fstream::in);
	loadConfig(file);
	file.close();
	*p_open = false;
	config = jconfig.get<Config::Config>();	// Reload the configuration struct.
}



// STATIC FUNCTIONS THAT THE MODULE USES:

/// <summary>
/// Splits a string with a specified delimiter
/// </summary>
/// <param name="str"> String to parse </param>
/// <param name="delimiter"> delimiter to split the main string </param>
/// <returns>The last substring with the specified delimiter </returns>
static std::string parseString(std::string str, const std::string& delimiter)
{
	std::string token;
	size_t pos = 0;
	while ((pos = str.find(delimiter)) != std::string::npos)
	{
		token = str.substr(0, pos);
		str.erase(0, pos + delimiter.length());
	}
	return str;
}

/// <summary>
/// retrieves all the files of a given directory
/// </summary>
/// <param name="directory"></param>
/// <returns>All the file names of a given directory</returns>
static std::vector<std::string> getFiles(std::string directory)
{
	std::vector<std::string> filenames;
	for (const auto& entry : std::filesystem::directory_iterator(directory))
	{
		std::string path = entry.path().string();
		filenames.push_back(parseString(path, "/"));
	}
	return filenames;
}