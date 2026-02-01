#pragma once

#include <iostream>
#include <fstream>
#include <mutex>
#include <string>
#include <chrono>
#include <ctime>
#include <filesystem>

/**
 * @brief Class responsible for application logging.
 *
 * This class provide a centralized interface to logging diagnostic data to file
 * with different severity levels.
 *
 * Responsibilities:
 * - Opening and writing to log output file.
 * - Controlling 
 *
 */
class Logging
{
	public:

    enum class LogLevel
    {
      Idle,
      Info,
      Warning,
      Error
    };

    enum class LogMode
    {
      Disabled,
      Enabled
    };

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
    ~Logging();
    void appendLog(LogLevel logLevel, LogMode logMode, std::string logMessage);

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
	  std::string getLogLevelType(LogLevel logLevel);
};