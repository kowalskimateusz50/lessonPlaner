
#include <iostream>
#include <OpenXLSX.hpp>
#include "programSettings.h"

using namespace std;
using namespace OpenXLSX;

int main() 
{
    //Getting program settings 
    programSettings settings;
    settings.readProgramSettings();

    cout << "\nInput file path is: " << settings.getInputFilePath() << endl;

    return 0;
}