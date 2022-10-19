#include "rcInterface.h"

#define HIT		0
#define MOVE	1
#define SEL		2

RcInterface::RcInterface()
	: receiveX(nullptr), receiveY(nullptr), x_connected(false), y_connected(false), window(nullptr)
{
	dataX[HIT] =  'N';	// No hit key
	dataX[MOVE] = 'S';	// Static movement
	dataX[SEL] =  'N';	// No select key

	dataY[HIT] =  'N';	// No hit key
	dataY[MOVE] = 'S';	// Static movement
	dataY[SEL] =  'N';	// No select key
}

RcInterface::~RcInterface()
{
	finiX();
	finiY();
}

std::string RcInterface::getUsrXInput()
{
	if (window && !x_connected) { updateDataXWithKeyboard(); }
	return std::string(dataX);
}

std::string RcInterface::getUsrYInput()
{
	if (window && !y_connected) { updateDataYWithKeyboard(); }
	return std::string(dataY);
}

void RcInterface::initY(unsigned int port)
{
	std::cout << "[RC INTERFACE]: init Y called.\n";
	serverY.init();
	serverY.bindS(port);								// Binding the specifed port
	if (!receiveY) receiveY = new std::thread(&RcInterface::listenY, this);	// Starting connection dispatcher thread if not set already
}

void RcInterface::initX(unsigned int port)
{
	std::cout << "[RC INTERFACE]: init X called.\n";
	serverX.init();
	serverX.bindS(port);								// Binding the specifed port
	if (!receiveX) receiveX = new std::thread(&RcInterface::listenX, this);	// Starting connection dispatcher thread if not set already
}

void RcInterface::finiX()
{
	std::cout << "[RC INTERFACE] fini X called \n";
	serverX.stop();
	if (receiveX)
	{
		receiveX->join();
		delete receiveX;
		receiveX = nullptr;
	}
}

void RcInterface::finiY()
{
	std::cout << "[RC INTERFACE] fini Y called \n";
	serverY.stop();
	if (receiveY)
	{
		receiveY->join();
		delete receiveY;
		receiveY = nullptr;
	}
}

void RcInterface::listenX()
{
	if (serverX.isActive() && serverX.listenS())			// If succesful connection continue
	{
		x_connected = true;
		receptionX();
	}
}

void RcInterface::listenY()
{
	while (serverY.isActive() && serverY.listenS())			// If succesful connection continue
	{
		y_connected = true;
		receptionY();
	}
}

void RcInterface::receptionX()
{
	while (serverX.isActive())
	{
		char datum = '0';
		if (serverX.recieveBuffer(&datum))	processByteX(datum);
		else break;			// If client dosen't send bytes within the timout we consider a disconnected client
	}
	x_connected = false;
}

void RcInterface::receptionY()
{
	while (serverY.isActive())
	{
		char datum = '0';
		if (serverY.recieveBuffer(&datum, 1))	processByteY(datum);
		else break;			// If client dosen't send bytes within the timout we consider a disconnected client
	}
	y_connected = false;
}

void RcInterface::processByteX(char datum)
{
	// dataX format: <hit key, movement, select key>
	switch (datum)
	{
	

	case 'R':			// Right
		std::cout << "Right\n";
		dataX[MOVE] = 'R';
		dataX[SEL] = 'N';
		dataX[HIT] = 'N';
		break;
	case 'L':			// Left
		dataX[MOVE] = 'L';
		dataX[SEL] = 'N';
		dataX[HIT] = 'N';
		break;
	case 'S':			// Static
		dataX[MOVE] = 'S';
		dataX[SEL] = 'N';
		dataX[HIT] = 'N';
		break;
	case 'T':			// Select taget key
		dataX[SEL] = 'Y'; 
		dataX[HIT] = 'N';
		break;
	case 'E':			// Hit key
		dataX[HIT] = 'Y';
		dataX[SEL] = 'N';
		break;

	default:			// Not matchin character
		std::cout << "[RC CONTROL X]: Received byte not supported. \n";
		break;
	}
}

void RcInterface::processByteY(char datum)
{
	// dataX format: <hit key, movement, select key>
	// std::cout << datum << std::endl;
	switch (datum)
	{
	case 'U':			// Up
		dataY[MOVE] = 'U';
		dataY[SEL] = 'N';
		dataY[HIT] = 'N';
		break;
	case 'D':			// Down
		dataY[MOVE] = 'D';
		dataY[SEL] = 'N';
		dataY[HIT] = 'N';
		break;
	case 'S':			// Static
		dataY[1] = 'S';
		dataY[SEL] = 'N';
		dataY[HIT] = 'N';
		break;
	case 'T':			// Select taget key
		dataY[2] = 'Y';
		dataY[HIT] = 'N';
		break;
	case 'E':			// Hit key
		dataY[HIT] = 'Y';
		dataY[SEL] = 'N';
		break;
	default:			// Not matchin character
		std::cout << "[RC CONTROL Y]: Received byte not supported. \n";
		break;
	}
}

void RcInterface::updateDataXWithKeyboard()
{
	dataX[0] = 'N';	// No hit key
	dataX[1] = 'S'; // Static movement
	dataX[2] = 'N';	// No select key

	int state0 = glfwGetKey(window, GLFW_KEY_LEFT);			// Left key
	int state1 = glfwGetKey(window, GLFW_KEY_RIGHT);		// Right key
	int state2 = glfwGetKey(window, GLFW_KEY_Y);			// Select key
	int state3 = glfwGetKey(window, GLFW_KEY_ENTER);		// Select key

	// TODO: hit key 
	// Movement
	if (state0 == GLFW_PRESS)		dataX[MOVE] = 'L';	// Left
	else if (state1 == GLFW_PRESS)	dataX[MOVE] = 'R';  // Right
	else							dataX[MOVE] = 'S';	// Static
	// Selection
	if (state2 == GLFW_PRESS)		dataX[SEL] = 'Y';	// Select key on
	else							dataX[SEL] = 'N';	// Select key off

	// TODO: hit key
	if (state3 == GLFW_PRESS)		dataX[HIT] = 'Y';
	else							dataX[HIT] = 'N';

}

void RcInterface::updateDataYWithKeyboard()
{
	dataY[0] = 'N';	// No hit key
	dataY[1] = 'S'; // Static movement
	dataY[2] = 'N';	// No select key

	int state0 = glfwGetKey(window, GLFW_KEY_W);		// Left key
	int state1 = glfwGetKey(window, GLFW_KEY_S);		// Right key
	int state2 = glfwGetKey(window, GLFW_KEY_T);		// Select key
	int state3 = glfwGetKey(window, GLFW_KEY_SPACE);		// Select key
	// TODO: hit key 

	// Movement
	if (state0 == GLFW_PRESS)		dataY[MOVE] = 'U';	// Up
	else if (state1 == GLFW_PRESS)	dataY[MOVE] = 'D';  // Down
	else							dataY[MOVE] = 'S';	// Static
	// Selection						
	if (state2 == GLFW_PRESS)		dataY[SEL] = 'Y';	// Select key on
	else							dataY[SEL] = 'N';	// Select key off

	// TODO: hit key
	if (state3 == GLFW_PRESS)		dataY[HIT] = 'Y';
	else							dataY[HIT] = 'N';
}
