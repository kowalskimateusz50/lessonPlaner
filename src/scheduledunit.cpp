#include "scheduledunit.h"

ScheduledUnit::ScheduledUnit()
{
  // Initialize with empty values
  isScheduled_ = false;
}

bool ScheduledUnit::isScheduled()
{
  return isScheduled_;
}

bool ScheduledUnit::isFull()
{
  return (assignments_.size() >= programConfig::maxNoOfAssignmentsInUnit);
}

bool ScheduledUnit::hasThisTeacher(std::string teacherName)
{
  for (auto& assignment : assignments_)
  {
    for (auto& teacher : assignment.assignedTeachers)
    {
      if (teacher == teacherName)
      {
        return true;
      }
    }
  }

  return false;
}

void ScheduledUnit::scheduleUnit(Assignment assignment)
{
  if (assignments_.size() < programConfig::maxNoOfAssignmentsInUnit)
  {
    assignments_.push_back(assignment);
  }

  isScheduled_ = true;
}

std::string ScheduledUnit::getUnit()
{
  std::stringstream ssMessage;
  for (const auto& assignment : assignments_)
  {
    ssMessage << assignment.department;
    for (const auto& teacher : assignment.assignedTeachers)
    {
      ssMessage << " " << teacher;
    }
    if (assignments_.size() > 1)
    {
      ssMessage << std::endl;
    }
  }

  return ssMessage.str();
}
