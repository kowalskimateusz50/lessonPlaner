#include "scheduledunit.h"
#include <cmath>

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

bool ScheduledUnit::hasThisDepartment(std::string departmentName)
{
  for (auto& assignment : assignments_)
  {
    if (assignment.department == departmentName)
    {
      return true;
    }
  }
  return false;
}

void ScheduledUnit::scheduleUnit(Assignment assignment)
{
    assignments_.push_back(assignment);
    noOfAssignedTeachers_ += assignment.assignedTeachers.size();
    if (!isScheduled_)
    {
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

uint ScheduledUnit::getYearFromDepartmentName(std::string name)
{
  // Get year from first character in string
  char year = name[0];

  if (year >= '0' && year <= '9')
  {
    return year - '0';
  }
  
  return 0;
}

bool ScheduledUnit::isSuitableYearDifference(std::string departmentNameToSchedule)
{

  uint yearToSchedule = getYearFromDepartmentName(departmentNameToSchedule);

  if (assignments_.size() == 0)
  {
    return true;
  }

  for (const auto& assignment : assignments_)
  {
    uint scheduledYear = getYearFromDepartmentName(assignment.department);
    int yearDiff = static_cast<int>(scheduledYear) - static_cast<int>(yearToSchedule);
    if (std::abs(yearDiff) > programConfig::allowedYearDifference)
    {
      return false;
    }
  }

  return true;
}

bool ScheduledUnit::isOptimalTeacherUsage(std::size_t noOfAssignedTeachers)
{
  return ((noOfAssignedTeachers_ + static_cast<uint>(noOfAssignedTeachers)) ==
          (programConfig::maxNoOfTeachersInUnit));
}

