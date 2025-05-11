
#include <iostream>
#include "programSettings.h"
#include "school.h"

using namespace std;

int main() 
{
    //Getting program settings 
    programSettings settings;
    settings.readProgramSettings();

    school schoolClass;
    schoolClass.readTeacherAvailabilityFromFile(settings.getInputFilePath());

    return 0;
}