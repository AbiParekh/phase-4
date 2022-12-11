#include "../common/Phase4Messages.h"
#include "../SockerCode/Sockets.h"
#include <iostream>
#include <map>

void ReportResults(std::map<std::string, bool> outputMap);
bool Test_CreateThreadMsg();
bool Test_MappedFileMessage();
bool Test_ReducedFileMessage();
bool Test_JoinThreadMessage();
bool Test_HeartbeatMessage();
bool Test_SocketCreation();

using Show = StaticLogger<1>;
using namespace Sockets;

int main()
{
	std::map<std::string, bool>  testResults;

	// CreateThreadMsg Test
	std::pair<std::string, bool> CreateThreadMsg("Test_CreateThreadMsg", Test_CreateThreadMsg());
	testResults.insert(CreateThreadMsg);

	// getListOfTextFiles Test  
	std::pair<std::string, bool> JoinThreadMessage("Test_JoinThreadMessage", Test_JoinThreadMessage());
	testResults.insert(JoinThreadMessage);

	// Mapped FileMessage
	std::pair<std::string, bool> MappedFileMessage("Test_MappedFileMessage", Test_MappedFileMessage());
	testResults.insert(MappedFileMessage);

	// Reduced File Message Test   
	std::pair<std::string, bool> ReducedFileMessage("Test_ReducedFileMessage", Test_ReducedFileMessage());
	testResults.insert(ReducedFileMessage);

	// Heartbeat Test Reduced File Message Test   
	std::pair<std::string, bool> HeartbeatMessage("Test_HeartbeatMessage", Test_HeartbeatMessage());
	testResults.insert(HeartbeatMessage);


	/*
	// createDirectory Test
	std::pair<std::string, bool> SocketCreation("Test_SocketCreation", Test_SocketCreation());
	testResults.insert(createDirectory);
	*/

	ReportResults(testResults);

	return 0;
}

void ReportResults(std::map<std::string, bool> outputMap)
{
	std::cout << "\n\n\n" << std::endl;
	std::cout << "========================================== Test Results ==========================================" << std::endl;
	bool results = true;
	for (std::map<std::string, bool>::iterator outputMapIt = outputMap.begin(); outputMapIt != outputMap.end(); outputMapIt++)
	{
		std::string resultsformatted = "";
		if (outputMapIt->second == 0) resultsformatted = "FAIL";
		else  resultsformatted = "PASS";
		std::cout << "Test: " << outputMapIt->first << "; Result: " << resultsformatted << std::endl;
	}
	std::cout << "==================================================================================================" << std::endl;
}

