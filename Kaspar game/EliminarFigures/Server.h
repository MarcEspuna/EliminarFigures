#pragma once
#include<stdio.h>
#include<winsock2.h>

class Server
{
public:

	Server();
	~Server();

	void init();
	void bindS(const unsigned int& port, const unsigned long& address);
	void bindS(const unsigned int& port);
	bool listenS(const unsigned int& connections = 1);
	void sendBuffer(const char* message, unsigned int size);
	void recieveBufferWait(char* reply);
	bool recieveBuffer(char* reply, int len = 4);
	void stop();
	inline bool isActive() { return active; }

private:
	bool active;							//Server is active
	WSADATA wsa;							//Winsocket
	SOCKET s, new_socket;					//Actual socket
	sockaddr_in server, client;				//Struct used for: 
	const char* message;					
	int revc_size;

	void initWinsoc();
	void initSocket();

};

