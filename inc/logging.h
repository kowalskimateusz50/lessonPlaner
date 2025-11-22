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

/**
 * @brief 
 * 
 */
class loggingToTextFile
{
	public:
    /**
     * @brief Construct a new logging To Text File object
     * 
     * @param isLogModeOn 
     * @param isLogToConsoleOn 
     * @param isLogToFileOn 
     * @param logFolderPath 
     */
    loggingToTextFile(bool isLogModeOn,
                      bool isLogToConsoleOn,
                      bool isLogToFileOn,
                      string logFolderPath);
		void appendLog(int logType, string logMessage);

	private:
		mutex mLock;

		time_t tTime;
	  tm* localTime;

    bool isLogModeOn_;
    bool isLogToConsoleOn_;
    bool isLogToFileOn_;
		string logFolderPath_;

	  string logFileName;
		string logFilePath;

		fstream logFile;

		string addLeadingZero(int number);
		string getMessageType(int type);
};