bool Test_CreateThreadMsg()
{
	bool results = true;
	CreateThreadMessage ThreadMessage1;
	CreateThreadMessage ThreadMessage2;
	CreateThreadMessage ThreadMessage3;
	CreateThreadMessage ThreadMessage4;

	ThreadMessage1.inputFileName = "Input File Name";
	ThreadMessage1.outputFolderName = "Ouput\\Folder\\Location";
	ThreadMessage1.threadType = THREAD_TYPE::map;
	ThreadMessage1.messageType = 1;

	uint32_t bufferSize = 0;
	char* temp1 = NULL;
	if (ThreadMessage1.createBuffer(bufferSize) != true) 
	{
		results = false;
		std::cout << "TEST FAIL CreateBufferThreadMessage 1" << std::endl;
	}
	else
	{
		temp1 = ThreadMessage1.buffer;
		char placeholder[MAX_MESSAGE_SIZE];
		std::memcpy(placeholder, temp1, bufferSize);
		uint32_t bufferSizefromReduce = 0;
		if (ThreadMessage3.reduceBuffer(placeholder, bufferSizefromReduce) != true)
		{
			results = false;
			std::cout << "TEST FAIL: ThreadMessage 3 - reduceBuffer " << std::endl;
		}

		if (ThreadMessage3.outputFolderName.compare("Ouput\\Folder\\Location") != 0)
		{
			results = false;
			std::cout << "TEST FAIL: ThreadMessage 3 - outputFolderName" << std::endl;
		}

		if (ThreadMessage3.inputFileName.compare("Input File Name") != 0)
		{
			results = false;
			std::cout << "TEST FAIL: ThreadMessage 3 - inputFileName" << std::endl;
		}

		if (ThreadMessage3.threadType != THREAD_TYPE::map)
		{
			results = false;
			std::cout << "TEST FAIL: ThreadMessage 3 - threadType" << std::endl;
		}

		if (ThreadMessage3.messageType != 1)
		{
			results = false;
			std::cout << "TEST FAIL: ThreadMessage 3 - messageType" << std::endl;
		}

		if (bufferSizefromReduce != bufferSize)
		{
			results = false;
			std::cout << "TEST FAIL: ThreadMessage 3 - Buffer Size Fail (" << bufferSizefromReduce << ", " << bufferSize  << ")" << std::endl;
		}
	}
	
	ThreadMessage2.inputFileName = "RANDOM FILE NAME FOR THREAD #2";
	ThreadMessage2.outputFolderName = "C:\\Users\\steph\\Documents\\OO Design";
	ThreadMessage2.threadType = THREAD_TYPE::reduce;
	ThreadMessage2.messageType = 5;

	char* temp2;
	uint32_t  bufferSize2 = 0;

	if (ThreadMessage2.createBuffer(bufferSize2) != true)
	{
		results = false;
		std::cout << "TEST FAIL: ThreadMessage 2 - Create Buffer" << std::endl;
	}
	else
	{
		temp2 = ThreadMessage2.buffer;
		char placeholder2[MAX_MESSAGE_SIZE];
		std::memcpy(placeholder2, temp2, bufferSize2);
		uint32_t bufferSizefromReduce = 0;
		if (ThreadMessage4.reduceBuffer(placeholder2, bufferSizefromReduce) != true)
		{
			results = false;
			std::cout << "TEST FAIL: ThreadMessage 4 - reduceBuffer" << std::endl;
		}

		if (ThreadMessage4.outputFolderName.compare("C:\\Users\\steph\\Documents\\OO Design") != 0)
		{
			results = false;
			std::cout << "TEST FAIL: ThreadMessage 4 - outputFolderName (" << ThreadMessage4.outputFolderName << ")" << std::endl;
		}

		if (ThreadMessage4.inputFileName.compare("RANDOM FILE NAME FOR THREAD #2") != 0)
		{
			results = false;
			std::cout << "TEST FAIL: ThreadMessage 4 - inputFileName (" << ThreadMessage4.inputFileName << ")" << std::endl;
		}

		if (ThreadMessage4.threadType != THREAD_TYPE::reduce)
		{
			results = false;
			std::cout << "TEST FAIL: ThreadMessage 4 - threadType" << std::endl;
		}

		if (ThreadMessage4.messageType != 5)
		{
			results = false;
			std::cout << "TEST FAIL: ThreadMessage 4 - Message Type" << std::endl;
		}

		if (bufferSizefromReduce != bufferSize2)
		{
			results = false;
			std::cout << "TEST FAIL: ThreadMessage 4 - Buffer Size Fail (" << bufferSizefromReduce << ", " << bufferSize  << ")" <<  std::endl;
		}
	}

	return results;
}

