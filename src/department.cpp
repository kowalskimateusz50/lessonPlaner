#include "department.h"

department::department(OpenXLSX::XLWorksheet& wks,
                       uint& initialsRowPointer,
                       Logging& logger)
  : Availability(wks, initialsRowPointer, logger) // initialization list
{
  // Read settings TODO: More professional style of saving settings
  availabilitySettings_.initialsColumn = "I";
  availabilitySettings_.startColumn = static_cast<int>(wksColumns::J);
  availabilitySettings_.endColumn = static_cast<int>(wksColumns::N);
  availabilitySettings_.minInitialsLength = 2;
  availabilitySettings_.maxInitialsLength = 5;
  availabilitySettings_.maxNoOfAvailableUnits = 10;
  availabilitySettings_.maxNoOfAvailableDays = 5;
  availabilitySettings_.availabilityMatrixRowOffset = 12;
  availabilitySettings_.initialsToAvailabilityRowOffset = 1;
  // Always initialize department as not scheduled
  isScheduled_ = false;
}

bool department::isScheduled()
{
  return isScheduled_;
}
void department::setScheduledStatus(bool isScheduled)
{
  isScheduled_ = isScheduled;
}