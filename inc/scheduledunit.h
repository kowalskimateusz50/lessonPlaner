#include <iostream>
#include <vector>
#include <OpenXLSX.hpp>
#include "logging.h"
#include "teacherassigner.h"
#include "programsettings.h"

class ScheduledUnit
{
  public:
    ScheduledUnit();
    bool isScheduled();
    bool isFull();
    bool hasThisTeacher(std::string teacherName);
    void scheduleUnit(Assignment assignment);
    std::string getUnit();

  private:
    bool isScheduled_;
    std::vector<Assignment> assignments_;
};