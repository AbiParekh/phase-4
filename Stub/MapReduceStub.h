#pragma once
#include <string>
#include "../common/Phase4Messages.h"
#include "../SockerCode/Sockets.h"
#include "../SockerCode/Logger.h"
#include "../SockerCode/StringUtilities.h"
#include "MapReduceWF.h"


using Show = StaticLogger<1>;

static class ClientHandler
{

public:
	void operator()(Sockets::Socket& socket_);
	bool testStringHandling(Sockets::Socket& socket_);
	bool testBufferHandling(Sockets::Socket& socket_);
	void setParam(std::string params);
	// MapReducer mapReducer(param);

private:
	std::string param;

};

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


	bool startProcessor(Sockets::SocketListener& SL, Sockets::SocketConnecter& SI, std::string procID,std::string args, uint32_t portnumber);

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


bool MapReduceStub::startProcessor(Sockets::SocketListener& sl, Sockets::SocketConnecter& si, const std::string proc, std::string configFileLocation, uint32_t port )
{

	// Pass in Directories into construction of Map Reducer
	ClientHandler co;

	co.setParam((configFileLocation));
	//stubHandler sh;
	if(proc == "map")
	{

		
		sl.start(co, std::ref(configFileLocation));

		while (!si.connect("localhost", port))
		{
			Show::write("\n  client waiting to connect");
			::Sleep(100);
		}
		Show::title("Starting string test on client");
		//clientTestStringHandling(si);
		clientRunMap(si, configFileLocation);


	}

	else if (proc == "reduce")
	{ }

	else if (proc == "test")
	{
		sl.start(co,proc);

		while (!si.connect("localhost", port))
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

	}

	

	return true;
}
