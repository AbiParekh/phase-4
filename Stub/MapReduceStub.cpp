#include "MapReduceStub.h"
#include "../common/Phase4Messages.h"

using Show = StaticLogger<1>;
int main()
{
    MapReduceStub mrStub(9070, 1);
    mrStub.startServer();
    return 0;
}
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
}

void stubHandler::operator()(Sockets::Socket& socket_)
{
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