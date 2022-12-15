// StubTest.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <Windows.h>
#include "MapReduceStub.h"
#include <map>
#include <string>
#include <vector>
#include "../SockerCode/Sockets.h"
#include "../common/Phase4Messages.h"

using namespace std;


void exportResults(std::map<std::string, bool> OutputRESULTS);

bool testCase_startServer();

bool testCase_processMessage();

bool testCase_Operator();



int main()
{
	std::map<std::string, bool> unitTestResults;

	std::pair<std::string, bool> startServer("testCase_startServer", testCase_startServer());
	unitTestResults.insert(startServer);

	std::pair<std::string, bool> processMessage("testCase_processMessage", testCase_processMessage());
	unitTestResults.insert(processMessage);

	std::pair<std::string, bool> operator("testCase_Operator", testCase_Operator());
	unitTestResults.insert(operator);

	exportResults(unitTestResults);

	return 0;
}


void exportResults(std::map<std::string, bool> OutputRESULTS) {

	std::cout << "Unit Test Results" << std::endl;

	bool results = true;
	for (std::map<std::string, bool>::iterator i = OutputRESULTS.begin(); i != OutputRESULTS.end(); i++)
	{
		std::string wrap = "";
		if (i->second == 0) wrap = "TEST FAILED";
		else  wrap = "TEST PASSED";
		std::cout << "Test: " << i->first << "; Result: " << wrap << std::endl;
	}

}


bool testCase_startServer() {

	bool results = true;
	
	std::string ipAdress;
	uint32_t port;
	uint32_t ID;
	Sockets::SocketListener sl(port, Sockets::Socket::IP4);

	if (port = NULL || ipAdress != Sockets::Socket::IP4) results = false;
	
	return results;

}


bool testCase_Operator() {

	bool testResults = true;

	char* SockerBuffer = 0;
	char* bufferCurrentPointer = Sockerbuffer;
	int64_t remainingSize = actualsize;
	char headerBuffer[24];
	ReducedFileMessage reduce1;
	reduce1.messageType = 100;
	MapFileMessage map1;
	map1.messageType = 23;


	if (remainingSize < 0 || remainingSize = NULL) 
	{
		testResults = false
		cout << "ERROR: Not sufficient memory to hold operation instructions and messages" << endl;
	}

	if (char* SockerBuffer = NULL) 
	{
		testResults = false;
		cout << "ERROR: Buffer to hold sockets and its information does not exist" << endl;
	}

	if (char* bufferCurrentPointer != Sockerbuffer) 
	{
		testResults = false;
		cout << "ERROR: stack pointer for memory buffer is not correctly fetching the proper information needed" << endl;
	}

	if (remainingSize != actualsize) 
	{
		testResults = false;
		cout << "ERROR :incorrect memory size detected" << endl;
	}
	
	if (map1.messageType != 23) 
	{
		testResults = false;
		cout << "ERROR: Incorrect message type" << endl;
	}

	if (reduce1.messageType != 100)
	{
		testResults = false;
		cout << "ERROR: Incorrect message type" << endl;
	}
	
	//other test cases can include if the threads were called or not, if too many threads were created
	//if threads are not receiving the files

	return testResults;
}

bool testCase_processMessage() {
	bool testResults = true;

}




