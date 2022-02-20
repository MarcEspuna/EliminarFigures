#pragma once
#include <stdio.h>
#include <winsock2.h>

class Client
{
public:
	Client();
	~Client();

	void connectS(const unsigned int& port);
	void sendBuffer(const char* message);
	void recieveBuffer(char* reply, int maxSize);


private:
	WSADATA wsa;						//Winsocket
	SOCKET s, new_socket;							//Actual socket
	sockaddr_in server, client;					//Struct used for: 
	const char* message;
	char server_reply[4000];
	int revc_size;

	void initWinsoc();
	void initSocket();
};

