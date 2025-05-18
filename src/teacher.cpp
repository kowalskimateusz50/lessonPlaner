#include "teacher.h"

uint teacher::teacherInitialsReadLine = programConfig::initialsReadLineBegining;

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
        teacherInitialsReadLine += programConfig::availabilityMatrixOffset;
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
    string tempInitials =  cell.value().get<string>();
    //Check if initials are into criteria
    if (tempInitials.length() == programConfig::initialsLength) 
    {
        //Save initials to class
        initials = tempInitials;
        //Print initials to stdout
        cout << initials << endl;
        return 1;
    }

    return 0;
}

int teacher::readAvailabilityMatrix(const OpenXLSX::XLWorksheet& wks)
{
    //Initialize availability matrix vector
    availabilityMatrix = vector(programConfig::maxNoAvailabilityUnits, vector<int>(programConfig::maxNoAvailabilityDays, 0));

    for (int row = teacherInitialsReadLine + programConfig::initialsAvailabilityOffset, i = 0; row <= teacherInitialsReadLine + programConfig::maxNoAvailabilityUnits; row++, i++) 
    {
        for (int col = static_cast<int>(wksColumns::C), j = 0; col <= static_cast<int>(wksColumns::G); col++, j++) 
        {
            auto cell = wks.cell(XLCellReference(row, col));
            //save to vector 
            availabilityMatrix[i][j] = cell.value().get<int>(); 
        }
    }

    return 1;
}

void teacher::showAvailability()
{
    //Display initials 
    cout << "\nInitials: " << initials << endl;

    //Display availability matrix
    for (int units = 0; units < programConfig::maxNoAvailabilityUnits; units++) 
    {
        for (int days = 0; days < programConfig::maxNoAvailabilityDays; days++) 
        {
            //Print to stdout
            cout << availabilityMatrix[units][days] << " ";
        }
        cout << endl;
    }
}

