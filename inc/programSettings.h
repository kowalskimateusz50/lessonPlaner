#pragma once

#include <iostream>
#include <string>
#include <fstream>
#include <logging.h>

using namespace std;

namespace programConfig {
    constexpr int initialsBeginningRow = 2;
}

class programSettings
{
  private:
    string m_inputFilePath;
    string m_logfilePath;
    int m_isLogModeOn;

    enum class settingsFileLines
    {
      inputFile = 0,
      isLogModeOn,
      pathToLogFile
    }

  public:
    programSettings();
    int readProgramSettings();
    string getInputFilePath();

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

