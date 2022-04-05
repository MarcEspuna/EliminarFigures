#include "UsrInterface.h"
#include <iostream>
UsrInterface::UsrInterface()
	: usrCommand({0.0f, 0.0f, 0.0f}), receive(nullptr), connect(new std::thread(&UsrInterface::connect, this)), active(false)
{
	server.bindS(12000);
}

UsrInterface::~UsrInterface()
{
	std::cout << "[USER INTERFACE]: DESTRUCTOR\n";
	server.stop();
	active = false;
	connect->join();
	delete connect;
	if (receive)
	{
		receive->join();
		delete receive;
	}
}


glm::vec3 UsrInterface::getUsrInput()
{
	return usrCommand;
}

void UsrInterface::reception()
{
	while (active)
	{
		char usrInputData[1] = { 'X' }; // <MOVEMENT: R;L;U;D, SELECTION: Y;N>
		server.recieveBuffer(usrInputData);
		switch (usrInputData[0])
		{
		case 'R':
			usrCommand.x = 1.0f;
			break;
		case 'L':
			usrCommand.x = -1.0f;
			break;
		case 'S':
			usrCommand.x = 0.0f;
			break;
		case 'T':
			usrCommand.z = 1.0f;
			break;
		case 'E':
			break;
		default:
			break;
		}
	}
}

void UsrInterface::connection()
{
	if (server.listenS())
	{
		active = true;
		receive = new std::thread(&UsrInterface::reception, this);
	}
}
