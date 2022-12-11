#include <thread>
#include <chrono>
#include <iostream>
#include <fstream>
#include <Windows.h>
#include "stubReducer.h"
#include "ReduceLib.h"
//#include <unistd.h>


using namespace std;
using namespace std::literals::chrono_literals;

struct parameters {
    int port;
    char url[256];
    //any other parameters can go here
};
 
static bool rThreadFinished = false;

 void reducerFunc(){

    while(!rThreadFinished){
        //do stuff
        //sleep(1);
        std::cout << "Reducer thread running" << std::endl;
        std::this_thread::sleep_for(1s);
    }
}
bool stubReducer::createFirstReducerThread(void* (*func)(void*), parameters* param) {
    bool firstThreadCreated = false;

    if (firstThreadCreated == false) {
        std::thread reducer1(func, param);

        firstThreadCreated = true;
    }
    
    else {

        cout << "Error creating thread" << endl;
        return false;
    }
    return true;
}
    

bool stubReducer::createSecondReducerThread(void* (*func)(void*), parameters* param) {
    bool secondThreadCreated = false;

    if (secondThreadCreated == false) {
        std::thread reducer2(func, param);

        secondThreadCreated = true;
    }
    
    else {

        cout << "Error creating thread" << endl;
        return false;
    }
    return true;
    }
    


