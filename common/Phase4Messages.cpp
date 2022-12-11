#include "Phase4Messages.h"
#include <iostream>


baseMessage::baseMessage()
{
	buffer = new char[MAX_MESSAGE_SIZE];
}


bool CreateThreadMessage::createBuffer(uint32_t& Buffersize)
{
	try
	{
		uint32_t BufferCounter = 0; 
		// Message Type
		buffer[0] = messageType & 0xff;
		buffer[1] = (messageType >> 8) & 0xff;
		buffer[2] = (messageType >> 16) & 0xff;
		buffer[3] = (messageType >> 24) & 0xff;

		Buffersize = calculateSize();

		// Size of Message
		buffer[4] = Buffersize & 0xff;
		buffer[5] = (Buffersize >> 8) & 0xff;
		buffer[6] = (Buffersize >> 16) & 0xff;
		buffer[7] = (Buffersize >> 24) & 0xff;

		// Thread Type
		buffer[8] = threadType & 0xff;
		buffer[9] = (threadType >> 8) & 0xff;
		buffer[10] = (threadType >> 16) & 0xff;
		buffer[11] = (threadType >> 24) & 0xff;

		size_t inputFileStringSize = inputFileName.length();
		if (inputFileStringSize > 512)
		{
			std::cout << "ERROR: Input File String Size larger then 512 Characters" << std::endl;
			return false;
		}

		BufferCounter = 12;
		for (uint32_t i = 0; i < inputFileStringSize; i++)
		{
			buffer[BufferCounter] = inputFileName.at(i);
			BufferCounter++;
		}

		buffer[BufferCounter] = '\0';
		BufferCounter++;

		size_t outputFolderStringSize = outputFolderName.length();
		if (outputFolderStringSize > 512)
		{
			std::cout << "ERROR: Output Folder String Size larger then 512 Characters" << std::endl;
			return false;
		}
		
		for (uint32_t i = 0; i < outputFolderStringSize; i++)
		{
			buffer[BufferCounter] = outputFolderName.at(i);
			BufferCounter++;
		}

		buffer[BufferCounter] = '\0';
		BufferCounter++;

		if (BufferCounter != Buffersize)
		{
			std::cout << "ERROR: Calculated Size is not equal to Actual Buffer Size" << std::endl;
			return false;
		}

	}
	catch (...) 
	{
		std::cout << "CATCH EXPECTION createBuffer" << std::endl;
		return false;
	}

	return true;
}

bool CreateThreadMessage::reduceBuffer(char* Inputbuffer, uint32_t& Buffersize)
{
	try
	{
		messageType = Inputbuffer[3];
		messageType <<= 8;
		messageType |= Inputbuffer[2];
		messageType <<= 8;
		messageType |= Inputbuffer[1];
		messageType <<= 8;
		messageType |= Inputbuffer[0];

		uint32_t size = 0;
		size = Inputbuffer[7];
		size <<= 8;
		size |= Inputbuffer[6];
		size <<= 8;
		size |= Inputbuffer[5];
		size <<= 8;
		size |= Inputbuffer[4];
		
		Buffersize = size;

		uint32_t threadTypeInput;
		threadTypeInput = Inputbuffer[11];
		threadTypeInput <<= 8;
		threadTypeInput |= Inputbuffer[10];
		threadTypeInput <<= 8;
		threadTypeInput |= Inputbuffer[9];
		threadTypeInput <<= 8;
		threadTypeInput |= Inputbuffer[8];
		threadType = (THREAD_TYPE)threadTypeInput;

		uint32_t BufferCounter = 12;
		uint32_t fieldSize = 0;
		while (Inputbuffer[BufferCounter] != NULL)
		{
			inputFileName.push_back(Inputbuffer[BufferCounter]);
			BufferCounter++;
			fieldSize++;
			if (fieldSize > MAX_MESSAGE_FIELD_SIZE)
			{
				std::cout << "ERROR: Unable to find the end of the Input File Name" << std::endl;
				return false;
				break;
			}
		}

		BufferCounter++;
		fieldSize = 0;
		while (Inputbuffer[BufferCounter] != NULL)
		{
			outputFolderName.push_back(Inputbuffer[BufferCounter]);
			BufferCounter++;
			fieldSize++;
			if (fieldSize > MAX_MESSAGE_FIELD_SIZE)
			{
				std::cout << "ERROR: Unable to find the end of the Input File Name" << std::endl;
				return false;
				break;
			}
		}

		BufferCounter++;
		Buffersize = size;
	}
	catch (...)
	{
		std::cout << "CATCH EXPECTION CreateThreadMessage::reduceBuffer" << std::endl;
		return false;
	}

	return true;
}

