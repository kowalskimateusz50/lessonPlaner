
#include <iostream>
#include "programSettings.h"

using namespace std;
//using namespace OpenXLSX;

int main() 
{

    /*
    OpenXLSX::XLDocument doc;

    //Create doc document
    doc.create("example.xlsx"); 

    //Create worksheet
    XLWorksheet wks = doc.workbook().worksheet(1);

    wks.cell("A1") = "Hello";
    wks.cell("B1") = "World";

    //Closing XLSX document
    doc.save();
    doc.close();
    */

    //Getting program settings 
    programSettings settings;
    settings.readProgramSettings();

    cout << "\nInput file path is: " << settings.getInputFilePath() << endl;

    return 0;
}