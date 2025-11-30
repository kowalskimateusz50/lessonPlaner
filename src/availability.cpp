#include "availability.h"

Availability::Availability(OpenXLSX::XLWorksheet& wks,
                           uint& initialsRowPointer,
                           Logging& logger)
  : wks_(wks), initialsRowPointer_(initialsRowPointer), logger_(logger) {} // initialization list

int Availability::readAvailability()
{
  //Try to find initials
  if (!findAndCheckInitials())
  {
    logger_.appendLog(M_ERROR,
                      M_LOG_ENABLED,
                      (string)"LOG.3: availability.cpp Availability::readAvailability() " +
                      "Initials wasn't found");
    return 0;
  }
  stringstream logMessage;

  logMessage << "\nInitials found successfully:" << endl;

  //Read availability matrix
  if (!readAvailabilityMatrix())
  {
    logger_.appendLog(M_ERROR,
                      M_LOG_ENABLED,
                      (string)"LOG.4: availability.cpp Availability::readAvailability() " +
                      "Availability matrix read failed");
    return 0;
  }

  logMessage << "\nInitials found successfully:" << endl;
  //Increment initials read row pointer
  initialsRowPointer_ += availabilitySettings_.availabilityMatrixRowOffset;

  logger_.appendLog(M_INFO,
                    M_LOG_ENABLED,
                    (string)"LOG.5: availability.cpp Availability::readAvailability() " +
                    logMessage.str());
  return 1;
}

int Availability::findAndCheckInitials()
{
  string cellAdress = availabilitySettings_.initialsColumn + to_string(initialsRowPointer_);
  string tempInitials =  wks_.cell(cellAdress).value().get<string>();
  //Check if initials are into criteria
  if ((tempInitials.length() >= availabilitySettings_.minInitialsLength) &&
      (tempInitials.length() <= availabilitySettings_.maxInitialsLength))
  {
    initials_ = tempInitials;
    stringstream logMessage;
    logMessage << "Found initials:" << initials_ << endl;
    logger_.appendLog(M_INFO,
                      M_LOG_ENABLED,
                      (string)"LOG.6: availability.cpp Availability::findAndCheckInitials() " +
                      logMessage.str());
    return 1;
  }
  return 0;
}

int Availability::readAvailabilityMatrix()
{
  //Initialize availability matrix vector
  availabilityMatrix_ = vector(availabilitySettings_.maxNoOfAvailableUnits, 
                            vector<int>(availabilitySettings_.maxNoOfAvailableDays, 0));

  for (int row = initialsRowPointer_ + availabilitySettings_.initialsToAvailabilityRowOffset, i = 0;
       row <= initialsRowPointer_ + availabilitySettings_.maxNoOfAvailableUnits;
       row++, i++)
  {
    for (int col = availabilitySettings_.startColumn, j = 0;
        col <= availabilitySettings_.endColumn; 
        col++, j++)
    {
      auto cell = wks_.cell(XLCellReference(row, col));
      availabilityMatrix_[i][j] = cell.value().get<int>();
    }
  }

  return 1;
}

void Availability::showAvailability()
{
  stringstream logMessage;
  logMessage << "\nInitials: " << initials_<< endl;

  //Display availability matrix
  for (int units = 0; units < availabilitySettings_.maxNoOfAvailableUnits; units++)
  {
      for (int days = 0; days < availabilitySettings_.maxNoOfAvailableDays; days++)
      {
          //Print to stdout
          logMessage << availabilityMatrix_[units][days] << " ";
      }
      logMessage << endl;
  }
  logger_.appendLog(M_INFO,
                  M_LOG_ENABLED,
                  (string)"LOG.7: availability.cpp Availability::showAvailability() " +
                  logMessage.str());
}