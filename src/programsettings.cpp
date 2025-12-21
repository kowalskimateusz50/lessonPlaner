#include "programsettings.h"

ProgramSettings::ProgramSettings()
{
  //Initialize input file path
  isLogModeOn_ = true;
  isLogToConsoleOn_ = true;
  isLogToFileOn_ = true;
  inputFilePath_ = "";
  logFilePath_ = "";
}

int ProgramSettings::readProgramSettings()
{
  int readSettingsStatus = 0;
  std::ifstream settingsFile("ustawienia.txt");
  int lineIterator = 0;
  std::string verifyLineContent = "";
  std::string verifyLinePattern = "";
  std::string line = "";

  if (settingsFile.is_open())
  {
    while(getline(settingsFile, line))
    {
      switch (static_cast<settingsFileLines>(lineIterator))
      {
        case settingsFileLines::inputFile:
          verifyLineContent = "";
          verifyLinePattern = "plik_wsadowy='";
          for (int i = 0; i < verifyLinePattern.size(); i++)
          {
            verifyLineContent += line[i];
          }
          if (verifyLineContent == verifyLinePattern && line[line.size() - 1] == '\'')
          {
            for (int i = verifyLinePattern.size(); i < (line.size() - 1) ; i++)
            {
              inputFilePath_ += line[i];
            }
            readSettingsStatus++;
          }
          break;

        case settingsFileLines::outputFile:
          verifyLineContent = "";
          verifyLinePattern = "plik_wynikowy='";
          for (int i = 0; i < verifyLinePattern.size(); i++)
          {
            verifyLineContent += line[i];
          }
          if (verifyLineContent == verifyLinePattern && line[line.size() - 1] == '\'')
          {
            for (int i = verifyLinePattern.size(); i < (line.size() - 1) ; i++)
            {
              outputFilePath_ += line[i];
            }
            readSettingsStatus++;
          }
          break;

        case settingsFileLines::isLogModeOn:
          verifyLineContent = "";
          verifyLinePattern = "tryb_diagnostyczny='";
          for (int i = 0; i < verifyLinePattern.size(); i++)
          {
            verifyLineContent += line[i];
          }
          if (verifyLineContent == verifyLinePattern && line[line.size() - 1] == '\'')
          {
            std::string tmpIsLogModeOn = "";
            for (int i = verifyLinePattern.size(); i < (line.size() - 1) ; i++)
            {
              tmpIsLogModeOn += line[i];
            }
            tmpIsLogModeOn == "aktywny" ? true : false;
            readSettingsStatus++;
          }
          break;

        case settingsFileLines::pathToLogFile:
          verifyLineContent = "";
          verifyLinePattern = "plik_diagnostyczny='";
          for (int i = 0; i < verifyLinePattern.size(); i++)
          {
            verifyLineContent += line[i];
          }
          if (verifyLineContent == verifyLinePattern && line[line.size() - 1] == '\'')
          {
            for (int i = verifyLinePattern.size(); i < (line.size() - 1) ; i++)
            {
              logFilePath_ += line[i];
            }
            readSettingsStatus++;
          }
          break;
      }
      lineIterator++;
    }
  }
return readSettingsStatus;
}

bool ProgramSettings::isLogModeOn()
{
  return isLogModeOn_;
}

bool ProgramSettings::isLogToConsoleOn()
{
  return isLogToConsoleOn_;
}

bool ProgramSettings::isLogToFileOn()
{
  return isLogToFileOn_;
}

std::string ProgramSettings::getLogFilePath()
{
    return logFilePath_;
}

std::string ProgramSettings::getInputFilePath()
{
    return inputFilePath_;
}

std::string ProgramSettings::getOutputFilePath()
{
    return outputFilePath_;
}