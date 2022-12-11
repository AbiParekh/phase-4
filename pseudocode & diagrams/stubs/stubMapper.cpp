#include <thread>
#include <chrono>
#include <iostream>
#include <fstream>
#include <Windows.h>
#include "stubMapper.h"
//#include "MapLibrary.h"
//#include <unistd.h>

using namespace std;
using namespace std::literals::chrono_literals;

struct parameters {
    int port;
    char url[256];
    //other parameters can go here
};

static bool mThreadFinished = false;

void mapperFunc(){

    while(!mThreadFinished){
        //do stuff
        //sleep(1);
        std::cout << "Mapper thread running" << std::endl;
        std::this_thread::sleep_for(1s);
    }
}

bool stubMapper::createMapperThreads(void* (*func)(void*), parameters* param) {
    bool threadsCreated = false;

    if (threadsCreated == false) {
        std::thread Mapper1(func, param);
        std::thread Mapper2(func, param);

        threadsCreated = true;
    }
    
    else {

        cout << "Error creating threads" << endl;
        return false;
    }
    return true;
}
