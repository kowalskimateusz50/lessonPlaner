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
  std::stringstream logMessage;
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
                    (std::string)"LOG.2: school.cpp school::readTeachersAvailability()" +
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
  std::stringstream logMessage;
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
                    (std::string)"LOG.8: school.cpp school::readDepartmentsAvailability()" +
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
  std::stringstream logMessage;
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
  std::stringstream logMessage;

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

int school::findLowestAvailableDepartment(std::vector<department>& departments)
{
  int lowestAvailabilityUnits = departments[0].countAvailabilityUnits();
  int lowestAvailabityIndex = 0;

  for (int i = 1; i < departments.size(); i++)
  {
    int countAvailabilityUnits = departments[i].countAvailabilityUnits();
    if (countAvailabilityUnits < lowestAvailabilityUnits)
    {
      lowestAvailabilityUnits = departments[i].countAvailabilityUnits();
      lowestAvailabityIndex = i;
    }
  }
  return lowestAvailabityIndex;
}

int school::findSuitableUnit(std::vector<teacher>& teachers,
                             department department,
                             std::vector<TeacherAssigner>& assignments,
                             int& unitRowIndex,
                             int& unitColIndex)
{

  // 1. Get indexes of teachers related to departments
  std::vector<std::string> assignedTeachers;
  std::string departmentName = department.getName();
  for (int i = 0; i < assignments.size(); i++)
  {
    std::string assigneddepartmentName = std::to_string(assignments[i].getAssignedYear()) + 
      assignments[i].getAssignedDepartment();

    if (departmentName == assigneddepartmentName)
    {
      assignedTeachers = assignments[i].getAssignedTeachers();
      break;
    }
  }

  // 2. Prepare department availability and teachers availability
  std::vector<std::vector<std::vector<int>>> teachersAvailability;

  for (int i)
    getAvailabilityVector()


  // 3. Select 1st unit suitable for department and for teachers
  for (uint row = 0; row < department.size(); row++)
  {
    for (uint col = 0; col < department[row].size(); col++)
    {
      if ()

    }
  }
  
  return 0;
}

bool school::scheduleTimeTable()
{
  // Copy local copies of input data as read on
  std::vector<teacher> teachers = teachers_;
  std::vector<department> departments = departments_;
  std::vector<TeacherAssigner> assignments = assignments_;

  // Initialize vector with scheduled time plan

  bool foundPossibleTimeTable = false;

  logger_.appendLog(M_INFO,
                    M_LOG_ENABLED,
                    (std::string)"LOG.12: school.cpp school::scheduleTimeTable()");

  // Main algorithm loop
  //while (departmentsToSchedule.size() > 0)
  //{
    // 1. Find a department with the lowest availability
    int indexOfDepartmentToSchedule = findLowestAvailableDepartment(departments);

    std::stringstream logMessage;
    logMessage << "Step 1: Find department to schedule: " << indexOfDepartmentToSchedule;

    logger_.appendLog(M_INFO,
                      M_LOG_ENABLED,
                      (std::string)"LOG.12: school.cpp school::scheduleTimeTable()" +
                      logMessage.str());
    
    int rowOfSuitableUnit = 0;
    int colOfSuitableUnit = 0;
    
    // 2. Find a unit suitable for this class
    foundPossibleTimeTable = findSuitableUnit(teachers,
                                              departments[indexOfDepartmentToSchedule],
                                              assignments,
                                              rowOfSuitableUnit,
                                              colOfSuitableUnit);
    



  //}
  return foundPossibleTimeTable;
}