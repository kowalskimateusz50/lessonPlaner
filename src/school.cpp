#include "school.h"

school::school(ProgramSettings& settings, Logging& logger)
  : programSettings_(progamSettings_), logger_(logger) // initialization list
{
  // Open input data file
  prepareInputDataFile();


  // Initialize counters
  teachersCounter = 0;
  departmentsCounter = 0;
  assignmentsCounter = 0;
}

school::~school()
{
  //Close input file
  inputFile_.close();

  //Close output file TODO

}

school::prepareInputDataFile()
{
  //Preparing input file
  inputFile_.open(progamSettings.getInputFilePath());
  //Open input file worksheet
  inputFileWks_ = inputFile.workbook().worksheet(1);
}

school::prepareOutputDataFile()
{
  /* Create new local folder if doesn't exist */
  if (fs::create_directories(logFolderPath_))
  {
    std::cout << "Folder for log files created.\n";
  }
  else
  {
    std::cout << "Log file folder already exist.\n";
  }

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
  teacher readTeacher(inputFileWks_, rowPointer, logger_);

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
  department readDepartment(inputFileWks_, rowPointer, logger_);

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
  TeacherAssigner readTeacherAssignment(inputFileWks_, rowPointer, logger_);
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
template <typename T, std::size_t N>
bool school::findSuitableUnit(std::vector<teacher>& teachers,
              department department,
              std::vector<TeacherAssigner>& assignments,
              std::array<T, N>& scheduledTimeplan,
              int& unitRowIndex,
              int& unitColIndex,
              int& assignmentIndex)
{
  unitRowIndex = 0;
  unitColIndex = 0;
  assignmentIndex = 0;
  // 1. Get indexes of teachers related to departments
  std::vector<std::string> assignedTeachers;
  std::string departmentName = department.getName();
  for (int i = 0; i < assignments.size(); i++)
  {
    std::string assignedDepartmentName = std::to_string(assignments[i].getAssignedYear()) + 
      assignments[i].getAssignedDepartment();

    if (departmentName == assignedDepartmentName)
    {
      assignedTeachers = assignments[i].getAssignedTeachers();
      assignmentIndex = i;
      break;
    }
  }

  // 2. Prepare department availability and teachers availability
  std::vector<std::vector<int>> departmentAvailability = department.getAvailabilityVector();
  std::vector<std::vector<std::vector<int>>> teachersAvailability;

  for (int i = 0; i < assignedTeachers.size(); i++)
  {
      for (int j = 0; j < teachers.size(); j++)
      {
        if (assignedTeachers[i] == teachers[j].getName())
        {
          teachersAvailability.emplace_back(teachers[j].getAvailabilityVector());
        }
      }
  }

  uint unitIsSuitableForTeachers = 0;

  // 3. Select 1st unit suitable for department and for teachers
  for (uint uRow = 0; uRow < departmentAvailability.size(); uRow++)
  {
    for (uint uCol = 0; uCol < departmentAvailability[uRow].size(); uCol++)
    {
      if (!scheduledTimeplan[uRow][uCol].isScheduled())
      {
        unitIsSuitableForTeachers = 0;
        for (int it = 0; it < teachersAvailability.size(); it++)
        {
          if ((departmentAvailability[uRow][uCol] == 1) && 
              (teachersAvailability[it][uRow][uCol] == 1))
          {
            unitIsSuitableForTeachers++;
          }
        }
        if (unitIsSuitableForTeachers == teachersAvailability.size())
        {
          unitRowIndex = uRow;
          unitColIndex = uCol;

          // Sign this teachers as busy then
          for (int i = 0; i < assignedTeachers.size(); i++)
          {
            for (int j = 0; j < teachers.size(); j++)
            {
              if (assignedTeachers[i] == teachers[j].getName())
              {
                teachers[j].storeAvailability(unitRowIndex, unitColIndex, 0);
              }
            }
          }
          return true;
        }
      }
    }
  }

  return false;
}

bool school::scheduleTimeTable()
{
  // Copy local copies of input data as read on
  std::vector<teacher> teachers = teachers_;
  std::vector<department> departments = departments_;
  std::vector<TeacherAssigner> assignments = assignments_;

  bool foundPossibleTimeTable = false;

  logger_.appendLog(M_INFO,
                    M_LOG_ENABLED,
                    (std::string)"LOG.12: school.cpp school::scheduleTimeTable()");

  // Main algorithm loop
  // For first test run loop will be disabled and only one run will be performed
  //while (departments.size() > 0)
  //{
    // 1. Find a department with the lowest availability
    int indexOfDepartmentToSchedule = findLowestAvailableDepartment(departments);

    std::stringstream logMessage;
    
    if (indexOfDepartmentToSchedule > 0)
    {
      logMessage << "Step 1: Found department to schedule: " << 
        indexOfDepartmentToSchedule << std::endl;

      // 2. Find a unit suitable for this class and schedule unit
      int rowOfSuitableUnit = 0;
      int colOfSuitableUnit = 0;
      int assignmentIndex = 0;
      if (findSuitableUnit(teachers,
                          departments[indexOfDepartmentToSchedule],
                          assignments,
                          scheduledTimeplan_,
                          rowOfSuitableUnit,
                          colOfSuitableUnit,
                          assignmentIndex))
      {
        foundPossibleTimeTable = true;

        // Assign timeplan unit
        scheduledTimeplan_[rowOfSuitableUnit][colOfSuitableUnit].scheduleUnit(
          assignments_[assignmentIndex].getAssignment());

        logMessage << "Step 2: Found suitable unit to schedule: " << 
          "row: " << rowOfSuitableUnit << " col: " << colOfSuitableUnit << endl <<
            "scheduled unit: " << scheduledTimeplan_[rowOfSuitableUnit][colOfSuitableUnit].getUnit();
      }
      else
      {
        foundPossibleTimeTable = false;
        logMessage << "Step 2: FAILED: Unit suitable to schedule wasn't found: ";
      }
    }
    else
    {
      logMessage << "Step 1: FAILED: Department to schedule wasn't: ";
    }

    logger_.appendLog(M_INFO,
                      M_LOG_ENABLED,
                      (std::string)"LOG.12: school.cpp school::scheduleTimeTable()" +
                      logMessage.str());
  //}
  return foundPossibleTimeTable;
}