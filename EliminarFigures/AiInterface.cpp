#include "AiInterface.h"
#include "Server.h"

template <typename T> 
static int sgn(T val) {
	return (T(0) < val) - (val < T(0));
}


AiInterface::AiInterface()
	: m_Cursor(nullptr), receive(nullptr), send(nullptr), userPressedKey(nullptr), dataS(nullptr), activeObjects(0)
{
	std::memset(dataR, 0, sizeof(dataR));
	server.bindS(8888);
	connect = new std::thread(&AiInterface::connectionManager, this);
}

AiInterface::~AiInterface()
{
	server.stop();		//Will stop the server socket
	connect->join();			//We wait for the connection thread to stop
	cleanThreads();
	delete connect;
	delete dataS;
}

void AiInterface::setCursor(const Object* object)
{
	m_Cursor = object->getCollider();
	dataS = new char[11];							//Cursor Label ('C') Position cursor : 2 Floats(x pos, y pos), overFigure : 1 byte (Y, N), userHitKey : 1 byte (Y, N)
}

void AiInterface::setObjects(const std::vector<Object*> objects)
{
	for (const auto& object : objects)
	{
		m_Objects.push_back(object);
	}
	if (dataS)
	{
		delete dataS;
	}
	dataS = new char[1 + m_Objects.size() * 12];		//Identifier : 'O', Object id and positions : Each object 3 floats (id, xcoord, ycoord) 
}

glm::vec3 AiInterface::getAiInput()
{
	glm::vec3 desicion(0.0f);
	if (dataR[0] == 'R')
		desicion.x = 1.0f;
	else if (dataR[0] == 'L')
		desicion.x = -1.0f;
	if (dataR[1] == 'U')
		desicion.y = 1.0f;
	else if (dataR[1] == 'D')
		desicion.y = -1.0f;
	if (dataR[2] == 'Y')
		desicion.z = 1.0f;
	return desicion;
}

void AiInterface::setUserPressedKey(const bool* userKey)
{
	userPressedKey = userKey;
}

void AiInterface::connectionManager()
{
	if (server.listenS(1))
	{
		//Successfull connection
		//Always send objects when starting connection
		int size = loadObjectPositions();
		server.sendBuffer(dataS, size);

		cleanThreads();
		//If there is a successful incomming connection, we start the transmition thread and reception thread
		receive = new std::thread(&AiInterface::reception, this);
		send = new std::thread(&AiInterface::transmition, this);
	}
}

void AiInterface::reception()
{
	size_t waitingTime = 0;
	while (server.recieveBuffer(dataR));
	if (server.isActive())
	{
		connect->join();
		delete connect;
		connect = new std::thread(&AiInterface::connectionManager, this);
	}
}

void AiInterface::transmition()
{
	std::cout << "[AiInterface]: Starting data transmition" << std::endl;
	while (server.clientConnected())
	{
		unsigned int size = updateDataS();
		Sleep(5);
		server.sendBuffer(dataS, size);
	}
	std::cout << "[AiInterface]: Sutting down transmition..." << std::endl;
}

void AiInterface::cleanThreads()
{
	if (receive)
	{
		receive->join();
		delete receive;
		receive = nullptr;
	}
	if (send)
	{
		send->join();
		delete send;
		send = nullptr;
	}
}


unsigned int AiInterface::updateDataS()
{
	if (checkActiveObjects())
		return loadCursorPosition();
	else
		return loadObjectPositions();
}

unsigned int AiInterface::loadObjectPositions()
{
	if (m_Cursor && m_Objects.size() > 0)
	{
		dataS[0] = 'O';						//We mark the transmition as definning object positions
		int ptrIndex = 1;
		for (const auto& object : m_Objects)
		{
			if (object->isActive())
			{
				int id = object->getId();
				glm::vec2 position = object->getCentralPosition();
				std::memcpy(&dataS[ptrIndex], &id, sizeof(int));
				ptrIndex += sizeof(int);
				std::memcpy(&dataS[ptrIndex], &position.x, sizeof(float));
				ptrIndex += sizeof(float);
				std::memcpy(&dataS[ptrIndex], &position.y, sizeof(float));
				ptrIndex += sizeof(float);
			}
		}
		return ptrIndex;
	}
	else
		return 0;
}

bool AiInterface::checkActiveObjects()
{
	unsigned int currentActiveObjects = 0;
	for (const auto& object : m_Objects)
	{
		if (object->isActive())
			currentActiveObjects++;
	}
	if (currentActiveObjects != activeObjects)
	{
		activeObjects = currentActiveObjects;
		return false;
	}
	return true;
}

unsigned int AiInterface::loadCursorPosition()
{
	if (m_Cursor && m_Objects.size() > 0)
	{
		int ptrIndex = 0;
		dataS[ptrIndex++] = 'C';
		glm::vec2 position = m_Cursor->getPosition();
		std::memcpy(&dataS[ptrIndex], &position, sizeof(position));
		ptrIndex += sizeof(position);
		if (checkObjectCollisions())	//Fix
			dataS[ptrIndex++] = 'Y';
		else
			dataS[ptrIndex++] = 'N';
		if (userPressedKey)
		{
			if (*userPressedKey)
				dataS[ptrIndex++] = 'Y';
			else
				dataS[ptrIndex++] = 'N';
		}
		else
			dataS[ptrIndex++] = 'X';
		return ptrIndex;
	}
	else
		return 0;
}

bool AiInterface::checkObjectCollisions()
{
	for (const auto& object : m_Objects)
	{
		if (object->collided())
			return true;
	}
	return false;
}

