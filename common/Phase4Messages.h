#pragma once
#include <cstdint>
#include <string>
#include <iostream>

enum THREAD_TYPE { map, reduce };
enum HEALTH_STATUS {INIT, INPROGRESS, COMPLETE};
const int32_t MAX_MESSAGE_FIELD_SIZE	= 512;
const int32_t MAX_MESSAGE_SIZE			= 2048;
const int32_t CREATE_THREAD_MESSAGE		= 1;
const int32_t JOIN_MESSAGE				= 2;
const int32_t MAP_FILE_MESSAGE			= 3;
const int32_t REDUCE_FILE_MESSAGE		= 4;
const int32_t HEARTBEAT_MESSAGE			= 5;

class baseMessage
{

public:
	baseMessage();

	virtual bool createBuffer(uint32_t& Buffersize) = 0;

	virtual bool reduceBuffer(char*, uint32_t& Buffersize) = 0;

	virtual uint32_t calculateSize() = 0;
		
	char* buffer;
};

class CreateThreadMessage : public baseMessage
{
public:
	bool createBuffer(uint32_t& Buffersize);

	bool reduceBuffer(char*, uint32_t& Buffersize);

	uint32_t calculateSize();

	void printMessage();

	uint32_t messageType;

	THREAD_TYPE threadType;

	std::string inputFileName;

	std::string outputFolderName;
};

class JoinThreadsMessage : public baseMessage
{
public:
	bool createBuffer(uint32_t& Buffersize);

	bool reduceBuffer(char*, uint32_t& Buffersize);

	uint32_t calculateSize();

	uint32_t messageType;

	bool join;
};

class MapFileMessage : public baseMessage
{
public:
	bool createBuffer(uint32_t& Buffersize);

	bool reduceBuffer(char*, uint32_t& Buffersize);

	uint32_t calculateSize();

	uint32_t messageType;

	std::string OutputFileName;
};

class ReducedFileMessage : public baseMessage
{
public:
	bool createBuffer(uint32_t& Buffersize);

	bool reduceBuffer(char*, uint32_t& Buffersize);

	uint32_t calculateSize();

	uint32_t messageType;

	std::string outputFile;

};

class HeartbeatMessage : public baseMessage
{
public:
	bool createBuffer(uint32_t& Buffersize);

	bool reduceBuffer(char*, uint32_t& Buffersize);

	uint32_t calculateSize();

	uint32_t messageType;
	
	HEALTH_STATUS healthStatus;

	std::string threadName;
};

class MessageHeader
{
public:
	bool ParseMsgHeader(uint32_t& messageType, uint32_t& Buffersize, char* charArray);
};