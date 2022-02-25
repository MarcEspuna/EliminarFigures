#include "Level.h"
#include "Shader.h"
#include "Renderer.h"
#include "ObjParser.h"
#include "ObjectLight.h"
#include "BasicObject.h"
#include "ObjectReader.h"
#include "TextureObject.h"
#include "AiPlayer.h"
#include <unordered_map>
#include <future>
#include <mutex>
#include <algorithm>
#include <numeric>


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
		NormalLevel(bool playerXAI, bool playerYAI);
		~NormalLevel();

		void OnRender() override;
		void OnUpdate(float deltaTime, bool& testExit) override;
		void OnImGuiRender() override;
		void SaveWindow(GLFWwindow* window) override
		{
			ptr_window = window;
		}


	private:

		Renderer renderer;
		ObjectReader objectReader;
		AiPlayer aiPlayer;
		DataLink dataLink;


		bool x_AiEnabled;
		bool y_AiEnabled;

		glm::mat4 m_Proj = glm::ortho(640.0f , -640.0f, -360.0f, 360.0f, -640.0f, 640.0f);

		glm::mat4 m_View = glm::lookAt(
			glm::vec3(0.0f, 0.0f, 0.0f),
			glm::vec3(0.0f, 0.0f, 1.0f),
			glm::vec3(0.0f, 1.0f, 0.0f)
		);

		std::vector<Object*> worldBuffer;
		Slot slot;
		Cursor cursor;

		GLFWwindow* ptr_window = nullptr;

		RandomGenerator random;

		void BuildObjects();
		void LoadObjectFiles();
		void updateCursor(const float& deltaTime);
		bool userHitKey();
		void createNewLevel();
		void LoadConfig();
		void doAiXInput(const float& deltaTime);
		void doAiYInput(const float& deltaTime);
		void doUserXInput(const float& deltaTime);
		void doUserYInput(const float& deltaTime);


	};
	

}