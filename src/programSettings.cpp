#include "programSettings.h"

programSettings::programSettings()
{
    //Initialize input file path
    programSettings::inputFilePath = "";
}

int programSettings::readProgramSettings()
{
    ifstream settingsFile("ustawienia.txt");
    string line = "";
    string verifyLinePattern="plik='";
    string verifyLineContent="";

    if (settingsFile.is_open())
    {
        while(getline(settingsFile, line))
        {
            for (int i = 0; i < 6; i++)
            {
                verifyLineContent += line[i];
            }
            if (verifyLineContent == verifyLinePattern && line[line.size() - 1] == '\'')
            {
                for (int i = 6; i < line.size() - 1; i++)
                {
                    programSettings::inputFilePath += line[i];
                }
                return 1;
            }
        }
    }
    
    return 0;
}

string programSettings::getInputFilePath()
{
    return programSettings::inputFilePath;
}


