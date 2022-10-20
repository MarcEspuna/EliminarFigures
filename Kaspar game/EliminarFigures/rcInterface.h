#pragma once

#include <iostream>
#include <thread>
#include "Server.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>

/// <summary>
/// Remote control interface.
/// </summary>
class RcInterface
{
public:
	RcInterface();
	~RcInterface();

	std::string getUsrXInput();
	std::string getUsrYInput();

	void initX(unsigned int port);				// Initializes server and thread X axis
	void initY(unsigned int port);				// Initializes server and thread Y axis
	void finiX();								// Finalizes server and thread X axis
	void finiY();								// Finalizes server and thread Y axis

	inline bool isUsrXConnected() { return x_connected; }
	inline bool isUsrYConnected() { return y_connected; }

	inline void setWindow(GLFWwindow* input) { window = input; };

	inline bool isYActive() { return serverY.isActive(); }
	inline bool isXActive() { return serverX.isActive(); }

private:
	GLFWwindow* window;

	std::thread* receiveX;						// Thread used for handling data reception of X rc controller
	std::thread* receiveY;						// Thread used for handling data reception of Y rc controller

	char dataX[3];								// Data buffer where we store all the recieved informaction of X axis user
	char dataY[3];								// Data buffer where we store all the recieved informaction of Y axis user

	Server serverX;								// Server of the X axis control
	Server serverY;								// Server of the Y axis control

	bool x_connected;							// Indicates if client of x axis rc control is connected
	bool y_connected;							// Indicates if client of y axis rc control is connected

	void listenX();								// Method used to wait for incomming connection on the specifed server (port) and start reception if successful
	void listenY();								// Method used to wait for incomming connection on the specifed server (port) and start reception if successful

	void receptionX();							// Method used to receive incomming data and store to dataX buffer
	void receptionY();							// Method used to receive incomming data and store to dataY buffer
	
	void processByteX(char datum);				// Updates main bufferX based on the received byte
	void processByteY(char datum);				// Updates main bufferY based on the received byte

	void updateDataXWithKeyboard();				// Updates the data buffer X by looking at the pressed keys
	void updateDataYWithKeyboard();				// Updates the data buffer Y by looking at the pressed keys

};

