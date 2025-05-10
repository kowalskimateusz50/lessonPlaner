#include "teacher.h"

uint teacher::teacherInitialsReadLine = 2;

teacher::teacher()
{
    

}

int teacher::readAvailability(string inputFilePath)
{
    
    OpenXLSX::XLDocument doc;

    //Open xlsx document
    doc.open(inputFilePath); 
    //Open worksheet
    auto wks = doc.workbook().worksheet(1);

    //Try to find initials
    if (findAndCheckInitials(wks))
    {
        cout << "\nInitials found successfully:" << endl;
    }
    else
    {
        cout << "\nInitials wasn't found:" << endl;
        //Close XLSX document
        doc.close();
        return 1;
    }

    //Read availability matrix
     





    //Close XLSX document
    doc.close();
    
    return 0;
}

int teacher::findAndCheckInitials(const OpenXLSX::XLWorksheet& wks)
{
    string cellAdress = "B" + to_string(this->teacherInitialsReadLine);
    //Read initials from file
    auto cell = wks.cell(cellAdress);
    string initials =  cell.value().get<string>();
    //Check if initials are into criteria
    if (initials.length() == 2) 
    {
        cout << initials << endl;
        //Increment initials read line 
        this->teacherInitialsReadLine += 12;
        return 1;
    }

    return 0;
}

