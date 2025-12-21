#pragma once

#include <iostream>
#include <string>
#include <fstream>
#include <logging.h>

namespace programConfig {
    constexpr int initialsBeginningRow = 2;
    constexpr int maxNoOfAvailableUnits = 10;
    constexpr int maxNoOfAvailableDays = 5;
}

class ProgramSettings
{
  private:
    std::string inputFilePath_;
    bool isLogModeOn_;
    bool isLogToConsoleOn_;
    bool isLogToFileOn_;
    std::string logFilePath_;

    enum class settingsFileLines
    {
      inputFile = 0,
      isLogModeOn,
      pathToLogFile
    };

  public:
    ProgramSettings();
    int readProgramSettings();
    bool isLogModeOn();
    bool isLogToConsoleOn();
    bool isLogToFileOn();
    std::string getInputFilePath();
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

