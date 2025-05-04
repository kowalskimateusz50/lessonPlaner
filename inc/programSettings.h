#pragma once

#include <iostream>
#include <string>
#include <fstream>

using namespace std;

class programSettings
{
    private:
    string inputFilePath;

    public:
    programSettings();
    int readProgramSettings();
    string getInputFilePath();

};
