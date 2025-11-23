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

#define M_LOG_ENABLED true
#define M_LOG_DISABLED false

/**
 * @brief 
 * 
 */
class Logging
{
	public:
    /**
    * @brief Construct a new Logging object
    * 
    * @param isLogModeOn 
    * @param isLogToConsoleOn 
    * @param isLogToFileOn 
    * @param logFolderPath 
    */
    Logging(bool isLogModeOn,
                      bool isLogToConsoleOn,
                      bool isLogToFileOn,
                      string logFolderPath);
    /**
     * @brief 
     * 
     * @param logType 
     * @param isLogEnabled 
     * @param logMessage 
     */
    void appendLog(int logType, int isLogEnabled, string logMessage);

	private:
		mutex mLock_;

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