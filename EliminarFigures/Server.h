#pragma once
#include<stdio.h>
#include<winsock2.h>


class Server
{
public:

	Server();
	~Server();

	void bindS(const unsigned int& port, const unsigned long& address = INADDR_ANY);
	void listenS();
	void sendBuffer(const char* message);
	void recieveBuffer(char* reply);


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

