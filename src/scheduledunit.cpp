#include "scheduledunit.h"

ScheduledUnit::ScheduledUnit()
{
  // Initialize with empty values
  isScheduled_ = false;
  noOfAssignedTeachers_ = 0;
}

bool ScheduledUnit::isScheduled()
{
  return isScheduled_;
}

bool ScheduledUnit::isFull(std::size_t noOfAssignedTeachers)
{
  return ((noOfAssignedTeachers_ + static_cast<uint>(noOfAssignedTeachers)) >
          (programConfig::maxNoOfTeachersInUnit));
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
  if ((noOfAssignedTeachers_ + assignment.assignedTeachers.size()) <= 
        programConfig::maxNoOfTeachersInUnit)
  {
    assignments_.push_back(assignment);
    noOfAssignedTeachers_ += assignment.assignedTeachers.size();
    isScheduled_ = true;
  }
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