bool Test_MappedFileMessage()
{
	bool results = true;
	MapFileMessage Map1, Map3;
	MapFileMessage Map2, Map4;
	Map1.messageType = 23;
	Map1.OutputFileName = "Stub_Thread_MapFile_1.txt";

	uint32_t bufferSize = 0;
	char* temp1 = NULL;
	if (Map1.createBuffer(bufferSize) != true)
	{
		results = false;
		std::cout << "TEST FAIL: Mapped File 1 - Create Buffer" << std::endl;
	}
	else
	{
		temp1 = Map1.buffer;
		char placeholder[MAX_MESSAGE_SIZE];
		std::memcpy(placeholder, temp1, bufferSize);
		uint32_t bufferSizefromReduce = 0;
		if (Map3.reduceBuffer(placeholder, bufferSizefromReduce) != true)
		{
			results = false;
			std::cout << "TEST FAIL: Mapped Message 3 - reduceBuffer " << std::endl;
		}

		if (Map3.OutputFileName.compare("Stub_Thread_MapFile_1.txt") != 0)
		{
			results = false;
			std::cout << "TEST FAIL: Mapped Message 3 - outputFolderName" << std::endl;
		}

		if (Map3.messageType != 23)
		{
			results = false;
			std::cout << "TEST FAIL: Mapped Message 3 - messageType" << std::endl;
		}

		if (bufferSizefromReduce != bufferSize)
		{
			results = false;
			std::cout << "TEST FAIL: Mapped Message 3 - Buffer Size Fail (" << bufferSizefromReduce << ", " << bufferSize << ")" << std::endl;
		}
	}

	Map2.messageType = 24;
	Map2.OutputFileName = "Stub_10_Thread_45_MapFile_2.txt";
	
	char* temp2;
	uint32_t  bufferSize2 = 0;

	if (Map2.createBuffer(bufferSize2) != true)
	{
		results = false;
		std::cout << "TEST FAIL: ThreadMessage 2 - Create Buffer" << std::endl;
	}
	else
	{
		temp2 = Map2.buffer;
		char placeholder2[MAX_MESSAGE_SIZE];
		std::memcpy(placeholder2, temp2, bufferSize2);
		uint32_t bufferSizefromReduce = 0;
		if (Map4.reduceBuffer(placeholder2, bufferSizefromReduce) != true)
		{
			results = false;
			std::cout << "TEST FAIL: ThreadMessage 4 - reduceBuffer" << std::endl;
		}

		if (Map4.OutputFileName.compare("Stub_10_Thread_45_MapFile_2.txt") != 0)
		{
			results = false;
			std::cout << "TEST FAIL: ThreadMessage 4 - outputFolderName (" << Map4.OutputFileName << ")" << std::endl;
		}

		if (Map4.messageType != 24)
		{
			results = false;
			std::cout << "TEST FAIL: ThreadMessage 4 - Message Type" << std::endl;
		}

		if (bufferSizefromReduce != bufferSize2)
		{
			results = false;
			std::cout << "TEST FAIL: ThreadMessage 4 - Buffer Size Fail (" << bufferSizefromReduce << ", " << bufferSize << ")" << std::endl;
		}
	}

	return results;

}

bool Test_ReducedFileMessage()
{
	bool results = true;
	ReducedFileMessage Reduce1, Reduce3;
	ReducedFileMessage Reduce2, Reduce4;
	Reduce1.messageType = 100;
	Reduce1.outputFile = "GNOME_BUDDY_BUT_YOU_MapFile_1.txt";

	uint32_t bufferSize = 0;
	char* temp1 = NULL;
	if (Reduce1.createBuffer(bufferSize) != true)
	{
		results = false;
		std::cout << "TEST FAIL: Reduced File 1 - Create Buffer" << std::endl;
	}
	else
	{
		temp1 = Reduce1.buffer;
		char placeholder[MAX_MESSAGE_SIZE];
		std::memcpy(placeholder, temp1, bufferSize);
		uint32_t bufferSizefromReduce = 0;
		if (Reduce3.reduceBuffer(placeholder, bufferSizefromReduce) != true)
		{
			results = false;
			std::cout << "TEST FAIL: Reduced Message 3 - reduceBuffer " << std::endl;
		}

		if (Reduce3.outputFile.compare("GNOME_BUDDY_BUT_YOU_MapFile_1.txt") != 0)
		{
			results = false;
			std::cout << "TEST FAIL: Reduced Message 3 - outputFolderName" << std::endl;
		}

		if (Reduce3.messageType != 100)
		{
			results = false;
			std::cout << "TEST FAIL: Reduced Message 3 - messageType" << std::endl;
		}

		if (bufferSizefromReduce != bufferSize)
		{
			results = false;
			std::cout << "TEST FAIL: Reduced Message 3 - Buffer Size Fail (" << bufferSizefromReduce << ", " << bufferSize << ")" << std::endl;
		}
	}

	Reduce2.messageType = 120;
	Reduce2.outputFile = "Stub_10_Thread_45_ReducedFile_2.txt";

	char* temp2;
	uint32_t  bufferSize2 = 0;

	if (Reduce2.createBuffer(bufferSize2) != true)
	{
		results = false;
		std::cout << "TEST FAIL: Reduced 2 - Create Buffer" << std::endl;
	}
	else
	{
		temp2 = Reduce2.buffer;
		char placeholder2[MAX_MESSAGE_SIZE];
		std::memcpy(placeholder2, temp2, bufferSize2);
		uint32_t bufferSizefromReduce = 0;
		if (Reduce4.reduceBuffer(placeholder2, bufferSizefromReduce) != true)
		{
			results = false;
			std::cout << "TEST FAIL: Reduced 4 - reduceBuffer" << std::endl;
		}

		if (Reduce4.outputFile.compare("Stub_10_Thread_45_ReducedFile_2.txt") != 0)
		{
			results = false;
			std::cout << "TEST FAIL: Reduced 4 - outputFolderName (" << Reduce4.outputFile << ")" << std::endl;
		}

		if (Reduce4.messageType != 120)
		{
			results = false;
			std::cout << "TEST FAIL: Reduced 4 - Message Type (" << Reduce4.messageType << ", " << 120 << ")" << std::endl;
		}

		if (bufferSizefromReduce != bufferSize2)
		{
			results = false;
			std::cout << "TEST FAIL: Reduced 4 - Buffer Size Fail (" << bufferSizefromReduce << ", " << bufferSize << ")" << std::endl;
		}
	}

	return results;
}

