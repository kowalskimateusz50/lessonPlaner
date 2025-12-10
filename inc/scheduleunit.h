#include <iostream>
#include <vector>
#include <OpenXLSX.hpp>
#include "logging.h"
#include "teacherassigner.h"

class ScheduleUnit
{
  public:
    ScheduleUnit();


  private:
    bool isScheduled_;
    Assignment assignment_;
}