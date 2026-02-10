#pragma once

#include <iostream>
#include <string>
#include <fstream>
#include <logging.h>
#include <yaml-cpp/yaml.h>

/**
 * @brief Compile-time configuration constants using in reading and scheduling data.
 * 
 * This namepace contains all global configuration parameters using in 
 * reading and scheduling process of timetable generation.
 *
 */
namespace programConfig 
{
    constexpr int initialsBeginningRow = 2; ///< Row of xlsx file where to look for first initials
    constexpr int maxNoOfAvailableUnits = 10; ///< Maximum number of available lesson units
    constexpr int maxNoOfAvailableDays = 5; ///< Maximum number of available lesson days
    constexpr int maxNoOfTeachersInUnit = 4; ///< Maximum number of teachers in scheduled lesson unit
    constexpr int allowedYearDifference = 2; ///< Maximum year difference between departments in scheduled lesson unit
}

/**
 * @brief Class responsible for handling reading program settings from file
 *
 * This class is reading global program settings from yaml file with defined name.
 *
 */
class ProgramSettings
{

  public:
    /**
     * @brief Construct a new Program Settings object
     * 
     */
    ProgramSettings();
    
    /**
     * @brief Read program settings from file
     * 
     * @return int 1 if program settings have been read correctly, 0 otherwise
     */
    int readProgramSettings();
    /**
     * @brief Check whether logging mode is enabled
     * 
     * @return true if logging mode is enabled, false otherwise
     */
    bool isLogModeOn();
    
    /**
     * @brief Check whether logging to console is enabled
     * 
     * @return true if logging to console is enabled, false otherwise
     */
    bool isLogToConsoleOn();

    /**
     * @brief Check whether logging to file is enabled
     * 
     * @return true if logging to file is enabled, false otherwise
     */
    bool isLogToFileOn();

    /**
     * @brief Get the Input File Path object
     * 
     * @return std::string input file path
     */
    std::string getInputFilePath();

    /**
     * @brief Get the Time Plan File Path object
     * 
     * @return std::string timeplan file path
     */
    std::string getTimePlanFilePath();

    /**
     * @brief Get the Teacher Plan File Path object
     * 
     * @return std::string teacher plan file
     */
    std::string getTeacherPlanFilePath();

    /**
     * @brief Get the Log file path
     * 
     * @return std::string log file path
     */
    std::string getLogFilePath();

  private:
    std::string inputFilePath_; ///< read input file path
    std::string timePlanFilePath_; ///< read path to generate timeplan
    std::string teacherPlanFilePath_; ///< read path to generate teacherplan
    bool isLogModeOn_; ///< logging mode is turned on
    bool isLogToConsoleOn_; ///< console logging is enabled
    bool isLogToFileOn_; ///< file logging is enabled
    std::string logFilePath_; ///< read path to log file
};

/**
 * @brief Enum class is responsible for representing excel columns letters as numbers
 * 
 */
enum class wksColumns
{
    A = 1,
    B,
    C,
    D,
    E,
    F,
    G,
    H,
    I,
    J,
    K,
    L,
    M,
    N,
    O,
    P,
    Q,
    R,
    S,
    T
};