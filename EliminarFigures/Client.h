#pragma once
#include <stdio.h>
#include <winsock2.h>
#include <thread>

class Client
{
public:
	Client();
	~Client();

	void connectS(const unsigned int& port, const char* address = "127.0.0.1");
	void sendBuffer(const char* message, unsigned int size);
	void recieveBuffer(char* reply, int maxSize);
	bool isConnected();

private:
	WSADATA wsa;						//Winsocket
	SOCKET s;							//Actual socket
	sockaddr_in server;					//Server data
	char server_reply[15];				//Should be 14 bytes, agreed protocool;
	int revc_size;						//Size of the recieved data, should be 14 bytes

	bool connected;

	void initWinsoc();
	void initSocket();



};

