#include "teacher.h"

teacher::teacher(OpenXLSX::XLWorksheet& wks,
                 uint& initialsRowPointer,
                 Logging& logger)
  : Availability(wks, initialsRowPointer, logger) // initialization list
{
  //Read settings TODO: More professional style of saving settings
  availabilitySettings_.initialsColumn = "B";
  availabilitySettings_.startColumn = static_cast<int>(wksColumns::C);
  availabilitySettings_.endColumn = static_cast<int>(wksColumns::G);
  availabilitySettings_.minInitialsLength = 2;
  availabilitySettings_.maxInitialsLength = 2;
  availabilitySettings_.maxNoOfAvailableUnits = 10;
  availabilitySettings_.maxNoOfAvailableDays = 5;
  availabilitySettings_.availabilityMatrixRowOffset = 12;
  availabilitySettings_.initialsToAvailabilityRowOffset = 1;
}