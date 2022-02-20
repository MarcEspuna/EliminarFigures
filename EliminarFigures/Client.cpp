#include "Client.h"
#include <iostream>

Client::Client()
{
	initWinsoc();
	initSocket();
}


Client::~Client()
{

}

void Client::connectS(const unsigned int& port)
{
	server.sin_addr.s_addr = inet_addr("127.0.0.1");						//Connecting to localhost address
	server.sin_family = AF_INET;
	server.sin_port = htons(port);

	//Connect to remote server
	if (connect(s, (struct sockaddr*)&server, sizeof(server)) < 0)
	{
		std::cout << "[CLIENT ERROR]: Error connecting to server" << std::endl;
	}
	std::cout << "[CLIENT]: Successfully connected to server" << std::endl;
}

void Client::sendBuffer(const char* message)
{
	send(s, message, strlen(message), 0);
}

void Client::recieveBuffer(char* reply, int maxSize)
{
	size_t size = 0;
	while (!size)
	{
		size = recv(s, reply, maxSize, 0);
	}
	std::cout << size << std::endl;
	std::cout << reply << std::endl;
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
