#include "programSettings.h"

programSettings::programSettings()
{
    //Initialize input file path
    programSettings::inputFilePath = "";
}

int programSettings::readProgramSettings()
{
    uint readSettingsStatus = 0;
    ifstream settingsFile("ustawienia.txt");
    uint lineIterator = 0;
    string line = "";
    string verifyLinePattern="";
    string verifyLineContent="";

    if (settingsFile.is_open())
    {
        while(getline(settingsFile, line))
        {
          switch (lineIterator)
            case settingsFileLines::inputFile
              verifyLinePattern="plik_wsadowy='";
              for (int i = 0; i < verifyLinePattern.size(); i++)
              {
                verifyLineContent += line[i];
              }
              if (verifyLineContent == verifyLinePattern && line[line.size() - 1] == '\'')
              {
                for (int i = 6; i < line.size() - 1; i++)
                {
                    programSettings::inputFilePath += line[i];
                }
                readSettingsStatus++;
              }
        }
    }

    return 0;
}

string programSettings::getInputFilePath()
{
    return programSettings::inputFilePath;
}


