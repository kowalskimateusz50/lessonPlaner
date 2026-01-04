#pragma once

#include <iostream>
#include <string>
#include <fstream>
#include <logging.h>
#include <OpenXLSX.hpp>
#include <vector>

class Availability
{
  public:
    class availabilitySettings
    {
      public:
        std::string initialsColumn;
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
    int countAvailabilityUnits();
    std::string getName();
    const std::vector<std::vector<int>>& getAvailabilityVector() const;
    bool isAvailable(int row, int col);
    void storeAvailability(int row, int col, int availabilityStatus);

  protected:

    OpenXLSX::XLWorksheet& wks_;
    Logging& logger_;
    uint& initialsRowPointer_;

    availabilitySettings availabilitySettings_;
    std::string initials_;
    std::vector<std::vector<int>> availabilityMatrix_;
};
