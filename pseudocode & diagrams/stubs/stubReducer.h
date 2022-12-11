//header file for stubReducer.cpp
#pragma once

#include <thread>
#include <chrono>
#include <iostream>
#include <fstream>
#include <Windows.h>
#include "stubReducer.h"
//#include <unistd.h>

#ifndef STUBREDUCER_H
#define STUBREDUCER_H

class stubReducer {

    public:
        stubReducer();
        
        ~stubReducer();

        void reducerFunc();
        
        bool createFirstReducerThread(void* (*func)(void*), parameters* param);

        bool createSecondReducerThread(void* (*func)(void*), parameters* param);

    protected:


    private:

};


#endif //STUBREDUCER_H
