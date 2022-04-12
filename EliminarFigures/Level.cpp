#include "Level.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_opengl3.h"
#include "imgui/imgui_impl_glfw.h"
#include "Slot.h"
#include <iostream>
#include <fstream>
#include <filesystem>
#include <vector>
#include <iterator>

#define MAX_OBJ	5									// Maximum number of objects allowed in the KASPAR scene 

#define	CONFIG_FILEPATH	"res/config/default.json"
#define CONFIG_DIRECTORY "res/config/"
#define OBJECTS_DIRECTORY "res/obj/obj/"
#define IMAGES_DIRECTORY "res/obj/images/"
#define PACKAGES_DIRECTORY "res/obj/packages/"

static std::string parseString(std::string str, const std::string& delimiter);
static std::vector<std::string> getFiles(std::string directory);
static std::vector<std::pair<std::string, bool>> getSelectingFiles(const char* directory);
static void createObjectsSelectionTab(std::vector<std::pair<std::string, bool>>& selectionList, unsigned int& selectCounter);
static void HelpMarker(const char* desc);

Level::Menu::Menu(Level*& currentTest)
	:m_CurrentTest(currentTest), playerXAI(false), playerYAI(false)
{
	window_flags |= ImGuiWindowFlags_NoResize;
	
	initConfig();

	config = jconfig.get<Config::Config>();	

	loadObjectFiles();
	objectReader.buildObjects(worldBuffer);

}

Level::Menu::~Menu()
{
	
}

void Level::Menu::OnUpdate(float deltaTime, bool& testExit)
{
	ImguiVariables random;
	Slot slot;
	static std::vector staticModels = { slot[0][5], slot[1][5], slot[2][5], slot[3][5], slot[4][5] };
	// Execute the commands set in the ui interface:
	// Sockets:

	if (!aiInterface.isActive() && config.socket.aiEnable) aiInterface.init(config.socket.aiPort);
	if (aiInterface.isActive() && !config.socket.aiEnable) { aiInterface.fini(); }

	// Objects:
	int counter = 4;
	for (auto& object : worldBuffer)
	{
		const auto& scale = config.obj.scale[4-counter];
		const auto& rotation = config.obj.movement.rotation;
		object->GetModels()[0] = staticModels[counter];									// Upload the unscaled model
		object->setRotationSpeed(rotation);												// Set the rotation speed
		object->setLightDir({ config.obj.light.lightDir[0] * 10, config.obj.light.lightDir[1] * 10, config.obj.light.lightDir[2] * 10 });
		object->setLightParam(config.obj.light.ambient, config.obj.light.diffuse, config.obj.light.specular, 20);
		object->OnObjectUpdate(false, deltaTime, random);								// Rotate the object
		staticModels[counter] = object->GetModels()[0];									// Save the rotatin object before scaling
		object->GetModels()[0] = glm::scale(object->GetModels()[0], glm::vec3(scale));	// Scale to the setted up dimension
		object->isNotTarget();
		counter--;
	}
}

