
#include <iostream>
#include "programSettings.h"
#include "school.h"

using namespace std;

int main() 
{
    //Getting program settings 
    programSettings settings;
    settings.readProgramSettings();

    //Open xlsx document
    OpenXLSX::XLDocument doc;
    doc.open(settings.getInputFilePath()); 
    //Open worksheet
    OpenXLSX::XLWorksheet wks = doc.workbook().worksheet(1);

    school schoolInstance;
    schoolInstance.readTeachersAvailability(wks);
    schoolInstance.showTeachersAvailability();

    return 0;
}