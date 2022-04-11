#include "Server.h"
#include <thread>
#include <iostream>

Server::Server()
{
	initWinsoc();
	initSocket();
}

Server::~Server()
{
	if (active)
	{
		closesocket(s);
		std::cout << "[SERVER]: Closed socket" << std::endl;
		WSACleanup();
		std::cout << "[SERVER]: Closed winsocket" << std::endl;
		active = false;
	} 
	std::cout << "[SERVER]: Shutting down..." << std::endl;
}

void Server::init()
{
	if (!active) {
		initWinsoc();
		initSocket();
	}
}

void Server::bindS(const unsigned int& port, const unsigned long& address)
{
	//Prepare the sockaddr_in structure
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = address;
	server.sin_port = htons(port);

	//Bind
	if (bind(s, (struct sockaddr*)&server, sizeof(server)) == SOCKET_ERROR)
	{
		std::cout << "[SERVER ERROR]: Bind failed with error code : " << WSAGetLastError() << std::endl;
	}
	std::cout << "[SERVER]: Bind done.\n";
}

void Server::bindS(const unsigned int& port)
{
	//Prepare the sockaddr_in structure
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons(port);
	//Bind
	if (bind(s, (struct sockaddr*)&server, sizeof(server)) == SOCKET_ERROR)
	{
		std::cout << "[SERVER ERROR]: Bind failed with error code : " << WSAGetLastError() << std::endl;
	}
	std::cout << "[SERVER]: Bind done.\n";
}


bool Server::listenS(const unsigned int& connections)
{
	listen(s, connections);
	std::cout << "[SERVER]: Waiting for incoming connections..." << std::endl;
	int c = sizeof(struct sockaddr_in);
	new_socket = accept(s, (struct sockaddr*)&client, &c);
	if (new_socket == INVALID_SOCKET)
	{
		int error = WSAGetLastError();
		switch (error)
		{
		case WSAEINTR:
			std::cout << "[SERVER]: Interruption function call, stopping listening for connections..." << std::endl;
			break;
		default:
			std::cout << "[SERVER]: Accept failed with error code " << error << std::endl;
			break;
		}
		return false;
	}
	std::cout << "[SERVER]: Connection accepted" << std::endl;
	return true;
}

void Server::sendBuffer(const char* message, unsigned int size)
{
	send(new_socket, message, size, 0);
	//std::cout << "[SERVER]: Message sent" << std::endl;
}

void Server::recieveBufferWait(char* reply)
{
	std::cout << "[SERVER]: Waiting for message\n";
	int size = 0;
	while (!size)
	{
		size = recv(new_socket, reply, 2, 0);
	}
	reply[size] = '\0';
	std::cout << "[SERVER]: Message recieved: " << reply << std::endl;
}

/* It checks if a massage has been received for a period of time, if nothing is received it returns false */
bool Server::recieveBuffer(char* reply)
{
	size_t timeout = 0;
	while (timeout < 5)
	{
		int size = recv(new_socket, reply, 4, 0);
		if (size > 0)
		{
			return true;
		}
		else
			timeout++;
	}
	std::cout << "[SERVER]: Client disconnected" << std::endl;
	return false;
}

void Server::stop()
{
	closesocket(s);
	std::cout << "[SERVER]: Closed socket" << std::endl;
	WSACleanup();
	std::cout << "[SERVER]: Closed winsocket" << std::endl;
	active = false;
}



void Server::initWinsoc()
{
	std::cout << "[SERVER]: Initialising Winsock Server...\n";
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		std::cout << "[SERVER ERROR]: Failed. Error Code : " << WSAGetLastError() << std::endl;
		return;
	}
	std::cout << "[SERVER]: Initialised.\n";
}

void Server::initSocket()
{
	if ((s = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
	{
		std::cout << "[SERVER ERROR]: Could not create socket : " << WSAGetLastError() << std::endl;
		return;
	}
	std::cout << "[SERVER]: Socket created.\n";
	active = true;
}