#pragma once
#include <cstdint>
#include <string>

enum THREAD_TYPE { map, reduce };
const int32_t MAX_MESSAGE_FIELD_SIZE = 512;
const int32_t MAX_MESSAGE_SIZE = 2048;

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
