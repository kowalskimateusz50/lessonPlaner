#pragma once

#include <iostream>
#include <string>
#include <fstream>
#include <logging.h>

namespace programConfig 
{
    constexpr int initialsBeginningRow = 2;
    constexpr int maxNoOfAvailableUnits = 10;
    constexpr int maxNoOfAvailableDays = 5;
    constexpr int maxNoOfTeachersInUnit = 4;
    constexpr int allowedYearDifference = 2;
}

class ProgramSettings
{
  private:
    std::string inputFilePath_;
    std::string timePlanFilePath_;
    std::string teacherPlanFilePath_;
    bool isLogModeOn_;
    bool isLogToConsoleOn_;
    bool isLogToFileOn_;
    std::string logFilePath_;

    enum class settingsFileLines : int
    {
      inputFile = 0,
      timePlanFile,
      teacherPlanFile,
      isLogModeOn,
      logFile
    };

  public:
    ProgramSettings();
    int readProgramSettings();
    bool isLogModeOn();
    bool isLogToConsoleOn();
    bool isLogToFileOn();
    std::string getInputFilePath();
    std::string getTimePlanFilePath();
    std::string getTeacherPlanFilePath();    
    std::string getLogFilePath();
};

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

enum class availableDays 
{
    poniedzialek,
    wtorek,
    sroda,
    czwartek,
    piatek
};

