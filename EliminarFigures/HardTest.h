#include "Test.h"
#include "Shader.h"
#include "Renderer.h"
#include "ObjParser.h"
#include "Object.h"
#include <unordered_map>

namespace Test {


	class HardTest : public Test

	{
	public:
		HardTest();
		~HardTest();


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
		Object Rings;
		Object Pointy;

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
		void RegisterObject(Object* object);
		
		bool CatchingObject = false;


	};
	

}