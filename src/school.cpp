#include "school.h"

school::school(OpenXLSX::XLWorksheet& wks, Logging& logger)
  : wks_(wks), logger_(logger) {} // initialization list

/**
 * @brief Funtion to read teacher availability
 *
 * @param[in] wks reference to excel works sheet document
 *
 * @return amount of read teachers
 */
int school::readTeachersAvailability()
{
  int teacherCounter = 0;
  uint rowPointer = 2;
  stringstream logMessage;
  teacher readTeacher(wks_, rowPointer, logger_);

  while (readTeacher.readAvailability())
  {
      teacherCounter++;
      teachers_.push_back(readTeacher);
      logMessage << "\nFound teacher No.: " << teacherCounter << endl;
  }

  logMessage << "\nStopped searching teachers, found: " << teacherCounter << " teachers" << endl;

  logger_.appendLog(M_INFO,
                    M_LOG_ENABLED, 
                    (string)"LOG.2: school.cpp school::readTeachersAvailability()" +
                    logMessage.str());

  return teacherCounter;
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
  int departmentCounter = 0;
  uint rowPointer = 2;
  stringstream logMessage;
  department readDepartment(wks_, rowPointer, logger_);

  while (readDepartment.readAvailability())
  {
    departmentCounter++;
    departments_.push_back(readDepartment);
    logMessage << "\nFound department No.: " << departmentCounter << endl;
  }
  logMessage << "\nStopped searching departments: " << departmentCounter << endl;

  logger_.appendLog(M_INFO,
                    M_LOG_ENABLED,
                    (string)"LOG.8: school.cpp school::readDepartmentsAvailability()" +
                    logMessage.str());

  return departmentCounter;
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