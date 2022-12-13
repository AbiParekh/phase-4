#pragma once
#include <string>
#include "../common/Phase4Messages.h"
#include "../SockerCode/Sockets.h"
#include "../SockerCode/Logger.h"
#include "../SockerCode/StringUtilities.h"
#include "MapReduceWF.h"


using Show = StaticLogger<1>;



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

	template<typename CallObj>
	bool startServer(CallObj& SH, Sockets::SocketConnecter& SI);

	template<typename CallObj>
	bool startProcessor(CallObj& SH, Sockets::SocketConnecter& SI, std::string procID,std::string args);

	bool startServer();
};

class stubHandler
{
public:
	void operator()(Sockets::Socket& socket_);
	bool processMessage(Sockets::Socket& socket_);
};


template<typename CallObj>
bool MapReduceStub::startServer(CallObj& sh, Sockets::SocketConnecter& si)
{
	Sockets::SocketSystem ss;

	Sockets::SocketListener sl(port, Sockets::Socket::IP6);

	//stubHandler sh;

	sl.start(sh);

	while (!si.connect("localhost", 9071))
	{
		Show::write("\n  client waiting to connect");
		::Sleep(100);
	}

	Show::title("Starting string test on client");
	clientTestStringHandling(std::ref(si));

	////////////////////////////////////////////////////
	// This buffer handling test doesn't work yet.
	// I'll fix when time permits.
	//
	// Show::title("Starting buffer test on client");
	// clientTestBufferHandling(si);

	/*

	*/

	return true;
}

template<typename CallObj>
bool MapReduceStub::startProcessor(CallObj& sh, Sockets::SocketConnecter& si, const std::string proc, const std::string configFileLocation )
{
	Sockets::SocketSystem ss;
	Sockets::SocketListener sl(port, Sockets::Socket::IP6);
	// Pass in Directories into construction of Map Reducer



	//stubHandler sh;
	if(proc == "map")
	{
		
		sl.start(sh, std::ref(configFileLocation));

		while (!si.connect("localhost", 9071))
		{
			Show::write("\n  client waiting to connect");
			::Sleep(100);
		}

	}

	else if (proc == "reduce")
	{ }

	else if (proc == "test")
	{
		sl.start(sh,"test");

		while (!si.connect("localhost", 9071))
		{
			Show::write("\n  client waiting to connect");
			::Sleep(100);
		}

		Show::title("Starting string test on client");
		clientTestStringHandling(std::ref(si));
	}

	else if (proc == "stop")
	{
		si.sendString("TEST_STOP");
		Show::write("\n\n  client calling send shutdown\n");
		si.shutDownSend();
		sl.stop();
		std::cout << "Post SL Stop" << std::endl;
	}

	return true;
}
