#pragma once
#include "Entity.h"
#include "ObjParser.h"
#include "CollisionDetector.h"
#include "ImguiVariables.h"
#include "Shader.h"
#include "SquareCollider.h"
#include "ObjectMovement.h"
#include "RandomGenerator.h"
#include <functional>

class Object  : public Entity
{
public:

	Object();
	Object(glm::vec4 color);
	Object(float scale);
	Object(glm::vec4 color,const char* shaderPath);
	Object(glm::vec4 color,const char* shaderPath, const tinyobj::shape_t& shape, const glm::mat4& u_Model, const float& scale = 1.0f);
	Object(glm::vec4 color, glm::vec3 scale);



	virtual void OnObjectUpdate(bool deleteObject,const float& deltaTime, ImguiVariables& ImGuiVar) = 0;
	virtual void setUniformCollider(size_t objectIndex, const glm::mat4& projection, const glm::mat4& view);
	virtual void BindCollider() const;
	virtual size_t GetVertexSize() { return 0; };
	virtual size_t GetIndexSize() { return 0; };
	virtual std::vector<unsigned int>& GetIndex();
	virtual std::vector<float>& GetVertex();

	void TrackCollisionWith(Object* otherObject);
	float UpdateCollisionWith(Object* other);
	void New(const glm::mat4& u_NewModel, const glm::vec3& movement = { 1.0f,1.0f,1.0f });
	void New(const RandomGenerator& random);

	void moveUP(const float& deltaTime, const float& sensitivity);
	void moveDown(const float& deltaTime, const float& sensitivity);
	void moveRight(const float& deltaTime, const float& sensitivity);
	void moveLeft(const float& deltaTime, const float& sensitivity);

	inline std::vector<glm::mat4>& GetModels() { return vec_Model; }
	inline const glm::vec4& GetColor() { return u_Color; } 
	inline VertexArray& GetVao() { return vao; }
	inline IndexBuffer& GetIbo() { return ibo; }
	inline const size_t getIboCountCollider() { return m_CollisionView.getIboCount(); }

	inline bool GetCollisionStatus() { return collision.GetStatus(); }
	inline void CollisionEnd() { collision.End(); }
	inline std::vector<glm::vec3>& GetMovementValues() { return movementValues; }
	inline size_t getIboCount() const { return ibo.GetCount(); };
	inline size_t size() const { return vec_Model.size(); }
	inline void objectSelected() { selected = true; }
	inline void objectUnSelected() { selected = false; }
	inline void isTarget() { activeCollider = true; }
	inline void isNotTarget() { activeCollider = false; }
	inline void objectHit() { hit = true; }
	inline bool isColliderActive() { return activeCollider; }
	inline SquareObject& getVisualizer() { return m_CollisionView; }

protected:

	bool selected;
	bool hit;
	bool activeCollider;

	float m_DefaultScale = 1.0f;

	CollisionDetector collision;						//Move to BasicObject
	SquareCollider m_SquareCollider;
	SquareObject   m_CollisionView;
	ObjectMovement objectMovement;


	std::vector<glm::mat4> vec_Model;					//Canviar a vector per si volem renderitzar el mateix vao en varios llocs
	std::vector<glm::mat4> u_MVP;						//
	std::vector<glm::vec3> movementValues;				//Xsing, Ysign, Angle
														//Fixed color for all the objects
	
	inline int f_CheckCollision() {return collision.RefreshStatus(); };


};

enum class ObjectType
{
	LIGHT_OBJECT,
	BASIC_OBJECT,
	UNSUPORTED_OBJECT
};
