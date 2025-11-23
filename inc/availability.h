#pragma once

#include <iostream>
#include <string>
#include <fstream>
#include <logging.h>
#include <OpenXLSX.hpp>
#include <vector>

using namespace std;
using namespace OpenXLSX;

class Availability
{
  public:
    class availabilitySettings
    {
      public:
        string initialsColumn;
        int startColumn;
        int endColumn;
        int minInitialsLength;
        int maxInitialsLength;
        int maxNoOfAvailableUnits;
        int maxNoOfAvailableDays;
        int availabilityMatrixRowOffset;
        int initialsToAvailabilityRowOffset;
    };

    Availability(OpenXLSX::XLWorksheet& wks,
                 uint& initialsRowPointer,
                 Logging& logger);

    int readAvailability();
    int findAndCheckInitials();
    int readAvailabilityMatrix();
    void showAvailability();

  protected:
    OpenXLSX::XLWorksheet& wks_;
    Logging& logger_;
    uint& initialsRowPointer_;

    availabilitySettings availabilitySettings_;
    string initials_;
    vector<vector<int>> availabilityMatrix_;
};

