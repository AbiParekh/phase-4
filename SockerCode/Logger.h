#pragma once
#ifndef LOGGER_H
#define LOGGER_H

#include <iostream>
#include <string>
#include <thread>
#include "Cpp11-BlockingQueue.h"

class Logger
{
public:
	Logger() {}
	void attach(std::ostream* pOut);
	void start();
	void stop(const std::string& msg = "");
	void write(const std::string& msg);
	void flush();
	void title(const std::string& msg, char underline = '-');
	~Logger();
	Logger(const Logger&) = delete;
	Logger& operator=(const Logger&) = delete;
private:
	std::thread* _pThr;
	std::ostream* _pOut;
	BlockingQueue<std::string> _queue;
	bool _ThreadRunning = false;
};

template<int i>
class StaticLogger
{
public:
	static void attach(std::ostream* pOut) { _logger.attach(pOut); }
	static void start() { _logger.start(); }
	static void stop(const std::string& msg = "") { _logger.stop(msg); }
	static void write(const std::string& msg) { _logger.write(msg); }
	static void flush() { _logger.flush(); }
	static void title(const std::string& msg, char underline = '-') { _logger.title(msg, underline); }
	static Logger& instance() { return _logger; }
	StaticLogger(const StaticLogger&) = delete;
	StaticLogger& operator=(const StaticLogger&) = delete;
private:
	static Logger _logger;
};

template<int i>
Logger StaticLogger<i>::_logger;

struct Cosmetic
{
	~Cosmetic() { std::cout << "\n\n"; }
};

#endif
