#include "school.h"

school::school(OpenXLSX::XLWorksheet& wks, Logging& logger)
  : wks_(wks), logger_(logger) // initialization list
{
  // Initialize counters
  teachersCounter = 0;
  departmentsCounter = 0;
  assignmentsCounter = 0;
}
/**
 * @brief Funtion to read teacher availability
 *
 * @param[in] wks reference to excel works sheet document
 *
 * @return amount of read teachers
 */
int school::readTeachersAvailability()
{
  int teachersCounter = 0;
  uint rowPointer = 2;
  stringstream logMessage;
  teacher readTeacher(wks_, rowPointer, logger_);

  while (readTeacher.readAvailability())
  {
    teachersCounter++;
    teachers_.push_back(readTeacher);
    logMessage << "\nFound teacher No.: " << teachersCounter << endl;
  }

  logMessage << "\nStopped searching teachers, found: " << teachersCounter << " teachers" << endl;

  logger_.appendLog(M_INFO,
                    M_LOG_ENABLED, 
                    (string)"LOG.2: school.cpp school::readTeachersAvailability()" +
                    logMessage.str());

  return teachersCounter;
}
/**
 * @brief funtion to display teachers availability
 * 
 */
void school::showTeachersAvailability()
{
    for (int i = 0; i < teachers_.size(); i++)
    {
      teachers_[i].showAvailability();
    }
}

/**
 * @brief Funtion to read departments availability
 *
 * @param[in] wks reference to excel works sheet document
 *
 * @return amount of read departments
 */
int school::readDepartmentsAvailability()
{
  departmentsCounter = 0;
  uint rowPointer = 2;
  stringstream logMessage;
  department readDepartment(wks_, rowPointer, logger_);

  while (readDepartment.readAvailability())
  {
    departmentsCounter++;
    departments_.push_back(readDepartment);
    logMessage << "\nFound department No.: " << departmentsCounter << endl;
  }
  logMessage << "\nStopped searching departments: " << departmentsCounter << endl;

  logger_.appendLog(M_INFO,
                    M_LOG_ENABLED,
                    (string)"LOG.8: school.cpp school::readDepartmentsAvailability()" +
                    logMessage.str());

  return departmentsCounter;
}

/**
 * @brief funtion to display departments availability
 * 
 */
void school::showDepartmentsAvailability()
{
  for (int i = 0; i < departments_.size(); i++)
  {
    departments_[i].showAvailability();
  }
}

int school::readTeachersAssignment()
{
  assignmentsCounter = 0;
  uint rowPointer = 3;
  stringstream logMessage;
  TeacherAssigner readTeacherAssignment(wks_, rowPointer, logger_);
  while (readTeacherAssignment.readAssignment())
  {
    assignmentsCounter++;

    logMessage << "Found new assignment, no.: " << assignmentsCounter << endl;
    logMessage << "Year: " << readTeacherAssignment.getAssignedYear() << endl;
    logMessage << "Department: " << readTeacherAssignment.getAssignedDepartment() << endl;
 
    for (const auto& teacher : readTeacherAssignment.getAssignedTeachers())
    {
      logMessage << "Assigned teacher: " << teacher << endl;
    }

    assignments_.push_back(readTeacherAssignment);

    logger_.appendLog(M_INFO,
                      M_LOG_ENABLED,
                      (string)"LOG.9: school.cpp school::readTeachersAssignment()" +
                      logMessage.str());
  }

  logger_.appendLog(M_INFO,
                    M_LOG_ENABLED,
                    (string)"LOG.10: school.cpp school::readTeachersAssignment()" +
                    (string)"Teacher assignment wasn't, stop finding");

  return assignmentsCounter;
}

void school::showTeachersAssignment()
{
  stringstream logMessage;
 
  for (auto assignment : assignments_)
  {
    logMessage << "Year: " << assignment.getAssignedYear() << endl;
    logMessage << "Department: " << assignment.getAssignedDepartment() << endl;
 
    for (const auto& teacher : assignment.getAssignedTeachers())
    {
      logMessage << "Assigned teacher: " << teacher << endl;
    }
  }
  logger_.appendLog(M_INFO,
                    M_LOG_ENABLED,
                    (string)"LOG.11: school.cpp school::showTeachersAssignment()" +
                    logMessage.str());
}