bool Test_JoinThreadMessage()
{
	bool results = true;
	JoinThreadsMessage joinMessage1, joinMessage3;
	JoinThreadsMessage joinMessage2, joinMessage4;

	joinMessage1.messageType = 7;
	joinMessage1.join = true;
	joinMessage2.messageType = 9;
	joinMessage2.join = false;

	uint32_t bufferSize = 0;
	char* temp1 = NULL;
	if (joinMessage1.createBuffer(bufferSize) != true)
	{
		results = false;
		std::cout << "TEST FAIL: Join Message 1 CreateBuffer" << std::endl;
	}
	else
	{
		temp1 = joinMessage1.buffer;
		char placeholder[MAX_MESSAGE_SIZE];
		std::memcpy(placeholder, temp1, bufferSize);
		uint32_t bufferSizefromReduce = 0;
		if (joinMessage3.reduceBuffer(placeholder, bufferSizefromReduce) != true)
		{
			results = false;
			std::cout << "TEST FAIL: Join Message 3 Reduce Buffer" << std::endl;
		}

		if (joinMessage3.messageType != 7)
		{
			results = false;
			std::cout << "TEST FAIL: Join Message 3 messageType" << std::endl;
		}

		if (joinMessage3.join != true)
		{
			results = false;
			std::cout << "TEST FAIL: Join Message 3 Join" << std::endl;
		}

		if (bufferSizefromReduce != bufferSize)
		{
			results = false;
			std::cout << "TEST FAIL: Join Message 3 Buffer Size" << std::endl;
		}
	}

	char* temp2 = NULL;
	if (joinMessage2.createBuffer(bufferSize) != true)
	{
		results = false;
		std::cout << "TEST FAIL: Join Message 2 CreateBuffer" << std::endl;
	}
	else
	{
		temp2 = joinMessage2.buffer;
		char placeholder[MAX_MESSAGE_SIZE];
		std::memcpy(placeholder, temp2, bufferSize);
		uint32_t bufferSizefromReduce = 0;
		if (joinMessage4.reduceBuffer(placeholder, bufferSizefromReduce) != true)
		{
			results = false;
			std::cout << "TEST FAIL: Join Message 4 Reduce Buffer" << std::endl;
		}

		if (joinMessage4.messageType != 9)
		{
			results = false;
			std::cout << "TEST FAIL: Join Message 4 messageType" << std::endl;
		}

		if (joinMessage4.join != false)
		{
			results = false;
			std::cout << "TEST FAIL: Join Message 4 Join" << std::endl;
		}

		if (bufferSizefromReduce != bufferSize)
		{
			results = false;
			std::cout << "TEST FAIL: Join Message 4 Buffer Size" << std::endl;
		}
	}
		
	return results;
}

