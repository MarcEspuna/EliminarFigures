#pragma once
#include "glm/glm.hpp"
#include "Object.h"
#include "SquareCollider.h"
#include "Server.h"
#include <thread>
#include <mutex>


class AiInterface
{
public:
	AiInterface();
	~AiInterface();

	void setCursor(const Object* object);
	void setObjects(const std::vector<Object*> objects);
	glm::vec3 getAiInput();
	void setUserPressedKey(const bool* userKey);

private:

	const SquareCollider* m_Cursor;
	std::vector<const Object*> m_Objects;
	const bool* userPressedKey;
	unsigned int activeObjects;						
	
	Server server;									//Server of the aplication
	std::thread* receive;							//Thread that will manage the incomming connections
	std::thread* send;								//Thread that will send the cursor or object positions
	std::thread* connect;							//Thread that will receive incomming data and store it

	char* dataS;									//Data to send  ('O', <object positions>) or ('C', <cursor position>, <overFigure> id, userHitKey)
	char dataR[3];									//Data received (x axis: int_8, y axis: int_8, correct initial reception: bool)

	void connectionManager();						//Listens and manages incomming connections
	void reception();								//Reads incomming data and stores it, also it's responsible to determine if the client is still connected
	void transmition();								//Transmits relevant data periodically
	void cleanThreads();							//Used to clean send and receive threads, it doese not clean the connection thread

	unsigned int updateDataS();						//Updates the data to send, returns the data to send 
	unsigned int loadObjectPositions();				//Loads the object positions to the dataS buffer
	bool checkActiveObjects();						//Checks if there is a different number of active objects in the game (compared to previous check)
	unsigned int loadCursorPosition();				//Loads the current cursor position to the dataS buffer
	int checkObjectCollisions();					//Checks if any object has a collision currently 

};

