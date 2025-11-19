#pragma once

#include <iostream>
#include <string>
#include <fstream>
#include <logging.h>

using namespace std;

namespace programConfig {
    constexpr int initialsBeginningRow = 2;
}

class ProgramSettings
{
  private:
    string inputFilePath_;
    int isLogModeOn_;
    string logFilePath_;

    enum class settingsFileLines
    {
      inputFile = 0,
      isLogModeOn,
      pathToLogFile
    };

  public:
    ProgramSettings();
    int readProgramSettings();
    string getInputFilePath();
    string getLogFilePath();
    int isLogModeOn();
};

class avaiabilitySettings
{
  public:
    string initialsColumn;
    int startColumn;
    int endColumn;
    int initialsLength;
    int maxNoOfAvailableUnits;
    int maxNoOfAvailableDays;
    int availabilityMatrixRowOffset;
    int initialsToAvailabilityRowOffset;
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

