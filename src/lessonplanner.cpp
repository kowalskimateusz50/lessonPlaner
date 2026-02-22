#include "lessonplanner.h"

LessonPlanner::RuntimeErrors LessonPlanner::run()
{
  if (auto result = load(); result != RuntimeErrors::Success)
      return result;

  if (auto result = show(); result != RuntimeErrors::Success)
      return result;

  if (auto result = process(); result != RuntimeErrors::Success)
      return result;

  if (auto result = save(); result != RuntimeErrors::Success)
      return result;

  return RuntimeErrors::Success;
}

LessonPlanner::RuntimeErrors LessonPlanner::load()
{
  // Load data from input file
  if (schoolInstance_.readTeachersAvailability() == 0)
  {
    return RuntimeErrors::TeachersAvailabilityLoad;
  }

  if (schoolInstance_.readDepartmentsAvailability() == 0)
  {
    return RuntimeErrors::DepartmentsAvailabilityLoad;
  }

  if (schoolInstance_.readTeachersAssignment() == 0)
  {
    return RuntimeErrors::TeachersAssignmentLoad;
  }

  return RuntimeErrors::Success;
}

LessonPlanner::RuntimeErrors LessonPlanner::show()
{
  // Show loaded data
  schoolInstance_.showTeachersAvailability();
  schoolInstance_.showDepartmentsAvailability();
  schoolInstance_.showTeachersAssignment();

  return RuntimeErrors::Success;
}

LessonPlanner::RuntimeErrors LessonPlanner::process()
{
  // Schedule timeplan
  if (schoolInstance_.scheduleTimeTable() == 0)
  {
    return RuntimeErrors::ScheduleTimePlanProcess;
  }

  return RuntimeErrors::Success;
}

LessonPlanner::RuntimeErrors LessonPlanner::save()
{
  schoolInstance_.writeScheduledTimePlan();
  schoolInstance_.writeScheduledTeacherPlan();

  return RuntimeErrors::Success;
}