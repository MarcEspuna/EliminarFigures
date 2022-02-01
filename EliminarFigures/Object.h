#pragma once
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "ObjParser.h"
#include "CollisionDetector.h"
#include "ImguiVariables.h"
#include "Shader.h"
#include "SquareCollider.h"
#include <functional>

class Object
{
public:

	Object();
	Object(glm::vec4 color);
	Object(float scale);
	Object(glm::vec4 color,const char* shaderPath);
	Object(glm::vec4 color,const char* shaderPath, const tinyobj::shape_t& shape, const glm::mat4& u_Model);
	Object(glm::vec4 color, glm::vec3 scale);



	virtual void OnObjectUpdate(bool deleteObject,const float& deltaTime, ImguiVariables& ImGuiVar) = 0;
	virtual void setUniform(size_t objectIndex, const glm::mat4& projection, const glm::mat4& view) = 0;
	virtual void Bind() const;
	virtual size_t GetVertexSize() { return 0; };
	virtual size_t GetIndexSize() { return 0; };
	virtual std::vector<unsigned int>& GetIndex();
	virtual std::vector<float>& GetVertex();

	void TrackCollisionWith(Object* otherObject);
	void UpdateCollisionWith(Object* other);
	void New(const glm::mat4& u_NewModel, const glm::vec3& movement = { 1.0f,1.0f,1.0f });

	void moveUP(const float& deltaTime, const float& sensitivity);
	void moveDown(const float& deltaTime, const float& sensitivity);
	void moveRight(const float& deltaTime, const float& sensitivity);
	void moveLeft(const float& deltaTime, const float& sensitivity);

	inline std::vector<glm::mat4>& GetModels() { return vec_Model; }
	inline const glm::vec4& GetColor() { return u_Color; } 
	inline VertexArray& GetVao() { return vao; }
	inline IndexBuffer& GetIbo() { return ibo; }

	inline bool GetCollisionStatus() { return collision.GetStatus(); }
	inline void CollisionEnd() { collision.End(); }
	inline std::vector<glm::vec3>& GetMovementValues() { return movementValues; }
	inline size_t getIboCount() const { return ibo.GetCount(); };
	inline size_t size() const { return vec_Model.size(); }

protected:

	VertexArray vao;
	VertexBuffer vbo;
	IndexBuffer ibo;
	CollisionDetector collision;
	SquareCollider m_SquareCollider;
	Shader shader;

	std::vector<glm::mat4> vec_Model;					//Canviar a vector per si volem renderitzar el mateix vao en varios llocs
	std::vector<glm::mat4> u_MVP;						//
	std::vector<glm::vec3> movementValues;				//Xsing, Ysign, Angle
	glm::vec4 u_Color;									//Fixed color for all the objects
	
	inline int f_CheckCollision() {return collision.RefreshStatus(); };


};

enum class ObjectType
{
	LIGHT_OBJECT,
	BASIC_OBJECT,
	UNSUPORTED_OBJECT
};
