#include "department.h"

department::department(OpenXLSX::XLWorksheet& wks,
                       uint32_t& initialsRowPointer,
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
  state_ = State::Idle;
  scheduledCol_ = std::numeric_limits<uint32_t>::max();
}

department::State department::getState()
{
  return state_;
}

void department::setState(department::State state)
{
  state_ = state;
}

std::string department::stateToString(department::State state)
{
  switch (state)
  {
    case department::State::Idle: return "Idle";
    case department::State::ScheduleCombinedUnits: return "ScheduleCombinedUnits";
    case department::State::ScheduledCombinedUnits: return "ScheduledCombinedUnits";
    case department::State::ScheduleSingleUnit: return "ScheduleSingleUnit";
    case department::State::ScheduledSingleUnit: return "ScheduledSingleUnit";
    case department::State::Scheduled: return "Scheduled";
    case department::State::NotProperlyAssigned: return "NotProperlyAssigned";
    case department::State::SchedulingImpossible: return "SchedulingImpossible";
  }
  return "Unknown";
}

std::string department::stateToStringUI(department::State state)
{
  switch (state)
  {
    case department::State::Idle: return "Bezczynny";
    case department::State::ScheduleCombinedUnits: return "Rozplanuj łączoną jednostkę";
    case department::State::ScheduledCombinedUnits: return "Rozplanowano łączoną jednostkę";
    case department::State::ScheduleSingleUnit: return "Rozplanuj pojedyńczą jednostkę";
    case department::State::ScheduledSingleUnit: return "Rozplanowano pojedynczą jednostkę";
    case department::State::Scheduled: return "Rozplanowano";
    case department::State::NotProperlyAssigned: return "Nie poprawnie przypisany";
    case department::State::SchedulingImpossible: return "Nie możliwy do rozplanowania";
  }
  return "Nieznany";
}

void department::setScheduledCol(uint32_t col)
{
  scheduledCol_ = col;
}

bool department::isNotScheduledAtThisCol(uint32_t col)
{
  if ((scheduledCol_ != std::numeric_limits<uint32_t>::max()) &&
      (scheduledCol_ != col))
  {
    return true;
  }
  return false;
}

