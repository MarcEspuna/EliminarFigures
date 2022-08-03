#include "DataLink.h"
#include <string>
#include <chrono>

#define TS 0				//Timestamp 8 bytes
#define RM 8				//Index data defines robot movement
#define PM 9				//Index data defines person movement
#define TO 10				//Index data defines target object
#define EF 11				//Erased figure
#define RF 12				//Remainning figures
#define OF 13				//Over figure


DataLink::DataLink()
{
	server.bindS(8888);				//Default is localhost	
	server.listenS();
	//Load default data
	loadDefaultData();
}


DataLink::~DataLink()
{
	
}

void DataLink::cursorLeft()
{
	refreshTimestamp();
	if (data[PM] != 'L')
	{
		data[PM] = 'L';
		server.sendBuffer(data, 14);
	}
}

void DataLink::cursorRight()
{
	refreshTimestamp();
	if (data[PM] != 'R')
	{
		data[PM] = 'R';
		server.sendBuffer(data, 14);
	}
}

void DataLink::cursorUp()
{
	refreshTimestamp();
	if (data[RM] != 'U')
	{
		data[RM] = 'U';
		server.sendBuffer(data, 14);
	}
}

void DataLink::cursorDown()
{
	refreshTimestamp();
	if (data[RM] != 'D')
	{
		data[RM] = 'D';
		server.sendBuffer(data, 14);
	}
}

void DataLink::aiCursorStoped()
{
	if (data[RM] != 'S')
	{
		data[RM] = 'S';
		server.sendBuffer(data, 14);
	}
}

void DataLink::usrCursorStoped()
{
	refreshTimestamp();
	if (data[PM] != 'S')
	{
		data[PM] = 'S';
		server.sendBuffer(data, 14);
	}
}

void DataLink::setTargetObject(const unsigned int& objId)
{
	refreshTimestamp();
	std::string id = std::to_string(objId);
	if (data[TO] != id[0])
	{
		data[TO] = id[0];
		server.sendBuffer(data, 14);
	}
}

//NOT REALLY NEEDED
void DataLink::figureErased()
{
	refreshTimestamp();
	if (data[EF] != 'Y')
	{
		data[EF] = 'Y';
		server.sendBuffer(data, 14);
	}
}

void DataLink::figureNotErased()
{
	refreshTimestamp();
	if (data[EF] != 'N')
	{
		data[EF] = 'N';
		server.sendBuffer(data, 14);
	}
}

void DataLink::setRemainingFigures(const unsigned int& objectsLeft)
{
	refreshTimestamp();
	std::string remaining = std::to_string(objectsLeft);
	if (data[RF] != remaining[0])
	{
		data[RF] = remaining[0];
		server.sendBuffer(data, 14);
	}
}

void DataLink::overFigure()
{
	refreshTimestamp();
	if (data[OF] != 'Y')
	{
		data[OF] = 'Y';
		server.sendBuffer(data, 14);
	}
}

void DataLink::notOverFigure()
{
	refreshTimestamp();
	if (data[OF] != 'N')
	{
		data[OF] = 'N';
		server.sendBuffer(data, 14);
	}
}

void DataLink::loadDefaultData()
{
	refreshTimestamp();
	data[RM] = 'S';
	data[PM] = 'S';
	data[TO] = '0';
	data[EF] = 'N';
	data[RF] = '5';
	data[OF] = 'N';
	data[14] = '\0';
	server.sendBuffer(data, 14);
}

void DataLink::refreshTimestamp()
{
	double timestamp = 
		std::chrono::duration_cast<std::chrono::seconds>
		(std::chrono::system_clock::now().time_since_epoch()).count();
	std::memcpy(data, &timestamp, sizeof(timestamp));			//Copying timestamp to data buffer
}
