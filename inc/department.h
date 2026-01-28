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

    enum class Error
    {
      None,
      ScheduleCombinedUnits,
      ScheduleSingleUnit,
      NotProperlyAssigned,
    };

    department(OpenXLSX::XLWorksheet& wks,
               uint32_t& initialsRowPointer,
               Logging& logger);
    void setState(State state);
    State getState();
    void setError(Error error);
    Error getError();
    std::string stateToString(State state);
    std::string errorToString(Error error);
    void setScheduledCol(uint32_t col);
    bool isNotScheduledAtThisCol(uint32_t col);

  private:
    State state_;
    Error error_;
    uint32_t scheduledCol_;
};

