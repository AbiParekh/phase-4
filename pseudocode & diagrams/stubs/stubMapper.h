//header file for stubMapper.cpp
#pragma once

#ifndef STUBMAPPER_H
#define STUBMAPPER_H

#pragma once

#include <thread>
#include <chrono>
#include <iostream>
#include <fstream>
#include <Windows.h>
#include "stubMapper.h"
//#include <unistd.h>

class stubMapper {

    public:
        stubMapper();
        ~stubMapper();
        bool createMapperThreads(void* (*func)(void*), parameters* param);

    protected:


    private:


}

#endif //STUBMAPPER_H