uint32_t CreateThreadMessage::calculateSize()
{
	uint32_t sizeOfMessage = sizeof(messageType) + sizeof(int) + sizeof(threadType) + inputFileName.size()+ 1 + outputFolderName.size() +1;
	return sizeOfMessage;
}

void CreateThreadMessage::printMessage()
{
	std::cout << "\n\n\n Print Message:" << std::endl;
	std::cout << "      Message Type: " << messageType << std::endl;
	std::cout << "      Thread Type: " << threadType << std::endl;
	std::cout << "      Input File Name: " << inputFileName << std::endl;
	std::cout << "      Output Folder Name: " << outputFolderName << std::endl;
	return;
}

bool JoinThreadsMessage::createBuffer(uint32_t& Buffersize)
{
	try
	{
		uint32_t BufferCounter = 0;
		// Message Type
		buffer[0] = messageType & 0xff;
		buffer[1] = (messageType >> 8) & 0xff;
		buffer[2] = (messageType >> 16) & 0xff;
		buffer[3] = (messageType >> 24) & 0xff;

		Buffersize = calculateSize();

		// Size of Message
		buffer[4] = Buffersize & 0xff;
		buffer[5] = (Buffersize >> 8) & 0xff;
		buffer[6] = (Buffersize >> 16) & 0xff;
		buffer[7] = (Buffersize >> 24) & 0xff;

		// Thread Type
		buffer[8] = join;
		buffer[9] = 0x0;
		buffer[10] = 0x0;
		buffer[11] = 0x0;

	}
	catch (...)
	{
		std::cout << "CATCH EXPECTION createBuffer" << std::endl;
		return false;
	}
	return true;
}

bool JoinThreadsMessage::reduceBuffer(char* Inputbuffer, uint32_t& Buffersize)
{
	try
	{
		messageType = Inputbuffer[3];
		messageType <<= 8;
		messageType |= Inputbuffer[2];
		messageType <<= 8;
		messageType |= Inputbuffer[1];
		messageType <<= 8;
		messageType |= Inputbuffer[0];

		uint32_t size = 0;
		size = Inputbuffer[7];
		size <<= 8;
		size |= Inputbuffer[6];
		size <<= 8;
		size |= Inputbuffer[5];
		size <<= 8;
		size |= Inputbuffer[4];

		join = Inputbuffer[8];
		
		Buffersize = size;
	}
	catch (...)
	{
		std::cout << "CATCH EXPECTION CreateThreadMessage::reduceBuffer" << std::endl;
		return false;
	}

	return true;
}

uint32_t JoinThreadsMessage::calculateSize()
{
	return 12;
}

bool MapFileMessage::createBuffer(uint32_t& Buffersize)
{
	try
	{
		uint32_t BufferCounter = 0;
		// Message Type
		buffer[0] = messageType & 0xff;
		buffer[1] = (messageType >> 8) & 0xff;
		buffer[2] = (messageType >> 16) & 0xff;
		buffer[3] = (messageType >> 24) & 0xff;

		Buffersize = calculateSize();

		// Size of Message
		buffer[4] = Buffersize & 0xff;
		buffer[5] = (Buffersize >> 8) & 0xff;
		buffer[6] = (Buffersize >> 16) & 0xff;
		buffer[7] = (Buffersize >> 24) & 0xff;

		size_t OutputFileNameStringSize = OutputFileName.length();
		if (OutputFileNameStringSize > 512)
		{
			std::cout << "ERROR: Input File String Size larger then 512 Characters" << std::endl;
			return false;
		}

		BufferCounter = 8;
		for (uint32_t i = 0; i < OutputFileNameStringSize; i++)
		{
			buffer[BufferCounter] = OutputFileName.at(i);
			BufferCounter++;
		}

		buffer[BufferCounter] = '\0';
		BufferCounter++;


		if (BufferCounter != Buffersize)
		{
			std::cout << "ERROR: Calculated Size is not equal to Actual Buffer Size" << std::endl;
			return false;
		}


	}
	catch (...)
	{
		std::cout << "CATCH EXPECTION createBuffer" << std::endl;
		return false;
	}
	return true;
}

