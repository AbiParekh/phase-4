#include "MapReduceStub.h"
#include "../SockerCode/Sockets.h"
//#include "../common/Phase4Messages.h"


static class ClientHandler
{
public:
    void operator()(Sockets::Socket& socket_);
    bool testStringHandling(Sockets::Socket& socket_);
    bool testBufferHandling(Sockets::Socket& socket_);
};

using Show = StaticLogger<1>;



/// Convert Char* Arguments to Strings within a Vector 

void convertArgsToStrings(int numberOfArguments, char* argumentArray[], std::vector<std::string>& argumentVector)
{
    for (int i = 0; i < numberOfArguments; ++i)
    {
        argumentVector.push_back(std::string(argumentArray[i]));
    }
}

int main(int argc, char* argv[])
{

    std::vector<std::string> argumentVector;
    convertArgsToStrings(argc, argv, argumentVector);

    std::vector<std::string>::iterator argVecIT = find(argumentVector.begin(), argumentVector.end(), "-help");
    if (argVecIT != argumentVector.end())
    {
        std::cin.get();
        return -1;
    }
    else if (argumentVector.size() != 2)
    {
        //printHelp();
        std::cin.get();
        return -1;
    }

;

    Show::attach(&std::cout);
    Show::start();
    Show::title("Testing Sockets", '=');
    Sockets::SocketConnecter si;
    stubHandler sh;
    ClientHandler ch;
    MapReduceStub mrStub(9071, 1);
    std::string configFile = argumentVector.at(1);

   // Sockets::Socket clientSocket = accept(0, NULL, NULL);
    // std::thread clientThread(ch, std::move(std::ref(clientSocket)));
  
    mrStub.startProcessor(ch, si, "test", argumentVector.at(1));
    mrStub.startProcessor(ch, si, "map", std::ref(configFile));
    mrStub.startProcessor(ch, si, "stop", argumentVector.at(1));




 //   mrStub.stopServer()
/*
* s
    while (!si.connect("localhost", 9071))
    {
        std::cout<< "\ client waiting to connect" << std::endl;
        ::Sleep(1000);
    }*/


    return 0;
}

/*
bool MapReduceStub::startServer()
{
	Sockets::SocketSystem ss;

    Sockets::SocketListener sl(port, Sockets::Socket::IP4);

    stubHandler SH;

	sl.start(SH);
    std::cout << "Post SL Start" << std::endl;

    std::this_thread::sleep_for(std::chrono::seconds(3));
    sl.stop();
    std::cout << "Post SL Stop" << std::endl;

	return true;
}*/

void stubHandler::operator()(Sockets::Socket& socket_)
{

    std::string command = Sockets::Socket::removeTerminator(socket_.recvString());
    Show::write("\n  server rcvd command: " + command);

    char* Sockerbuffer = new char[MAX_MESSAGE_SIZE * 10]; // Should never have more then 10 messages 

    while (true)
    {
        static int counter = 0;

        // Wait for Data
        if (socket_.waitForData(100, 1200) == true)
        {
            memset(Sockerbuffer, 0, sizeof(char[MAX_MESSAGE_SIZE * 10]));
            counter = 0;

            // Determine how much to Grab
            size_t bytesToGrab = socket_.bytesWaiting();
            if (bytesToGrab > MAX_MESSAGE_SIZE) Show::write("ERROR: MORE DATA THEN THE BUFFER MISSING PROCESSING TO HANDLE THIS CASE. SHIT IS GOING TO HIT THE FAN");
            int64_t actualsize = socket_.recvStream(bytesToGrab, Sockerbuffer);

            // Get Data need to loop throught the entire buffer and carve out all the messages
            char* bufferCurrentPointer = Sockerbuffer;
            int64_t remainingSize = actualsize;
            char headerBuffer[24];

            // Loop until all messages have been captured
            while (remainingSize > 0)
            {
                // reset our buffer and copy Header of the Next Message
                memset(headerBuffer, 0, sizeof(headerBuffer));
                std::memcpy(headerBuffer, bufferCurrentPointer, 8);
                uint32_t messageType = 0;
                uint32_t size = 0;
                MessageHeader tempMessageHeader;
                if (tempMessageHeader.ParseMsgHeader(messageType, size, headerBuffer) == false)
                {
                    Show::write("ERROR: UNRECOVERABLE BUFFER THROWING IT AWAY HOPEFULLY ITS NOT IMPORTANT");
                    break;
                }
                else
                {
                    if (messageType == CREATE_THREAD_MESSAGE)
                    {
                        char newMessage[MAX_MESSAGE_SIZE];
                        std::memcpy(newMessage, bufferCurrentPointer, size);
                        bufferCurrentPointer = bufferCurrentPointer + size;
                        remainingSize = remainingSize - size;
                        CreateThreadMessage threadMessage;
                        uint32_t temp; // dont need it
                        if (threadMessage.reduceBuffer(newMessage, temp))
                        {
                            Show::write("INFO: RECEIVED MESSAGE TO CREATE THREAD!");
                            std::cout << " INFO: RECEIVED MESSAGE TO CREATE THREAD!" << threadMessage.threadType << " Filename " << threadMessage.inputFileName << std::endl;
                            if (threadMessage.threadType == THREAD_TYPE::map)
                            {
                                // call create thread with Map
                            }
                            else
                            {
                                // Call Reduce Thread
                            }
                        }
                        else
                        {
                            Show::write("ERROR: Invalid Message Type for this Port");
                            break;
                        }
                    }
                }
            }
        }
        else
        {
            if (counter == 5)
            {
                Show::write("\n  waitForData Socket Failed no Data received for 10 Minutes Closing Socket");
                break;
            }
            else Show::write("\n  waitForData Socket Failed no Data received for 2 Minutes");
            counter++;
        }
    }
    socket_.shutDown();
    socket_.close();
    Show::write("\n  stubHandler thread terminating");
}
