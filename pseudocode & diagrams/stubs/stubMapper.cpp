#include <thread>
#include <chrono>
#include <iostream>
#include <fstream>
#include <Windows.h>
#include "stubMapper.h"
#include "MapLibrary.h"
//#include <unistd.h>

using namespace std;

struct parameters {
    int port;
    char url[256];
};


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
