#include "AiInterface.h"
#include "Server.h"

template <typename T> 
static int sgn(T val) {
	return (T(0) < val) - (val < T(0));
}

AiInterface::AiInterface()
	: m_Cursor(nullptr), receive(nullptr), send(nullptr), userPressedKey(nullptr), 
	dataS(nullptr), activeObjects(0), clientConnected(false), initialized(false),
	connect(nullptr), userSelectKey(nullptr)
{
	dataS = new char[11];							//Cursor Label ('C') Position cursor : 2 Floats(x pos, y pos), overFigure : 1 byte (Y, N), userHitKey : 1 byte (Y, N)
}

void AiInterface::init(unsigned int port)
{
	std::cout << "[AIinterface]: init called\n";
	server.init();
	server.bindS(port);
	connect = new std::thread(&AiInterface::connectionManager, this);
	initialized = true;
}

void AiInterface::fini()
{
	std::cout << "[AIinterface]: fini called\n";
	if (server.isActive())	server.stop();
	if (connect) {
		connect->join();		//We wait for the connection thread to stop
		cleanThreads();
		delete connect;
		connect = nullptr;
	}
	if (dataS) {
		delete dataS;
		dataS = nullptr;
	}
	// Super important to reset all the important pointers and values
	initialized = false;
	clear();
}

void AiInterface::clear()
{
	m_Cursor = nullptr;
	activeObjects = 0;
	//clientConnected = false;
	userPressedKey = nullptr;
	m_Objects.clear();
}

AiInterface::~AiInterface()
{
	fini();
}

void AiInterface::setCursor(const Object* object)
{
	m_Cursor = object->getCollider();
}

/// <summary>
/// Very important to set up a mutex in case we are loading objects at the same time as transmitting!
/// </summary>
/// <param name="objects"></param>
void AiInterface::setObjects(const std::vector<Object*> objects)
{
	for (const auto& object : objects)
	{
		m_Objects.push_back(object);
	}
	if (dataS)
	{
		delete dataS;									// TODO: FIX, We need a mutex here 
		dataS = nullptr;
	}
	dataS = new char[1 + m_Objects.size() * 12];		//Identifier : 'O', Object id and positions : Each object 3 floats (id, xcoord, ycoord) 
}

void AiInterface::gameStart(const std::string desc[5])
{
	std::string buffer;
	buffer.push_back('I');
	buffer.push_back('X');
	for (int i = 0; i < 5; i++)
	{
		buffer.append(desc[i]);
		buffer.push_back('#');
	}
	buffer[1] = buffer.size() - 2;	// Minus header and size
	std::cout << "[AI Interface]: obj decriptions: " << buffer << std::endl;
	if (isConnected()) server.sendBuffer(buffer.c_str(), buffer.size());		// Send description to Ai interface
	std::cout << "[AI interface]: final size: " << buffer.size() << std::endl;
}

void AiInterface::gameEnd()
{
	if (isConnected()) {
		server.sendBuffer("F", 1);
		std::cout << "[AiInterface]: game ended byte sent.\n";
	}
}

std::string AiInterface::getAiInput()
{
	std::string desicion = { dataR[0], dataR[1], dataR[2] , dataR[3]};	//We make a copy into a string
	return desicion;
}

void AiInterface::setUserPressedKey(const bool* userKey)
{
	userPressedKey = userKey;
}

void AiInterface::setUserSelectKey(const bool* userSelect)
{
	userSelectKey = userSelect;
}

void AiInterface::transmitStatus()
{
	int size = loadObjectPositions();
	server.sendBuffer(dataS, size);
}


void AiInterface::connectionManager()
{
	if (server.listenS(1))
	{
		clientConnected = true;
		//Successfull connection
		cleanThreads();
		//If there is a successful incomming connection, we start the transmition thread and reception thread
		receive = new std::thread(&AiInterface::reception, this);
		send = new std::thread(&AiInterface::transmition, this);
	}
}

/* Manages the reception data from the aiPlayer module, if it's not connected starts a thread that waits for new connections */
void AiInterface::reception()
{
	while (server.recieveBuffer(dataR));
	if (server.isActive())
	{
		connect->join();
		delete connect;
		connect = new std::thread(&AiInterface::connectionManager, this);
	}
	std::cout << "[AiInterface]: Client disconnected\n";
	clientConnected = false;
}

void AiInterface::transmition()
{
	std::cout << "[AiInterface]: Starting data transmition" << std::endl;
	while (server.isActive())	
	{
		if (m_Cursor) {			// Once the cursor is set we start updating and transmitting
			unsigned int size = updateDataS();
			Sleep(1);
			server.sendBuffer(dataS, size);
		}
		Sleep(1);
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
	dataS[0] = 'O';						//We mark the transmition as definning object positions
	int ptrIndex = 1;
	if (m_Cursor && m_Objects.size() > 0)
	{
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
	}
	return ptrIndex;
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

		dataS[ptrIndex++] = checkObjectCollisions();						// We load the id of the collided object, if no object id = 0
		// Updating if user pressed it's hit key
		if (userPressedKey)
		{
			if (*userPressedKey)
				dataS[ptrIndex++] = 'Y';
			else
				dataS[ptrIndex++] = 'N';
		}
		else
			dataS[ptrIndex++] = 'X';
		// Updating if user pressed the select key
		if (userSelectKey) {
			if (*userSelectKey)
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

int AiInterface::checkObjectCollisions()
{
	for (const auto& object : m_Objects)
	{
		if (object->collided())
			return object->getId();
	}
	return 0;
}

