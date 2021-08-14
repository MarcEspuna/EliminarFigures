#pragma once
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "ObjParser.h"
#include "CollisionDetector.h"
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

	std::function<void(glm::mat4&, glm::vec4&, const float& deltaTime)> f_ModelColorUpdate = [&](glm::mat4& model, glm::vec4& color, const float& deltaTime) {};

	void OnObjectUpdate(bool deleteObject,const float& deltaTime);
	void TrackCollisionWith(Object* otherObject);
	void New(glm::mat4 u_NewModel);


	inline std::vector<glm::mat4>& GetModels() { return vec_Model; }
	inline glm::vec4& GetColor() { return u_Color; }
	inline VertexArray& GetVao() { return vao; }
	inline IndexBuffer& GetIbo() { return ibo; }
	inline size_t GetVertexSize() { return m_Data.GetVerticesIn2D().size(); }
	inline size_t GetIndexSize() { return m_Data.GetIndexes().size(); }
	inline std::vector<unsigned int>& GetIndex() { return m_Data.GetIndexes(); }
	inline std::vector<float>& GetVertex() { return m_Data.GetVerticesIn2D(); }
	inline bool GetCollisionStatus() { return collision.GetStatus(); }
	inline void CollisionEnd() { collision.End(); }

private:

	ObjParser m_Data;
	VertexArray vao;
	VertexBuffer vbo;
	IndexBuffer ibo;
	CollisionDetector collision;

	std::vector<glm::mat4> vec_Model;					//Canviar a vector per si volem renderitzar el mateix vao en varios llocs
	glm::vec4 u_Color;									//Fixed color for all the objects
	
	std::function<int()> f_CheckCollision = [&]() {return collision.RefreshStatus(); };
};

