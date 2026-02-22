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
 * - Creating and writing to log output file named by actual date
 * - Providing logging function with possible to define severity level
 * 
 */
class Logging
{
	public:

    /**
     * @brief Enum class to represent logging level
     *
     */
    enum class LogLevel
    {
      Idle,                           ///< Idle log level (not used)
      Info,                           ///< Information log level
      Warning,                        ///< Warning log level
      Error                           ///< Error log level
    };

    /**
     * @brief Enum class to represent visibility modes in logs.
     * 
     */
    enum class LogMode
    {
      Disabled,                       ///< Log will not be printed
      Enabled                         ///< Log will be printed
    };

    /**
    * @brief Construct a new Logging object
    *
    * @param[in] isLogModeOn          ///< Setting up logging
    * @param[in] isLogToConsoleOn     ///< Setting up logging to console
    * @param[in] isLogToFileOn        ///< Setting up logging to file
    * @param[in] logFolderPath        ///< Setting up logging folder path
    */
    Logging(bool isLogModeOn,
            bool isLogToConsoleOn,
            bool isLogToFileOn,
            std::string logFolderPath);
    /**
     * @brief Destroy the Logging object
     * Closing log file if was opened
     */
    ~Logging();

    /**
     * @brief Function to print log
     * 
     * @param logLevel              ///< Log severity level described in enum class LogLevel
     * @param logMode               ///< Logging mode that controls log visibility
     * @param logMessage            ///< Message to be printed
     */
    void appendLog(LogLevel logLevel, LogMode logMode, std::string logMessage);

private:

    std::mutex mLock_;              ///< Mutex protecting concurrent access to the log output
    std::tm localTime_{};           ///< Cached local system time used for log timestamps

    bool isLogModeOn_;              ///< Enables or disables logging globally
    bool isLogToConsoleOn_;         ///< Enables logging output to the console
    bool isLogToFileOn_;            ///< Enables logging output to a file

    std::string logFolderPath_;     ///< Path to the directory containing log files
    std::string logFileName_;       ///< Name of the log file
    std::string logFilePath_;       ///< Full path to the log file
    std::fstream logFile_;          ///< File stream used for log file output

    /**
     * @brief Funtion to add 0 before number with one digit.
     *
     * @param[in] number number to which zero will be added
     * @return std::string with added zero before digit
     */
    std::string addLeadingZero(int number);

    /**
     * @brief Get the Log Level Type object
     *
     * @param logLevel input log level
     * @return std::string log level in string
     */
    std::string getLogLevelType(LogLevel logLevel);
};