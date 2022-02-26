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
	inline glm::vec2 getAiInput() { return aiCommand; }
	void setUserPressedKey(const bool* userKey);

private:

	const SquareCollider* m_Cursor;
	std::vector<const Object*> m_Objects;
	const bool* userPressedKey;
	unsigned int activeObjects;

	glm::vec2 aiCommand;							//Latest stored data of the decision of the ai
	
	Server server;									//Server of the aplication
	std::thread* receive;							//Thread that will manage the incomming connections
	std::thread* send;								//Thread that will send the cursor or object positions
	std::thread* connect;							//Thread that will receive incomming data and store it

	char* dataS;									//Data to send  ('O', <object positions>) or ('C', <cursor position>, overFigure, userHitKey)
	char dataR[3];									//Data received (x axis: int_8, y axis: int_8, correct initial reception: bool)

	void connectionManager();						//Listens and manages incomming connections
	void reception();								//Reads incomming data and stores it, also it's responsible to determine if the client is still connected
	void transmition();								//Transmits relevant data periodically
	void cleanThreads();

	unsigned int updateDataS();
	unsigned int loadObjectPositions();
	bool checkActiveObjects();
	unsigned int loadCursorPosition();
	bool checkObjectCollisions();

};

