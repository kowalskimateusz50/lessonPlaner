#pragma once

#include <iostream>
#include <fstream>
#include <mutex>
#include <string>
#include <fstream>
#include <chrono>
#include <ctime>
#include <filesystem>

namespace fs = std::filesystem;

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
                      std::string logFolderPath);
    /**
     * @brief 
     * 
     * @param logType 
     * @param isLogEnabled 
     * @param logMessage 
     */
    void appendLog(int logType, int isLogEnabled, std::string logMessage);

	private:
		std::mutex mLock_;

		time_t tTime;
	  tm* localTime;

    bool isLogModeOn_;
    bool isLogToConsoleOn_;
    bool isLogToFileOn_;
		std::string logFolderPath_;

	  std::string logFileName;
		std::string logFilePath;

		std::fstream logFile;

		std::string addLeadingZero(int number);
		std::string getMessageType(int type);
};