#include "Test.h"
#include "Shader.h"
#include "Renderer.h"
#include "ObjParser.h"
#include "Object.h"
#include <unordered_map>

namespace Test {


	class EasyTest : public Test

	{
	public:
		EasyTest();
		~EasyTest();


		void OnUpdate(float deltaTime) override;
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

		Shader shader;

		Renderer renderer;

		float x = 1.0f;
		float y = 1.0f;

		glm::mat4 m_Proj = glm::ortho(-640.0f, 640.0f, -360.0f, 360.0f);
		glm::mat4 m_View = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
		glm::mat4 u_MVP = glm::mat4(1.0f);

		GLFWwindow* ptr_window = nullptr;
	
		std::vector<Object*> WorldBuffer;

		void LoadVaoUpdateFuntions();
	
		bool CatchingObject = false;

		unsigned int IndexTracking = 0;
		std::unordered_map<unsigned int, bool> DeletedObjects =
		{
			{0, true},
			{1, true},
			{2, true},
			{3, true},
			{4, true},
			{5, true},

		};

		ImguiVariables m_Imgui = { 0 , 20, 0.0f };

	};
	

}