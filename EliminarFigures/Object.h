#pragma once
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "ObjParser.h"
#include "CollisionDetector.h"
#include "ImguiVariables.h"
#include "Shader.h"
#include <functional>

class Object
{
public:
	Object(const char* filePath);
	Object(const char* filePath, glm::vec4 color);
	Object(const char* filePath, float scale);
	Object(const char* filePath, glm::vec4 color, float scale);
	Object(const char* filePath, glm::vec4 color, glm::vec3 scale);
	~Object();

	std::function<void(glm::mat4&, const glm::vec2& ,glm::vec4&, const float&, glm::vec3& movement)> f_ModelColorUpdate =
		[&](glm::mat4& model, const glm::vec2& oneVertex ,glm::vec4& color, const float& deltaTime, glm::vec3& movement) {};

	void OnObjectUpdate(bool deleteObject,const float& deltaTime, ImguiVariables& ImGuiVar);
	void TrackCollisionWith(Object* otherObject);
	void New(const glm::mat4& u_NewModel, const glm::vec3& movement = { 1.0f,1.0f,1.0f });
	void Bind() const;
	void setUniform(size_t objectIndex, const glm::mat4& projection, const glm::mat4& view);

	inline std::vector<glm::mat4>& GetModels() { return vec_Model; }
	inline const glm::vec4& GetColor() { return u_Color; } 
	inline VertexArray& GetVao() { return vao; }
	inline IndexBuffer& GetIbo() { return ibo; }
	inline size_t GetVertexSize() { return m_Data.GetVerticesIn2D().size(); }
	inline size_t GetIndexSize() { return m_Data.GetIndexes().size(); }
	inline std::vector<unsigned int>& GetIndex() { return m_Data.GetIndexes(); }
	inline std::vector<float>& GetVertex() { return m_Data.GetVerticesIn2D(); }
	inline bool GetCollisionStatus() { return collision.GetStatus(); }
	inline void CollisionEnd() { collision.End(); }
	inline std::vector<glm::vec3>& GetMovementValues() { return movementValues; }
	inline size_t getIboCount() const { return ibo.GetCount(); };
	inline size_t size() const { return vec_Model.size(); }

private:

	ObjParser m_Data;
	VertexArray vao;
	VertexBuffer vbo;
	IndexBuffer ibo;
	CollisionDetector collision;
	Shader shader;

	std::vector<glm::mat4> vec_Model;					//Canviar a vector per si volem renderitzar el mateix vao en varios llocs
	std::vector<glm::mat4> u_MVP;						//
	std::vector<glm::vec3> movementValues;				//Xsing, Ysign, Angle
	glm::vec4 u_Color;									//Fixed color for all the objects
	
	inline int f_CheckCollision() {return collision.RefreshStatus(); };


};

