#pragma once
#include "DataLink.h"
#include "Level.h"
#include "Shader.h"
#include "Renderer.h"
#include "ObjParser.h"
#include "ObjectLight.h"
#include "BasicObject.h"
#include "TextureObject.h"
#include "AiPlayer.h"
#include <unordered_map>
#include <future>
#include <mutex>
#include <algorithm>
#include <numeric>
#include <thread>

namespace Level {

	struct Cursor
	{
		Object* HLine;
		Object* VLine;
		Object* CQuad;
	};

	class NormalLevel : public Level

	{
	public:
		NormalLevel(const Config::Config& config);
		~NormalLevel();

		void OnRender() override;
		void OnUpdate(float deltaTime, bool& testExit) override;
		void OnImGuiRender(GLFWwindow* window) override;
		void SaveWindow(GLFWwindow* window) override;
	


	private:

		int targetObjectId;
		bool levelActive;

		Config::Config configuration;
		//DataLink dataLink;
		std::thread* cursorUpdaterX;
		std::thread* cursorUpdaterY;

		bool x_AiEnabled;
		bool y_AiEnabled;
		bool userPressedKey;
		bool userSelectKeyX;
		bool userSelectKeyY;

		Slot slot;
		Cursor cursor;

		GLFWwindow* ptr_window = nullptr;

		RandomGenerator random;

		void BuildObjects();
		void LoadObjectFiles();
		//void updateCursor(const float& deltaTime);
		bool userHitKey();
		void createNewLevel();
		void LoadConfig();
		void doAiXInput();
		void doAiYInput();
		void doUserXInput();
		void doUserYInput();
		void loadCommunications();
		void loadThread();
		bool usrExitKey();

		void updateTarget();

	};
	

}