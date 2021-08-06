#include "Test.h"
#include "PositionsBuffers.h"
#include "VertexArray.h"
#include "VertexBuffer.h"


namespace Test {

	class HardTest : public Test

	{
	public:
		HardTest();
		~HardTest();


		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;


	private:
		Figures m_Figures;
		ControlLines m_ControlLines;
		VertexArray vaoH;                   //Vertex Array for the Horizontal Line
		VertexArray vaoV;                   //Vertex Array for the Vertical Line
		VertexArray vaoC;                   //Vertex Array for the Contral Quad

		VertexArray vaoQuad;                    //Vertex Array for a random Quad
		VertexArray vaoStar;                    //Vertex Array for a random Star
		
		VertexBuffer vboH;
		VertexBuffer vboV;
		/*
		VertexBuffer vboC;
		VertexBuffer vboQuad;
		VertexBuffer vboStar;

		VertexArrayLayout layout;

		IndexBuffer iboH;
		IndexBuffer iboV;
		IndexBuffer iboC;

		IndexBuffer iboQuad;
		IndexBuffer iboStar;

		Shader shader;

		CollisionDetector collision;
		CollisionDetector collision2;
		*/
		float x = 1.0f;
		float y = 1.0f;

		glm::mat4 m_Proj;
		glm::mat4 m_View;
		glm::mat4 u_MVP;

	};


}