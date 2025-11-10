#include "department.h"

department::department()
{
    //Read settings TODO: More professional style of saving settings
    m_availabilitySettings.initialsColumn = "I";
    m_availabilitySettings.startColumn = static_cast<int>(wksColumns::J);
    m_availabilitySettings.endColumn = static_cast<int>(wksColumns::N);
    m_availabilitySettings.initialsLength = 2;
    m_availabilitySettings.maxNoOfAvailableUnits = 10;
    m_availabilitySettings.maxNoOfAvailableDays = 5;
    m_availabilitySettings.availabilityMatrixRowOffset = 12;
    m_availabilitySettings.initialsToAvailabilityRowOffset = 1;
}

int department::readAvailability(const OpenXLSX::XLWorksheet& wks, uint& initialsLinePointer)
{

    //Try to find initials
    if (findAndCheckInitials(wks, initialsLinePointer))
    {
      cout << "\nInitials found successfully:" << endl;
      //Read availability matrix
      if (readAvailabilityMatrix(wks, initialsLinePointer))
      {
        cout << "\nAvailability read successfully: " << endl;
      }
        else
        {
            cout << "\nAvailability read failed: " << endl;
            return 0;
        }

        //Increment initials read row pointer
        initialsLinePointer += m_availabilitySettings.availabilityMatrixRowOffset;
        return 1;
    }
    else
    {
        cout << "\nInitials wasn't found:" << endl;
        return 0;
    }
}

int department::findAndCheckInitials(const OpenXLSX::XLWorksheet& wks, uint& initialsRowPointer)
{
    string cellAdress = m_availabilitySettings.initialsColumn + to_string(initialsRowPointer);
    string tempInitials =  wks.cell(cellAdress).value().get<string>();
    //Check if initials are into criteria
    if ((tempInitials.length() == m_availabilitySettings.initialsLength) || 
        (tempInitials.length() == m_availabilitySettings.initialsLength + 1)) 
    {
        m_initials = tempInitials;
        //Print initials to stdout TODO: Debug settings
        cout << m_initials << endl;
        return 1;
    }

    return 0;
}

int department::readAvailabilityMatrix(const OpenXLSX::XLWorksheet& wks, uint& initialsRowPointer)
{
    //Initialize availability matrix vector
    m_availabilityMatrix = vector(m_availabilitySettings.maxNoOfAvailableUnits, 
                              vector<int>(m_availabilitySettings.maxNoOfAvailableDays, 0));

    for (int row = initialsRowPointer + m_availabilitySettings.initialsToAvailabilityRowOffset, i = 0;
         row <= initialsRowPointer + m_availabilitySettings.maxNoOfAvailableUnits;
         row++, i++)
    {
        for (int col = m_availabilitySettings.startColumn, j = 0;
             col <= m_availabilitySettings.endColumn; 
             col++, j++)
        {
            auto cell = wks.cell(XLCellReference(row, col));
            //save to vector
            m_availabilityMatrix[i][j] = cell.value().get<int>();
        }
    }

    return 1;
}

void department::showAvailability()
{
    //Display initials
    cout << "\nInitials: " << m_initials << endl;

    //Display availability matrix
    for (int units = 0; units < m_availabilitySettings.maxNoOfAvailableUnits; units++)
    {
        for (int days = 0; days < m_availabilitySettings.maxNoOfAvailableDays; days++)
        {
            //Print to stdout
            cout << m_availabilityMatrix[units][days] << " ";
        }
        cout << endl;
    }
}
