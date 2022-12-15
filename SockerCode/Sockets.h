#pragma once
#ifndef SOCKETS_H
#define SOCKETS_H
#ifndef WIN32_LEAN_AND_MEAN  // prevents duplicate includes of core parts of windows.h in winsock2.h
#define WIN32_LEAN_AND_MEAN
#endif

#include <Windows.h>      // Windnows API
#include <winsock2.h>     // Windows sockets, ver 2
#include <WS2tcpip.h>     // support for IPv6 and other things
#include <IPHlpApi.h>     // ip helpers

#include <vector>
#include <string>
#include <atomic>
#include <MapReduceWF.h>
#include "StringUtilities.h"
#include "WindowsHelpers.h"
#include "Logger.h"

#pragma warning(disable:4267)  // conversion from int to size_t for send and recv
#pragma comment(lib, "Ws2_32.lib")

namespace Sockets
{
    /////////////////////////////////////////////////////////////////////////////
    // SocketSystem class - manages loading and unloading Winsock library

    class SocketSystem
    {
    public:
        SocketSystem();
        ~SocketSystem();
    private:
        int iResult;
        WSADATA wsaData;
    };

    /////////////////////////////////////////////////////////////////////////////
    // Socket class
    // - used by server for client handling
    // - base for SocketConnecter and SocketListener classes

    class Socket
    {
    public:
        enum IpVer { IP4, IP6 };
        using byte = char;

        // disable copy construction and assignment
        Socket(const Socket& s) = delete;
        Socket& operator=(const Socket& s) = delete;

        Socket(IpVer ipver = IP4);
        Socket(::SOCKET);
        Socket(Socket&& s);
        operator ::SOCKET() { return socket_; }
        Socket& operator=(Socket&& s);
        virtual ~Socket();

        IpVer& ipVer();
        bool send(size_t bytes, byte* buffer);
        bool recv(size_t bytes, byte* buffer);
        size_t sendStream(size_t bytes, byte* buffer);
        size_t recvStream(size_t bytes, byte* buffer);
        bool sendString(const std::string& str, byte terminator = '\0');
        std::string recvString(byte terminator = '\0');
        static std::string removeTerminator(const std::string& src);
        size_t bytesWaiting();
        bool waitForData(size_t timeToWait, size_t timeToCheck);
        bool shutDownSend();
        bool shutDownRecv();
        bool shutDown();
        void close();


        bool validState() { return socket_ != INVALID_SOCKET; }

    protected:
        WSADATA wsaData;
        ::SOCKET socket_;
        struct addrinfo* result = NULL, * ptr = NULL, hints;
        int iResult;
        IpVer ipver_ = IP4;
    };

    /////////////////////////////////////////////////////////////////////////////
    // SocketConnecter class
    // - supports connecting to a SocketListener

    class SocketConnecter : public Socket
    {
    public:
        SocketConnecter(const SocketConnecter& s) = delete;
        SocketConnecter& operator=(const SocketConnecter& s) = delete;

        SocketConnecter();
        SocketConnecter(SocketConnecter&& s);
        SocketConnecter& operator=(SocketConnecter&& s);
        virtual ~SocketConnecter();

        bool connect(const std::string& ip, size_t port);
    };

    /////////////////////////////////////////////////////////////////////////////
    // SocketListener class
    // - listens for incoming connections
    // - each connection is handled on its own thread

    class SocketListener : public Socket
    {
    public:
        SocketListener(const SocketListener& s) = delete;
        SocketListener& operator=(const SocketListener& s) = delete;

        SocketListener(size_t port, IpVer ipv = IP6);
        SocketListener(SocketListener&& s);
        SocketListener& operator=(SocketListener&& s);
        virtual ~SocketListener();

        template<typename CallObj>
        bool start(CallObj& co, std::string param);
        void stop();
 

    private:
        bool bind();
        bool listen();
        Socket accept();
        std::atomic<bool> stop_ = false;
        size_t port_;
        bool acceptFailed_ = false;
    };

    //----< SocketListener start function runs listener on its own thread >------
    /*
    *  - Accepts Callable Object that defines the operations
    *    to handle client requests.
    *  - You will find an example Callable Object, ClientProc,
    *    used in the test stub below
    */


    template<typename CallObj>
    bool SocketListener::start(CallObj& co, std::string param)
    {

        if (!bind())
        {
            return false;
        }

        if (!listen())
        {
            return false;
        }
        // listen on a dedicated thread so server's main thread won't block
        std::cout << "Completed Bind and Listen" << std::endl;

      
        

       // mapReducer.reduce(std::ref(outputFileName));
        //std::thread mapThread(&MapThreadFunction, mapDLLLocation, inputMapDirectory, outputMapDirectory, fileList, bufferSize, threadID, totalReduceThreads);
        std::thread ListenThread(
            [&, param, co]()
            {
                
            
                StaticLogger<1>::write("\n  -- server waiting for connection");

                while (!acceptFailed_)
                {
                    if (stop_.load())
                        break;

                    // Accept a client socket - blocking call
                    Socket clientSocket = accept();    // uses move ctor
                    if (!clientSocket.validState()) {
                        continue;
                    }
                    StaticLogger<1>::write("\n  -- server accepted connection");

                    // start thread to handle client request

                    // pass co by value to avoid interactions between threads
                   
                    //std::thread mapThread(&MapThreadFunction, mapDLLLocation, inputMapDirectory, outputMapDirectory, fileList, bufferSize, threadID, totalReduceThreads);
                    
                    
                    
                   // std::thread clientThread();


                    //
          
                    
                    std::thread clientThread(
                        co, std::ref(clientSocket)
                     );

                    std::thread::id tempThreadId = clientThread.get_id();
                    std::ostringstream conv;
                    conv << tempThreadId;
                    std::string sid = ", thread id = " + conv.str();
                    std::cout << "\n  created ClientHandler Thread " + sid;

                    clientThread.join();  // detach - listener won't access thread again

                
                }
                StaticLogger<1>::write("\n  -- Listen thread stopping");
            }
        );


       ListenThread.detach();

        return true;
    }




    void clientTestStringHandling(Socket& si);
    void clientRunMap(Socket& si, std::string param);
    

}

#endif

