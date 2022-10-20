#pragma once
#include "Entity.h"

class SquareObject : public Entity 
{
public:
	SquareObject();
	SquareObject(const float* shape, const unsigned int* indexes);
	
	void Load(const float* shape, const unsigned int* indexes);
	void setUniform(size_t objectIndex, const glm::mat4& projection, const glm::mat4& view) override;



private:

};

