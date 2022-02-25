#pragma once
#include "Client.h"
#include "Server.h"
#include "Logger.h"
#include <thread>
class DataLink
{
public:
	DataLink();
	~DataLink();

	//X will be user for now
	void cursorLeft();
	void cursorRight();
	//Y will be AI for now
	void cursorUp();
	void cursorDown();
	//Stoped cursor
	void aiCursorStoped();
	void usrCursorStoped();
	
	void setTargetObject(const unsigned int& id);
	void figureErased();
	void figureNotErased();
	void setRemainingFigures(const unsigned int& objectsLeft);
	void overFigure();
	void notOverFigure();

private:

	char data[15];
	Server server;
	Logger log;
	//std::thread outputLink;
	//std::thread inputLink;


	void loadDefaultData();
	void refreshTimestamp();
	

};

