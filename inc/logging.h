#pragma once

#include <iostream>
#include <fstream>
#include <mutex>
#include <string>
#include <fstream>
#include <chrono>
#include <ctime>
#include <filesystem>

using namespace std;
namespace fs = filesystem;

/* Define types of messages */
#define M_INFO 1
#define M_WARNING 2
#define M_ERROR 3

class loggingToTextFile
{
	public:
		loggingToTextFile(string iLogFolderPath);
		int appendLog(string ilogMessage, int iLogType);

	private:

		mutex mLock;

		time_t tTime;
	  tm* localTime;

	  string logFileName;
		string logFolderPath;
		string logFilePath;

		fstream logFile;

		string addLeadingZero(int number);
		string getMessageType(int type);
};