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
                      (std::string)"LOG.3: availability.cpp Availability::readAvailability() " +
                      "Initials wasn't found");
    return 0;
  }
  std::stringstream logMessage;

  logMessage << "\nInitials found successfully:" << std::endl;

  //Read availability matrix
  if (!readAvailabilityMatrix())
  {
    logger_.appendLog(M_ERROR,
                      M_LOG_ENABLED,
                      (std::string)"LOG.4: availability.cpp Availability::readAvailability() " +
                      "Availability matrix read failed");
    return 0;
  }

  logMessage << "\nInitials found successfully:" << std::endl;
  //Increment initials read row pointer
  initialsRowPointer_ += availabilitySettings_.availabilityMatrixRowOffset;

  logger_.appendLog(M_INFO,
                    M_LOG_ENABLED,
                    (std::string)"LOG.5: availability.cpp Availability::readAvailability() " +
                    logMessage.str());
  return 1;
}

int Availability::findAndCheckInitials()
{
  std::string cellAdress = availabilitySettings_.initialsColumn + std::to_string(initialsRowPointer_);
  std::string tempInitials =  wks_.cell(cellAdress).value().get<std::string>();
  //Check if initials are into criteria
  if ((tempInitials.length() >= availabilitySettings_.minInitialsLength) &&
      (tempInitials.length() <= availabilitySettings_.maxInitialsLength))
  {
    initials_ = tempInitials;
    std::stringstream logMessage;
    logMessage << "Found initials:" << initials_ << std::endl;
    logger_.appendLog(M_INFO,
                      M_LOG_ENABLED,
                      (std::string)"LOG.6: availability.cpp Availability::findAndCheckInitials() " +
                      logMessage.str());
    return 1;
  }
  return 0;
}

int Availability::readAvailabilityMatrix()
{
  //Initialize availability matrix vector
  availabilityMatrix_ = std::vector(availabilitySettings_.maxNoOfAvailableUnits, 
                          std::vector<int>(availabilitySettings_.maxNoOfAvailableDays, 0));

  for (int row = initialsRowPointer_ + availabilitySettings_.initialsToAvailabilityRowOffset, i = 0;
       row <= initialsRowPointer_ + availabilitySettings_.maxNoOfAvailableUnits;
       row++, i++)
  {
    for (int col = availabilitySettings_.startColumn, j = 0;
        col <= availabilitySettings_.endColumn; 
        col++, j++)
    {
      auto cell = wks_.cell(OpenXLSX::XLCellReference(row, col));
      availabilityMatrix_[i][j] = cell.value().get<int>();
    }
  }

  return 1;
}

void Availability::showAvailability() 
{
  std::stringstream logMessage;
  logMessage << "\nInitials: " << initials_<< std::endl;

  //Display availability matrix
  for (int units = 0; units < availabilitySettings_.maxNoOfAvailableUnits; units++) 
  {
      for (int days = 0; days < availabilitySettings_.maxNoOfAvailableDays; days++)
      {
          //Print to stdout
          logMessage << availabilityMatrix_[units][days] << " ";
      }
      logMessage << std::endl;
  }
  logger_.appendLog(M_INFO,
                  M_LOG_ENABLED,
                  (std::string)"LOG.7: availability.cpp Availability::showAvailability() " +
                  logMessage.str());
}

int Availability::countAvailabilityUnits() 
{
  int counter = 0;
  for (int units = 0; units < availabilitySettings_.maxNoOfAvailableUnits; units++) 
  {
      for (int days = 0; days < availabilitySettings_.maxNoOfAvailableDays; days++) 
      {
          if (availabilityMatrix_[units][days] == 1) 
          {
            counter++;
          }
      }
  }
  return counter;
}

std::string Availability::getName()
{
  return initials_;
}

const std::vector<std::vector<int>>& getAvailabilityVector()
{
  return availabilityMatrix_;
}


