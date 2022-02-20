#include "Server.h"
#include <iostream>


Server::Server()
{
	initWinsoc();
	initSocket();
}

Server::~Server()
{
	closesocket(s);
	std::cout << "[SERVER]: Closed socket" << std::endl;
	WSACleanup();
	std::cout << "[SERVER]: Closed winsocket" << std::endl;
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

void Server::listenS()
{
	listen(s, 3);
	std::cout << "[SERVER]: Waiting for incoming connections..." << std::endl;
	int c = sizeof(struct sockaddr_in);
	new_socket = accept(s, (struct sockaddr*)&client, &c);
	if (new_socket == INVALID_SOCKET)
	{
		printf("[SERVER]: Accept failed with error code : %d", WSAGetLastError());
	}
	std::cout << "[SERVER]: Connection accepted" << std::endl;

}

void Server::sendBuffer(const char* message)
{
	//Reply to client
	send(new_socket, message, strlen(message), 0);
	
}

void Server::recieveBuffer(char* reply)
{
	int size = 0;
	while (!size)
	{
		size = recv(new_socket, reply, 10, 0);
	}
	reply[size] = '\0';
	std::cout << reply << std::endl;
}

void Server::initWinsoc()
{
	std::cout << "[SERVER]: Initialising Winsock Server...\n";
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		std::cout << "[SERVER ERROR]: Failed. Error Code : " << WSAGetLastError() << std::endl;

	}
	std::cout << "[SERVER]: Initialised.\n";
}

void Server::initSocket()
{
	if ((s = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
	{
		std::cout << "[SERVER ERROR]: Could not create socket : " << WSAGetLastError() << std::endl;
	}
	std::cout << "[SERVER]: Socket created.\n";
}