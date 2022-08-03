#include "Client.h"
#include <iostream>

Client::Client()
	: connected(false)
{
	initWinsoc();
	initSocket();
}

Client::~Client()
{

}

void Client::connectS(const unsigned int& port, const char* address)
{
	server.sin_addr.s_addr = inet_addr(address);						//Connecting to localhost address
	server.sin_family = AF_INET;
	server.sin_port = htons(port);

	//Connect to remote server
	if (connect(s, (struct sockaddr*)&server, sizeof(server)) < 0)
	{
		std::cout << "[CLIENT ERROR]: Error connecting to server" << std::endl;
	}
	else
	{
		connected = true;
		std::cout << "[CLIENT]: Successfully connected to server" << std::endl;
	}
	
}

void Client::sendBuffer(const char* message, unsigned int size)
{

	if (connected)
	{
		send(s, message, size, 0);
		std::cout << "[CLIENT]: Message sent" << std::endl;
	}
	else
	{
		std::cout << "[CLIENT ERROR]: Data can't be sent, not connected to server\n";
	}
}

void Client::recieveBuffer(char* reply, int maxSize)
{
	if (connected)
	{
		size_t size = 0;
		while (!size)
		{
			size = recv(s, reply, maxSize, 0);
		}
	}
	else
	{
		std::cout << "[CLIENT ERROR]: Can't recieve data, not connected to server\n";
	}
}

bool Client::isConnected()
{
	return connected;
}

void Client::initWinsoc()
{
	std::cout << "[CLIENT]: Initialising Winsock Client...\n";
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		std::cout << "[CLIENT ERROR]: Failed. Error Code : " << WSAGetLastError() << std::endl;

	}
	std::cout << "[CLIENT]: Initialised.\n";
}

void Client::initSocket()
{
	if ((s = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
	{
		std::cout << "[CLIENT ERROR]: Could not create socket : " << WSAGetLastError() << std::endl;
	}
	std::cout << "[CLIENT]: Socket created.\n";
}
