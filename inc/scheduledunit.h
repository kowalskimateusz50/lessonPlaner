#include <iostream>
#include <vector>
#include <OpenXLSX.hpp>
#include "logging.h"
#include "teacherassigner.h"

class ScheduledUnit
{
  public:
    ScheduledUnit();
    bool isScheduled();
    void scheduleUnit();

  private:
    bool isScheduled_;
    Assignment assignment_;
};