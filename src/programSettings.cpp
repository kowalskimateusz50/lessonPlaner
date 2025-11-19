#include "programSettings.h"

ProgramSettings::ProgramSettings()
{
  //Initialize input file path
  inputFilePath_ = "";
  logFilePath_ = "";
  isLogModeOn_ = 0;
}

int ProgramSettings::readProgramSettings()
{
  uint readSettingsStatus = 0;
  ifstream settingsFile("ustawienia.txt");
  uint lineIterator = 0;
  string verifyLineContent = "";
  string verifyLinePattern = "";
  string line = "";

  if (settingsFile.is_open())
  {
    while(getline(settingsFile, line))
    {
      switch (lineIterator)
      {
        case 0:
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

        case 1:
          verifyLineContent = "";
          verifyLinePattern = "tryb_diagnostyczny='";
          for (int i = 0; i < verifyLinePattern.size(); i++)
          {
            verifyLineContent += line[i];
          }
          if (verifyLineContent == verifyLinePattern && line[line.size() - 1] == '\'')
          {
            string tmpIsLogModeOn = "";
            for (int i = verifyLinePattern.size(); i < (line.size() - 1) ; i++)
            {
              tmpIsLogModeOn += line[i];
            }
            if (tmpIsLogModeOn == "true")
            {
              isLogModeOn_ = 1;
            }
            else
            {
              isLogModeOn_ = 0;
            }
            readSettingsStatus++;
          }
          break;
          
        case 2:
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

string ProgramSettings::getInputFilePath()
{
    return inputFilePath_;
}

int ProgramSettings::isLogModeOn()
{
  return isLogModeOn_;
}

string ProgramSettings::getLogFilePath()
{
    return logFilePath_;
}
