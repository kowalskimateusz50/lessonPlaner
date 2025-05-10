
#include <iostream>
#include "programSettings.h"
#include "teacher.h"

using namespace std;

int main() 
{
    //Getting program settings 
    programSettings settings;
    settings.readProgramSettings();

    teacher firstTeacher;
    firstTeacher.readAvailability(settings.getInputFilePath());

    return 0;
}