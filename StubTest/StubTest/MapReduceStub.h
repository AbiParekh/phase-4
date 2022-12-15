#pragma once
#pragma once
#include <string>
#include "../common/Phase4Messages.h"
#include "../SockerCode/Sockets.h"


class MapReduceStub
{
public:
	MapReduceStub(uint32_t portIn, uint32_t Identiciation) :
		port(portIn),
		ipAddress("localHost"),
		ID(Identiciation)
	{}

	uint32_t port;
	std::string ipAddress;
	uint32_t ID;

	bool startServer();
};

class stubHandler
{
public:
	void operator()(Sockets::Socket& socket_);
	bool processMessage(Sockets::Socket& socket_);
};

