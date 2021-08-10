#include "Test.h"
#include "PositionsBuffers.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "CollisionDetector.h"
#include "Shader.h"
#include "Renderer.h"
#include "ObjParser.h"
#include <tuple>

namespace Test {

	class HardTest : public Test

	{
	public:
		HardTest();
		HardTest(GLFWwindow* window);
		~HardTest();


		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;

		void SaveWindow(GLFWwindow* window) override
		{
			ptr_window = window;
		}


	private:
		Figures m_Figures;
		ControlLines m_ControlLines;
		ObjParser m_Donut;


		VertexArray vaoH;                   //Vertex Array for the Horizontal Line
		VertexArray vaoV;                   //Vertex Array for the Vertical Line
		VertexArray vaoC;                   //Vertex Array for the Contral Quad

		VertexArray vaoQuad;                //Vertex Array for a random Quad
		VertexArray vaoStar;                //Vertex Array for a random Star
		VertexArray vaoVFigure;					//Vertex Array for a random V type figure;	
		VertexArray vaoDonut;

		VertexBuffer vboH;					
		VertexBuffer vboV;
		VertexBuffer vboC;
		VertexBuffer vboQuad;
		VertexBuffer vboStar;
		VertexBuffer vboVFigure;
		VertexBuffer vboDonut;

		IndexBuffer iboH;
		IndexBuffer iboV;
		IndexBuffer iboC;

		IndexBuffer iboQuad;
		IndexBuffer iboStar;
		IndexBuffer iboVFigure;
		IndexBuffer iboDonut;

		Shader shader;

		CollisionDetector collision;
		CollisionDetector collision2;
		CollisionDetector collision3;

		Renderer renderer;

		float x = 1.0f;
		float y = 1.0f;

		glm::mat4 m_Proj;
		glm::mat4 m_View;
		glm::mat4 m_Model;
		glm::mat4 u_MVP;

		GLFWwindow* ptr_window;
	
		std::vector<std::tuple<VertexArray&, IndexBuffer&, CollisionDetector*>> WorldBuffer;

		void RegisterWorldBuffer(VertexArray& vao, IndexBuffer& ibo, CollisionDetector* cdo);
		void OnVaoRender(std::vector<std::tuple<VertexArray&, IndexBuffer&, CollisionDetector*>> worldBuffer, const Shader& shader);
		

	};
	

}