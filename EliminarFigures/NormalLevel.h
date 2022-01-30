#include "Level.h"
#include "Shader.h"
#include "Renderer.h"
#include "ObjParser.h"
#include "ObjectLight.h"
#include "TextureObject.h"
#include <unordered_map>
#include <future>
#include <mutex>
#include <algorithm>
#include <numeric>


namespace Level {

	class NormalLevel : public Level

	{
	public:
		NormalLevel();
		~NormalLevel();

		void OnRender() override;
		void OnUpdate(float deltaTime, bool& testExit) override;
		void OnImGuiRender() override;
		void SaveWindow(GLFWwindow* window) override
		{
			ptr_window = window;
		}


	private:

		Shader shader;
		Shader TexShader;

		Renderer renderer;

		glm::mat4 m_Proj = glm::ortho(-16.0f, 9.0f, -16.0f, 9.0f);


		glm::mat4 m_View = glm::lookAt(
			glm::vec3(0.0f, 0.0f, 0.0f),
			glm::vec3(0.0f, 0.0f, 1.0f),
			glm::vec3(0.0f, 1.0f, 0.0f)
		);

		std::vector<ObjectLight> worldBuffer;

		GLFWwindow* ptr_window = nullptr;

		RandomGenerator random;

		void RegisterObjects();



	};
	

}