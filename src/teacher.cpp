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
        //Read availability matrix
        if (readAvailabilityMatrix(wks))
        {
            cout << "\nAvailability read successfully: " << endl;

        }
        else
        {
            cout << "\nAvailability read failed: " << endl;
            //Close XLSX document
            doc.close();
            return 0;
        }

        //Increment initials read line 
        teacherInitialsReadLine += 12;
        //Close XLSX document
        doc.close();
        return 1;
    }
    else
    {
        cout << "\nInitials wasn't found:" << endl;
        //Close XLSX document
        doc.close();
        return 0;
    }
}

int teacher::findAndCheckInitials(const OpenXLSX::XLWorksheet& wks)
{
    string cellAdress = "B" + to_string(teacherInitialsReadLine);
    //Read initials from file
    auto cell = wks.cell(cellAdress);
    string initials =  cell.value().get<string>();
    //Check if initials are into criteria
    if (initials.length() == 2) 
    {
        cout << initials << endl;
        return 1;
    }

    return 0;
}

int teacher::readAvailabilityMatrix(const OpenXLSX::XLWorksheet& wks)
{

    for (int row = teacherInitialsReadLine + 1; row <= teacherInitialsReadLine + 10; row++) 
    {
        for (int col = 3; col <= 7; ++col) 
        {
            auto cell = wks.cell(XLCellReference(row, col));
            cout << cell.value().get<int>() << " ";
        }

        cout << endl;
    }

    return 1;

}

