#pragma once
#include "glm/glm.hpp"
#include "Server.h"
#include <thread>
class UsrInterface
{
public:
	UsrInterface();
	~UsrInterface();

	glm::vec3 getUsrInput();


private:
	
	glm::vec3 usrCommand;
	bool active;
	Server server;
	std::thread* receive;
	std::thread* connect;
	void reception();
	void connection();

};

