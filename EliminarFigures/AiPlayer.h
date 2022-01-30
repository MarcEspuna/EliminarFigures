#pragma once
#include <iostream>
#include <vector>
#include "BasicObject.h"
#include <unordered_map>

class AiPlayer
{
public:
	//In the collision detector we have all the updated vertex positions of the main square
	AiPlayer(const std::vector<Object*>& objects, int* playerInput, Object& centerQuad);

	void makeNewDesicion();
	std::tuple<int, int, bool> getOutput();
	void refreshAiPlayerPosition();

private:

	Object& centerQuad;
	int* otherPlayerInput; 
	const std::vector<Object*>& objects;
	std::vector<glm::vec2> objectsPositions;			//Posicions of all the objects in the world
	std::vector<glm::vec2> updatedVerticesQuad;
	
	
	void refreshObjectsPositions();			//Using the model matrix we calculate the positions of the world objects (And our position)
	std::pair<int,int> objectSelector();	//Returs the index of the object and the index of the model matrix that we want to track.
	void UpdateVerticies(const std::vector<float>& originalVertex2D, const glm::mat4& u_Model, std::vector<glm::vec2>& vectorToUpdate);



	//Idea si el jugador va cap a una direccio, només tindra en compte els objectes d'aquella direcció

};