bool MapFileMessage::reduceBuffer(char* Inputbuffer, uint32_t& Buffersize)
{
	try
	{
		messageType = Inputbuffer[3];
		messageType <<= 8;
		messageType |= Inputbuffer[2];
		messageType <<= 8;
		messageType |= Inputbuffer[1];
		messageType <<= 8;
		messageType |= Inputbuffer[0];

		uint32_t size = 0;
		size = Inputbuffer[7];
		size <<= 8;
		size |= Inputbuffer[6];
		size <<= 8;
		size |= Inputbuffer[5];
		size <<= 8;
		size |= Inputbuffer[4];

		Buffersize = size;

		uint32_t BufferCounter = 8;
		uint32_t fieldSize = 0;
		while (Inputbuffer[BufferCounter] != NULL)
		{
			OutputFileName.push_back(Inputbuffer[BufferCounter]);
			BufferCounter++;
			fieldSize++;
			if (fieldSize > MAX_MESSAGE_FIELD_SIZE)
			{
				std::cout << "ERROR: Unable to find the end of the Input File Name" << std::endl;
				return false;
				break;
			}
		}

		BufferCounter++;
		Buffersize = size;
	}
	catch (...)
	{
		std::cout << "CATCH EXPECTION CreateThreadMessage::reduceBuffer" << std::endl;
		return false;
	}
	return true;
}

uint32_t MapFileMessage::calculateSize()
{
	uint32_t sizeOfMessage = sizeof(messageType) + sizeof(int) + OutputFileName.size() + 1;
	return sizeOfMessage;
}

bool ReducedFileMessage::createBuffer(uint32_t& Buffersize)
{
	try
	{
		uint32_t BufferCounter = 0;
		// Message Type
		buffer[0] = messageType & 0xff;
		buffer[1] = (messageType >> 8) & 0xff;
		buffer[2] = (messageType >> 16) & 0xff;
		buffer[3] = (messageType >> 24) & 0xff;

		Buffersize = calculateSize();

		// Size of Message
		buffer[4] = Buffersize & 0xff;
		buffer[5] = (Buffersize >> 8) & 0xff;
		buffer[6] = (Buffersize >> 16) & 0xff;
		buffer[7] = (Buffersize >> 24) & 0xff;

		size_t OutputFileNameStringSize = outputFile.length();
		if (OutputFileNameStringSize > 512)
		{
			std::cout << "ERROR: Input File String Size larger then 512 Characters" << std::endl;
			return false;
		}

		BufferCounter = 8;
		for (uint32_t i = 0; i < OutputFileNameStringSize; i++)
		{
			buffer[BufferCounter] = outputFile.at(i);
			BufferCounter++;
		}

		buffer[BufferCounter] = '\0';
		BufferCounter++;

		if (BufferCounter != Buffersize)
		{
			std::cout << "ERROR: Calculated Size is not equal to Actual Buffer Size" << std::endl;
			return false;
		}
	}
	catch (...)
	{
		std::cout << "CATCH EXPECTION createBuffer" << std::endl;
		return false;
	}
	return true;
}

bool ReducedFileMessage::reduceBuffer(char* InputBuffer, uint32_t& Buffersize)
{
	try
	{
		messageType = InputBuffer[3];
		messageType <<= 8;
		messageType |= InputBuffer[2];
		messageType <<= 8;
		messageType |= InputBuffer[1];
		messageType <<= 8;
		messageType |= InputBuffer[0];

		uint32_t size = 0;
		size = InputBuffer[7];
		size <<= 8;
		size |= InputBuffer[6];
		size <<= 8;
		size |= InputBuffer[5];
		size <<= 8;
		size |= InputBuffer[4];

		Buffersize = size;

		uint32_t BufferCounter = 8;
		uint32_t fieldSize = 0;
		while (InputBuffer[BufferCounter] != NULL)
		{
			outputFile.push_back(InputBuffer[BufferCounter]);
			BufferCounter++;
			fieldSize++;
			if (fieldSize > MAX_MESSAGE_FIELD_SIZE)
			{
				std::cout << "ERROR: Unable to find the end of the Input File Name" << std::endl;
				return false;
				break;
			}
		}
		BufferCounter++;
		Buffersize = size;
	}
	catch (...)
	{
		std::cout << "CATCH EXPECTION CreateThreadMessage::reduceBuffer" << std::endl;
		return false;
	}
	return true;
}