void Level::Menu::OnRender()
{
	glEnable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	renderer.ClearDepth();
	//Render all the world space objects:
	for (auto& object : worldBuffer)
	{
		renderer.Draw(*object, m_Proj, m_View);                  //Each pass will draw all the objects of the current shape object
	}
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
			static auto objNames = getSelectingFiles(OBJECTS_DIRECTORY);
			static auto imgNames = getSelectingFiles(IMAGES_DIRECTORY);
			static auto pkgNames = getSelectingFiles(PACKAGES_DIRECTORY);
			static unsigned int selectionsCounter = 0;

			ImGui::Spacing();
			ImGui::Spacing();
			ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_None;
			if (ImGui::BeginTabBar("Object types", tab_bar_flags))
			{
				ImGui::Spacing();
				if (ImGui::BeginTabItem("Png Objects"))
				{
					HelpMarker("Hold CTRL and click to select multiple items.");
					createObjectsSelectionTab(imgNames, selectionsCounter);
					ImGui::EndTabItem();
				}
				if (ImGui::BeginTabItem("3D objects"))
				{
					HelpMarker("Hold CTRL and click to select multiple items.");
					createObjectsSelectionTab(objNames, selectionsCounter);
					ImGui::EndTabItem();
				}
				if (ImGui::BeginTabItem("Object packages"))
				{
					HelpMarker("Hold CTRL and click to select multiple items.");
					createObjectsSelectionTab(pkgNames, selectionsCounter);
					ImGui::EndTabItem();

				}
				ImGui::Spacing(); ImGui::Spacing();
				if (ImGui::Button("Load objects"))
				{
					updateAllObjectSelection(objNames, imgNames, pkgNames);
				}
				ImGui::Spacing(); ImGui::Spacing();
				ImGui::EndTabBar();
			}

			ImGui::TreePop();
		}

		if (ImGui::TreeNode("Lighting"))
		{
			ImGui::Spacing();
			ImGui::Spacing();
			ImGui::SliderFloat("X position", &config.obj.light.lightDir[0], -100.0f, 100.0f, "%.3f");
			ImGui::SliderFloat("Y position", &config.obj.light.lightDir[1], -100.0f, 100.0f, "%.3f");
			ImGui::SliderFloat("Z position", &config.obj.light.lightDir[2], -100.0f, 100.0f, "%.3f");

			ImGui::Spacing();
			ImGui::Spacing();

			ImGui::SliderInt("Ambient light", &config.obj.light.ambient, 0.0f, 100.0f, "%.2d");
			ImGui::SliderInt("Diffuse light", &config.obj.light.diffuse, 0.0f, 100.0f, "%.2d");
			ImGui::SliderInt("Specular light", &config.obj.light.specular, 0.0f, 100.0f, "%.2d");
			ImGui::Spacing();
			ImGui::Spacing();
			ImGui::Checkbox("Highlight on collision", &config.obj.light.highlight);
			ImGui::TreePop();
		}

		if (ImGui::TreeNode("Edit objects"))
		{
			ImGui::Text("Objects rotation speed: ");
			ImGui::SliderInt("Edit rotation", &config.obj.movement.rotation, -50, 50, "%.2d");
			ImGui::Text("Objects scale:");
			ImGui::SliderInt(parseString(config.obj.filenames[0], "/").c_str(), &config.obj.scale[0], 0, 600);
			ImGui::SliderInt(parseString(config.obj.filenames[1], "/").c_str(), &config.obj.scale[1], 0, 600);
			ImGui::SliderInt(parseString(config.obj.filenames[2], "/").c_str(), &config.obj.scale[2], 0, 600);
			ImGui::SliderInt(parseString(config.obj.filenames[3], "/").c_str(), &config.obj.scale[3], 0, 600);
			ImGui::SliderInt(parseString(config.obj.filenames[4], "/").c_str(), &config.obj.scale[4], 0, 600);
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
	if (ImGui::CollapsingHeader("Levels"))
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
				initConfig();								// Reload the default values to the jconfig object.
				config = jconfig.get<Config::Config>();		// Reload the configuration struct.
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
		"res/obj/obj/teapot.obj",
		"res/obj/obj/bunny.obj",
		"res/obj/obj/square.obj",
		"res/obj/obj/Icosphere.obj",
		"res/obj/obj/torus.obj"
	};
	obj["scale"] = {
		90,
		500,
		35,
		50,
		35
	};
	obj["lighting"] = {
		{"light direction", { 0.0f, 0.0f, -100.0f}},
		{"ambient", 1.0f},
		{"diffuse", 4.0f},
		{"specular", 20.0f},
		{"highlight", true}
	};
	obj["movement"] = {
		{"rotation", 20 },
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

/// <summary>
/// It checks and rund the auxiliar windows of the app. It look at the appropiate boolean variables.
/// </summary>
void Level::Menu::checkAuxiliarWindows()
{
	if (open)			createFileExplorer(&open, CONFIG_DIRECTORY, "Load config", &Menu::loadConfigButton);		
	else if (save)		createFileExplorer(&save, CONFIG_DIRECTORY, "Save config", &Menu::saveConfigButton);
}

/// <summary>
/// Opens a new window with a file explorer an a specific directory. You can also specify the button label and a pointer to the button action. Needs to be a method
/// of the current class.
/// </summary>
/// <param name="p_open"> Pointer to a boolean indicating if the current window is open or closed </param>
/// <param name="directory"> Directory where the file explorer will be </param>
/// <param name="buttonName"> Label of the button </param>
/// <param name="buttonAction"> Function pointer that will be run when the button is pressed </param>
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

void Level::Menu::updateAllObjectSelection(const str_vector_pair& objNames, const str_vector_pair& imgNames, const str_vector_pair& pkgNames)
{
	for (auto& string : config.obj.filenames)	string.clear();		// We clear all the strings in the filenames
	std::string* it = std::begin(config.obj.filenames);
	updateObjectSelection(it, objNames, OBJECTS_DIRECTORY);
	updateObjectSelection(it, imgNames, IMAGES_DIRECTORY);
	updateObjectSelection(it, pkgNames, PACKAGES_DIRECTORY);
	for (auto& string : config.obj.filenames)	std::cout << string << std::endl;
}

void Level::Menu::updateObjectSelection(std::string*& it, const str_vector_pair& names, const char* directory)
{
	for (const auto& [name, select] : names)
	{
		if (it == std::end(config.obj.filenames)) return;	// If we arrived at the end of the filenames array we stop the method
		if (select) {
			*it = { directory + name };
			it++;
		}
	}

}

void Level::Menu::loadObjectFiles()
{
	Slot slot;
	Object::init();   //Reset the counter of objects to 0, in order to propoerly set the object ids
//We define the objects that we want to load:
	// TODO: We need to check lenght of filenames
	std::vector<ObjectArguments> objectArguments = {
		{config.obj.filenames[0], ObjectType::LIGHT_OBJECT, slot[4][5], 1.0f, {1.0f, 0.0f, 0.0f, 1.0f}},
		{config.obj.filenames[1], ObjectType::LIGHT_OBJECT, slot[3][5], 1.0f, {1.0f, 1.0f, 1.0f, 1.0f}},
		{config.obj.filenames[2], ObjectType::LIGHT_OBJECT, slot[2][5], 1.0f, {0.0f, 0.0f, 1.0f, 1.0f}},
		{config.obj.filenames[3], ObjectType::LIGHT_OBJECT, slot[1][5], 1.0f, {1.0f, 1.0f, 0.0f, 1.0f}},
		{config.obj.filenames[4], ObjectType::LIGHT_OBJECT, slot[0][5], 1.0f, {0.0f, 1.0f, 0.0f, 1.0f}}
	};
	objectReader.loadObjectFiles(objectArguments);
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


// Helper to display a little (?) mark which shows a tooltip when hovered.
// In your own code you may want to display an actual icon if you are using a merged icon fonts (see docs/FONTS.md)
static void HelpMarker(const char* desc)
{
	ImGui::TextDisabled("(?)");
	if (ImGui::IsItemHovered())
	{
		ImGui::BeginTooltip();
		ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
		ImGui::TextUnformatted(desc);
		ImGui::PopTextWrapPos();
		ImGui::EndTooltip();
	}
}
/// <summary>
/// Creates a vector of pairs of filenames and booleans. Used for the UI to indicate if the user selected the file or not.
/// </summary>
/// <param name="directory"></param>
/// <returns></returns>
static std::vector<std::pair<std::string, bool>> getSelectingFiles(const char* directory)
{
	std::vector<std::pair<std::string,bool>> filenames;
	for (const auto& entry : std::filesystem::directory_iterator(directory))
	{
		std::string path = entry.path().string();
		filenames.emplace_back(parseString(path, "/"), false);
	}
	return filenames;
}

static void resetSelection(std::vector<std::pair<std::string, bool>>& selectionList, unsigned int& selectCounter)
{
	for (auto& [name,select] : selectionList)
	{
		if (select)
		{
			selectCounter--;
			select = false;
		}
	}
}

static void createObjectsSelectionTab(std::vector<std::pair<std::string, bool>>& selectionList, unsigned int& selectCounter)
{
	for (auto& [name, select] : selectionList)
	{
		if (ImGui::Selectable(name.c_str(), select))
		{
			if (!ImGui::GetIO().KeyCtrl)							// Clear selection when CTRL is not held
			{
				resetSelection(selectionList, selectCounter);
			}
			else if (selectCounter < MAX_OBJ)
			{
				selectCounter++;
				select = true;										// We select the current selection
			}
		}
	}
}
