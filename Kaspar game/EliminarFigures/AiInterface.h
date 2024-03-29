#pragma once
#include "glm/glm.hpp"
#include "Object.h"
#include "SquareCollider.h"
#include "Server.h"
#include <thread>
#include <mutex>
#include <string>

class AiInterface
{
public:
	AiInterface();
	~AiInterface();

	void setCursor(const Object* object);
	void setObjects(const std::vector<Object*> objects);
	void gameStart(const std::string desc[5]);
	void gameEnd();
	std::string getAiInput();
	void setUserPressedKey(const bool* userKey);
	void setUserSelectKey(const bool* userSelect);
	void transmitStatus();									// Sends the current data through the socket
	void init(unsigned int port);
	void fini();
	void clear();
	inline bool isConnected() { return clientConnected; }
	inline bool isActive() { return initialized; }

private:

	const SquareCollider* m_Cursor;
	std::vector<const Object*> m_Objects;
	const bool* userPressedKey;
	const bool* userSelectKey;
	unsigned int activeObjects;						
	
	Server server;									//Server of the aplication
	std::thread* receive;							//Thread that will manage the incomming connections
	std::thread* send;								//Thread that will send the cursor or object positions
	std::thread* connect;							//Thread that will receive incomming data and store it

	char* dataS;									//Data to send  ('O', <object positions>) or ('C', <cursor position>, <overFigure> id, userHitKey)
	char dataR[4] = { 'S','S','S','S' };									//Data received (x axis: int_8, y axis: int_8, correct initial reception: bool)

	void connectionManager();						//Listens and manages incomming connections
	void reception();								//Reads incomming data and stores it, also it's responsible to determine if the client is still connected
	void transmition();								//Transmits relevant data periodically
	void cleanThreads();							//Used to clean send and receive threads, it doese not clean the connection thread

	unsigned int updateDataS();						//Updates the data to send, returns the data to send 
	unsigned int loadObjectPositions();				//Loads the object positions to the dataS buffer
	bool checkActiveObjects();						//Checks if there is a different number of active objects in the game (compared to previous check)
	unsigned int loadCursorPosition();				//Loads the current cursor position to the dataS buffer
	int checkObjectCollisions();					//Checks if any object has a collision currently 

	bool clientConnected;
	bool initialized;
};