uint32_t ReducedFileMessage::calculateSize()
{
	uint32_t sizeOfMessage = sizeof(messageType) + sizeof(int) + outputFile.size() + 1;
	return sizeOfMessage;
}

bool HeartbeatMessage::createBuffer(uint32_t& Buffersize)
{
	try
	{
		uint32_t BufferCounter = 0;
		// Message Type
		buffer[0] = messageType & 0xff;
		buffer[1] = (messageType >> 8) & 0xff;
		buffer[2] = (messageType >> 16) & 0xff;
		buffer[3] = (messageType >> 24) & 0xff;

		Buffersize = calculateSize();

		// Size of Message
		buffer[4] = Buffersize & 0xff;
		buffer[5] = (Buffersize >> 8) & 0xff;
		buffer[6] = (Buffersize >> 16) & 0xff;
		buffer[7] = (Buffersize >> 24) & 0xff;

		// Thread Type
		buffer[8] = healthStatus & 0xff;
		buffer[9] = (healthStatus >> 8) & 0xff;
		buffer[10] = (healthStatus >> 16) & 0xff;
		buffer[11] = (healthStatus >> 24) & 0xff;

		size_t threadNameStringSize = threadName.length();
		if (threadNameStringSize > 512)
		{
			std::cout << "ERROR: Thread Name String Size larger then 512 Characters" << std::endl;
			return false;
		}

		BufferCounter = 12;
		for (uint32_t i = 0; i < threadNameStringSize; i++)
		{
			buffer[BufferCounter] = threadName.at(i);
			BufferCounter++;
		}

		buffer[BufferCounter] = '\0';
		BufferCounter++;

		if (BufferCounter != Buffersize)
		{
			std::cout << "ERROR: Calculated Size is not equal to Actual Buffer Size" << std::endl;
			return false;
		}
	}
	catch (...)
	{
		std::cout << "CATCH EXPECTION HeartbeatMessage createBuffer" << std::endl;
		return false;
	}
	return true;
}

bool HeartbeatMessage::reduceBuffer(char* InputBuffer, uint32_t& Buffersize)
{
	try
	{
		messageType = InputBuffer[3];
		messageType <<= 8;
		messageType |= InputBuffer[2];
		messageType <<= 8;
		messageType |= InputBuffer[1];
		messageType <<= 8;
		messageType |= InputBuffer[0];

		uint32_t size = 0;
		size = InputBuffer[7];
		size <<= 8;
		size |= InputBuffer[6];
		size <<= 8;
		size |= InputBuffer[5];
		size <<= 8;
		size |= InputBuffer[4];

		Buffersize = size;

		uint32_t healthStatusInput;
		healthStatusInput = InputBuffer[11];
		healthStatusInput <<= 8;
		healthStatusInput |= InputBuffer[10];
		healthStatusInput <<= 8;
		healthStatusInput |= InputBuffer[9];
		healthStatusInput <<= 8;
		healthStatusInput |= InputBuffer[8];
		healthStatus = (HEALTH_STATUS)healthStatusInput;

		uint32_t BufferCounter = 12;
		uint32_t fieldSize = 0;
		while (InputBuffer[BufferCounter] != NULL)
		{
			threadName.push_back(InputBuffer[BufferCounter]);
			BufferCounter++;
			fieldSize++;
			if (fieldSize > MAX_MESSAGE_FIELD_SIZE)
			{
				std::cout << "ERROR: Unable to find the end of the Input File Name" << std::endl;
				return false;
				break;
			}
		}

		BufferCounter++;
		Buffersize = size;
	}
	catch (...)
	{
		std::cout << "CATCH EXPECTION CreateThreadMessage::reduceBuffer" << std::endl;
		return false;
	}

	return true;
}

uint32_t HeartbeatMessage::calculateSize()
{
	uint32_t sizeOfMessage = sizeof(messageType) + sizeof(healthStatus) + sizeof(int) + threadName.size() + 1;
	return sizeOfMessage;
	
}
