#pragma once

#include <iostream>
#include <string>
#include <OpenXLSX.hpp>
#include "availability.h"
#include "programsettings.h"

/**
 * @brief Class which describes department
 *
 *
 *
 */
class department  : public Availability
{
  public:
    enum class State
    {
      Idle,
      ScheduleCombinedUnits,
      ScheduledCombinedUnits,
      ScheduleSingleUnit,
      ScheduledSingleUnit,
      Scheduled,
      NotProperlyAssigned,
      SchedulingImpossible
    };

    department(OpenXLSX::XLWorksheet& wks,
               uint& initialsRowPointer,
               Logging& logger);
    void setState(State state);
    State getState();
    std::string stateToString(State state);
    void setScheduledRow(uint row);
    bool isNotScheduledAtThisRow(uint row);

  private:
    State state_;
    uint scheduledRow_;
};

