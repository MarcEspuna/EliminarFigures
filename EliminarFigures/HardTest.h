#include "Test.h"
#include "Shader.h"
#include "Renderer.h"
#include "ObjParser.h"
#include "Object.h"
#include "TextureObject.h"
#include <unordered_map>
#include <future>
#include <mutex>
#include <algorithm>
#include <numeric>


namespace Test {

	enum class WinOrLose 
	{
		LOST = 1,
		WON = 2
	};


	class HardTest : public Test

	{
	public:
		HardTest();
		~HardTest();


		void OnUpdate(float deltaTime, bool& testExit) override;
		void OnRender() override;
		void OnImGuiRender() override;
		void SaveWindow(GLFWwindow* window) override
		{
			ptr_window = window;
		}


	private:

		Object Horse;
		Object HLine;
		Object VLine;
		Object CQuad;
		Object Star;
		Object Rings;

		TextureObject tex_GameOver;
		TextureObject tex_YouLose;
		TextureObject tex_YouWin;

		Shader shader;
		Shader TexShader;

		Renderer renderer;

		glm::mat4 m_Proj = glm::ortho(-640.0f, 640.0f, -360.0f, 360.0f);
		glm::mat4 m_View = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
		glm::mat4 u_MVP = glm::mat4(1.0f);

		GLFWwindow* ptr_window = nullptr;
	
		std::vector<Object*> WorldBuffer;
		std::vector<TextureObject*> TextureBuffer;
		std::vector<std::future<void>> m_Futures;

		void LoadObjectUpdateFuntions();
		void RegisterObject(Object* object);
		void RegisterTexture(TextureObject* texture);
		void LoadNewObjects(const float& TimeLeft);

		bool CatchingObject = false;
		ImguiVariables m_Imgui = { 0, 20, 0.0f};
		float Time = 0;
		float TimeLeft = 75;


		unsigned int newTest = 0;

		std::unordered_map<unsigned int, bool> newObjectsSelector = { {0, true}, {1, true},{2, true},{3, true},{4, true},{5, true}};
		WinOrLose winOrLose = WinOrLose::LOST;

	};
	

}