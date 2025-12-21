#include "scheduledunit.h"

ScheduledUnit::ScheduledUnit()
{
  // Initialize with empty values
  isScheduled_ = false;
  assignment_.year = 0;
  assignment_.department = "";
}

bool ScheduledUnit::isScheduled()
{
  return isScheduled_;
}

void ScheduledUnit::scheduleUnit(Assignment assignment)
{
  assignment_ = assignment;
  isScheduled_ = true;
}

std::string ScheduledUnit::getUnit()
{
  std::stringstream ssMessage;
  ssMessage << assignment_.year << assignment_.department;
  for (int i = 0; i < assignment_.assignedTeachers.size(); i++)
  {
    ssMessage << " " << assignment_.assignedTeachers[i];
  }

  return ssMessage.str();
}