bool Test_HeartbeatMessage()
{
	bool results = true;
	HeartbeatMessage HeartbeatMessage1;
	HeartbeatMessage HeartbeatMessage2;
	HeartbeatMessage HeartbeatMessage3;
	HeartbeatMessage HeartbeatMessage4;

	HeartbeatMessage1.threadName = "THREAD_STUBX_THREADY";
	HeartbeatMessage1.healthStatus = HEALTH_STATUS::COMPLETE;
	HeartbeatMessage1.messageType = 5;

	uint32_t bufferSize = 0;
	char* temp1 = NULL;
	if (HeartbeatMessage1.createBuffer(bufferSize) != true)
	{
		results = false;
		std::cout << "TEST FAIL: HeartbeatMessage Create Buffer" << std::endl;
	}
	else
	{

		temp1 = HeartbeatMessage1.buffer;

		char placeholder[MAX_MESSAGE_SIZE];
		std::memcpy(placeholder, temp1, bufferSize);
		uint32_t bufferSizefromReduce = 0;
		if (HeartbeatMessage3.reduceBuffer(placeholder, bufferSizefromReduce) != true)
		{
			results = false;
			std::cout << "TEST FAIL: HeartbeatMessage 3 - reduceBuffer " << std::endl;
		}

		if (HeartbeatMessage3.threadName.compare("THREAD_STUBX_THREADY") != 0)
		{
			results = false;
			std::cout << "TEST FAIL: HeartbeatMessage 3 - outputFolderName" << std::endl;
		}

		if (HeartbeatMessage3.healthStatus != HEALTH_STATUS::COMPLETE)
		{
			results = false;
			std::cout << "TEST FAIL: HeartbeatMessage 3 - threadType" << std::endl;
		}

		if (HeartbeatMessage3.messageType != 5)
		{
			results = false;
			std::cout << "TEST FAIL: HeartbeatMessage 3 - messageType" << std::endl;
		}

		if (bufferSizefromReduce != bufferSize)
		{
			results = false;
			std::cout << "TEST FAIL: HeartbeatMessage 3 - Buffer Size Fail (" << bufferSizefromReduce << ", " << bufferSize << ")" << std::endl;
		}
	}

	HeartbeatMessage2.threadName = "RANDOM THREAD NAME YAHHHHH.txt";
	HeartbeatMessage2.healthStatus = HEALTH_STATUS::INPROGRESS;
	HeartbeatMessage2.messageType = 5;

	char* temp2;
	uint32_t  bufferSize2 = 0;

	if (HeartbeatMessage2.createBuffer(bufferSize2) != true)
	{
		results = false;
		std::cout << "TEST FAIL: HeartbeatMessage 2 - Create Buffer" << std::endl;
	}
	else
	{
		temp2 = HeartbeatMessage2.buffer;
		char placeholder2[MAX_MESSAGE_SIZE];
		std::memcpy(placeholder2, temp2, bufferSize2);
		uint32_t bufferSizefromReduce = 0;
		if (HeartbeatMessage4.reduceBuffer(placeholder2, bufferSizefromReduce) != true)
		{
			results = false;
			std::cout << "TEST FAIL: HeartbeatMessage 4 - reduceBuffer" << std::endl;
		}

		if (HeartbeatMessage4.threadName.compare("RANDOM THREAD NAME YAHHHHH.txt") != 0)
		{
			results = false;
			std::cout << "TEST FAIL: HeartbeatMessage 4 - inputFileName (" << HeartbeatMessage4.threadName << ")" << std::endl;
		}

		if (HeartbeatMessage4.healthStatus != THREAD_TYPE::reduce)
		{
			results = false;
			std::cout << "TEST FAIL: HeartbeatMessage 4 - threadType" << std::endl;
		}

		if (HeartbeatMessage4.messageType != 5)
		{
			results = false;
			std::cout << "TEST FAIL: HeartbeatMessage 4 - Message Type" << std::endl;
		}

		if (bufferSizefromReduce != bufferSize2)
		{
			results = false;
			std::cout << "TEST FAIL: HeartbeatMessage 4 - Buffer Size Fail (" << bufferSizefromReduce << ", " << bufferSize << ")" << std::endl;
		}
	}

	return results;
}

bool Test_SocketCreation()
{
	Show::attach(&std::cout);
	Show::start();
	Show::title("Testing Sockets", '=');

	try
	{
		SocketSystem ss;
		SocketConnecter si;
		SocketListener sl(9070, Socket::IP4);
		ClientHandler cp;
		sl.start(cp);

		while (!si.connect("localhost", 9070))
		{
			Show::write("\n  client waiting to connect");
			::Sleep(100);
		}

		Show::title("Starting string test on client");
		clientTestStringHandling(si);

		////////////////////////////////////////////////////
		// This buffer handling test doesn't work yet.
		// I'll fix when time permits.
		//
		// Show::title("Starting buffer test on client");
		// clientTestBufferHandling(si);

		si.sendString("TEST_STOP");

		Show::write("\n\n  client calling send shutdown\n");
		si.shutDownSend();
		sl.stop();
	}
	catch (std::exception& ex)
	{
		std::cout << "\n  Exception caught:";
		std::cout << "\n  " << ex.what() << "\n\n";
	}
}
