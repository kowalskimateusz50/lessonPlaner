#include "school.h"

namespace fs = std::filesystem;

school::school(ProgramSettings& programSettings, Logging& logger)
  : programSettings_(programSettings), logger_(logger) // initialization list
{
  // Open input data file
  prepareInputDataFile();

  // Open output data file
  prepareOutputDataFiles();

  // Initialize counters
  teachersCounter = 0;
  departmentsCounter = 0;
  assignmentsCounter = 0;
  scheduledDepartmentsCounter = 0;
}

school::~school()
{
  // Close input file
  inputFile_.close();

  // Close time plan file
  timePlanFile_.close();

  // Close teacher plan file
  teacherPlanFile_.close();
}

void school::prepareInputDataFile()
{
  //Preparing input file
  inputFile_.open(programSettings_.getInputFilePath());
  //Open input file worksheet
  inputFileWks_ = inputFile_.workbook().worksheet(1);
}

void school::prepareOutputDataFiles()
{
  std::string timePlanFilePath = programSettings_.getTimePlanFilePath();
  std::string teacherPlanFilePath = programSettings_.getTeacherPlanFilePath();

  fs::path filePathObj(timePlanFilePath);
  fs::path directory = filePathObj.parent_path();

    // Create folder if doesn't exists
    if (!directory.empty() && !fs::exists(directory))
    {
        try
        {
            fs::create_directories(directory);
            std::cout << "Created directory: "<< directory << '\n';
        }
        catch (const fs::filesystem_error& e)
        {
            std::cerr << "Directory creation failed: "
                      << e.what() << '\n';
            return;
        }
    }

    // Open and create output wks

    try
    {
        timePlanFile_.create(timePlanFilePath, true);   // Create new time plan file wks
        timePlanFile_.open(timePlanFilePath);     // Open new time plan file path wks
        teacherPlanFile_.create(teacherPlanFilePath, true);   // Create new time plan file wks
        teacherPlanFile_.open(teacherPlanFilePath);     // Open new time plan file path wks
    }
    catch (const std::exception& e)
    {
        std::cerr << "Failed to create/open XLSX file: " << e.what() << '\n';
        return;
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
  teachersCounter = 0;
  uint rowPointer = 2;
  std::stringstream logMessage;

  while (true)
  {
    teacher readTeacher(inputFileWks_, rowPointer, logger_);

    if (!readTeacher.readAvailability())
    {
      break;
    }

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

  while (true)
  {
    department readDepartment(inputFileWks_, rowPointer, logger_);

    if (!readDepartment.readAvailability())
    {
      break;
    }

    departmentsCounter++;
    readDepartment.setState(department::State::ScheduleCombinedUnits);
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
  for (auto& department : departments_)
  {
    department.showAvailability();
  }
}

int school::readTeachersAssignment()
{
  assignmentsCounter = 0;
  uint rowPointer = 3;
  std::stringstream logMessage;

  while (true)
  {
    TeacherAssigner readTeacherAssignment(inputFileWks_, rowPointer, logger_);

    if (!readTeacherAssignment.readAssignment())
    {
      break;
    }

    assignmentsCounter++;

    //Clear log stringstream
    logMessage.str("");
    logMessage.clear();

    logMessage << "Found new assignment, no.: " << assignmentsCounter << endl;
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

uint school::findLowestAvailableDepartment(std::vector<department>& departments,
                                           std::vector<teacher>& teachers,
                                           std::vector<TeacherAssigner>& assignments,
                                           std::array<std::array<ScheduledUnit, 
                                           programConfig::maxNoOfAvailableDays>, 
                                           programConfig::maxNoOfAvailableUnits>& scheduledTimeplan)
{
  // Set lowest value as max possible to schedule + 1
  uint lowestAvailabilityUnits = static_cast<uint>(programConfig::maxNoOfAvailableUnits *
    programConfig::maxNoOfAvailableDays + 1);
  uint lowestAvailabilityIndex = std::numeric_limits<uint>::max();
  uint counterOfAvailableUnits = std::numeric_limits<uint>::max();

  // Selecting combined units
  for (int i = 0; i < departments.size(); i++)
  {
    if (departments[i].getState() == department::State::ScheduleCombinedUnits)
    {
      counterOfAvailableUnits = 0;

      // 1. Get teachers assigned to this departments
      std::vector<std::string> assignedTeachers;
      std::string departmentName = departments[i].getName();

      for (int asit = 0; asit < assignments.size(); asit++)
      {
        if (departmentName == assignments[asit].getAssignedDepartment())
        {
          assignedTeachers = assignments[asit].getAssignedTeachers();
          break;
        }
      }

      // 2. Get department and teachers avaiability matrix
      std::vector<std::vector<int>> departmentAvailability = departments[i].getAvailabilityVector();
      std::vector<std::vector<std::vector<int>>> teachersAvailability;
      for (uint ite = 0; ite < assignedTeachers.size(); ite++)
      {
        for (uint j = 0; j < teachers.size(); j++)
        {
          if (assignedTeachers[ite] == teachers[j].getName())
          {
            teachersAvailability.emplace_back(teachers[j].getAvailabilityVector());
          }
        }
      }

      std::size_t noOfAssignedTeachers = teachersAvailability.size();
      // 3. Count availability
      for (uint uRow = 0; uRow < departmentAvailability.size(); uRow++)
      {
        for (uint uCol = 0; uCol < departmentAvailability[uRow].size(); uCol++)
        {
          uint unitIsSuitableForTeachers = 0;
          for (uint it = 0; it < noOfAssignedTeachers; it++)
          {
            if ((teachersAvailability[it][uRow][uCol] == 1) &&
                (departmentAvailability[uRow][uCol] == 1) &&
                (!scheduledTimeplan[uRow][uCol].isFull(noOfAssignedTeachers)))
            {
              unitIsSuitableForTeachers++;
            }
          }
          if (unitIsSuitableForTeachers == noOfAssignedTeachers)
          {
            counterOfAvailableUnits++;
          }
        }
      }

      // 4. Check whether counted availability is lower than previous
      if (counterOfAvailableUnits < lowestAvailabilityUnits)
      {
        lowestAvailabilityUnits = counterOfAvailableUnits;
        lowestAvailabilityIndex = i;
      }
    }
  }
  if (lowestAvailabilityIndex != std::numeric_limits<uint>::max())
  {
    return lowestAvailabilityIndex;
  }

  // Selecting single units
  for (int i = 0; i < departments.size(); i++)
  {
    if (departments[i].getState() == department::State::ScheduleSingleUnit)
    {
      counterOfAvailableUnits = 0;

      // 1. Get teachers assigned to this departments
      std::vector<std::string> assignedTeachers;
      std::string departmentName = departments[i].getName();

      for (int asit = 0; asit < assignments.size(); asit++)
      {
        if (departmentName == assignments[asit].getAssignedDepartment())
        {
          assignedTeachers = assignments[asit].getAssignedTeachers();
          break;
        }
      }

      // 2. Get department and teachers avaiability matrix
      std::vector<std::vector<int>> departmentAvailability = departments[i].getAvailabilityVector();
      std::vector<std::vector<std::vector<int>>> teachersAvailability;
      for (uint ite = 0; ite < assignedTeachers.size(); ite++)
      {
        for (uint j = 0; j < teachers.size(); j++)
        {
          if (assignedTeachers[ite] == teachers[j].getName())
          {
            teachersAvailability.emplace_back(teachers[j].getAvailabilityVector());
          }
        }
      }

      std::size_t noOfAssignedTeachers = teachersAvailability.size();
      // 3. Count availability
      for (uint uRow = 0; uRow < departmentAvailability.size(); uRow++)
      {
        for (uint uCol = 0; uCol < departmentAvailability[uRow].size(); uCol++)
        {
          uint unitIsSuitableForTeachers = 0;
          for (uint it = 0; it < noOfAssignedTeachers; it++)
          {
            if ((teachersAvailability[it][uRow][uCol] == 1) &&
                (departmentAvailability[uRow][uCol] == 1) &&
                (!scheduledTimeplan[uRow][uCol].isFull(noOfAssignedTeachers)))
            {
              unitIsSuitableForTeachers++;
            }
          }
          if (unitIsSuitableForTeachers == noOfAssignedTeachers)
          {
            counterOfAvailableUnits++;
          }
        }
      }

      // 4. Check whether counted availability is lower than previous
      if (counterOfAvailableUnits < lowestAvailabilityUnits)
      {
        lowestAvailabilityUnits = counterOfAvailableUnits;
        lowestAvailabilityIndex = i;
      }
    }
  }

  return lowestAvailabilityIndex;
}
bool school::findSuitableUnit(std::vector<teacher>& teachers,
              department& department,
              std::vector<TeacherAssigner>& assignments,
              uint& unitRowIndex,
              uint& unitColIndex,
              uint& assignmentIndex)
{
  // 1. Get indexes of teachers related to departments
  std::vector<std::string> assignedTeachers;
  std::string departmentName = department.getName();

  for (uint i = 0; i < assignments.size(); i++)
  {
    if (departmentName == assignments[i].getAssignedDepartment())
    {
      assignedTeachers = assignments[i].getAssignedTeachers();
      assignmentIndex = i;
      break;
    }
  }

  if (assignmentIndex == std::numeric_limits<uint>::max())
  {
    department.setState(department::State::NotProperlyAssigned);
    return false;
  }

  // 2. Prepare department availability and teachers availability
  std::vector<std::vector<int>> departmentAvailability = department.getAvailabilityVector();
  std::vector<std::vector<std::vector<int>>> teachersAvailability;
  std::vector<std::string> teachersNames;  

  for (uint i = 0; i < assignedTeachers.size(); i++)
  {
    for (uint j = 0; j < teachers.size(); j++)
    {
      if (assignedTeachers[i] == teachers[j].getName())
      {
        teachersAvailability.emplace_back(teachers[j].getAvailabilityVector());
        teachersNames.emplace_back(teachers[j].getName());
      }
    }
  }

  // 3. Select 1st unit suitable for department and for teachers,
  // depends on department scheduling state
  switch (department.getState())
  {
    // Scheduling of combined unit
    case department::State::ScheduleCombinedUnits:
    {
      uint unitIsSuitableForTeachers = 0;
      std::size_t noOfAssignedTeachers = teachersAvailability.size();

      // 1st schedule in the most efficient way
      for (uint uRow = 0; uRow < (departmentAvailability.size() - 1); uRow++)
      {
        for (uint uCol = 0; uCol < departmentAvailability[uRow].size(); uCol++)
        {
          // Debug: scheduling logic: Supress if not used
          //std::string departmentToDebug = "1TRM";
          //if (departmentName == departmentToDebug)
          //{
          //  std::cout << "Department debug: " << departmentToDebug;
          //}

          // Verify whether actual unit and unit underneath him is not full and has no already
          // scheduled this department
          if ((scheduledTimeplan_[uRow][uCol].isScheduled()) && 
              (scheduledTimeplan_[uRow + 1][uCol].isScheduled()) &&
              (scheduledTimeplan_[uRow][uCol].isOptimalTeacherUsage(noOfAssignedTeachers)) && 
              (scheduledTimeplan_[uRow + 1][uCol].isOptimalTeacherUsage(noOfAssignedTeachers)) &&
              !(scheduledTimeplan_[uRow][uCol].hasThisDepartment(departmentName)) &&
              !(scheduledTimeplan_[uRow + 1][uCol].hasThisDepartment(departmentName)))
          {
            unitIsSuitableForTeachers = 0;
            for (uint it = 0; it < noOfAssignedTeachers; it++)
            {
              if ((departmentAvailability[uRow][uCol] == 1) &&
                  (teachersAvailability[it][uRow][uCol] == 1) &&
                  (departmentAvailability[uRow + 1][uCol] == 1) &&
                  (teachersAvailability[it][uRow + 1][uCol] == 1) && 
                  (!scheduledTimeplan_[uRow][uCol].hasThisTeacher(assignedTeachers[it])) &&
                  (!scheduledTimeplan_[uRow + 1][uCol].hasThisTeacher(assignedTeachers[it])) &&
                  (scheduledTimeplan_[uRow][uCol].isSuitableYearDifference(departmentName)) &&
                  (scheduledTimeplan_[uRow + 1][uCol].isSuitableYearDifference(departmentName)))
              {
                unitIsSuitableForTeachers++;
              }
            }
            if (unitIsSuitableForTeachers == noOfAssignedTeachers)
            {
              unitRowIndex = uRow;
              unitColIndex = uCol;

              // Check whether everything has been successfully scheduled
              if ((unitRowIndex != std::numeric_limits<uint>::max()) &&
                  (unitColIndex != std::numeric_limits<uint>::max()))
              {
                department.setScheduledCol(uCol);

                // Sign this teachers as busy then
                for (uint i = 0; i < assignedTeachers.size(); i++)
                {
                  for (uint j = 0; j < teachers.size(); j++)
                  {
                    if (assignedTeachers[i] == teachers[j].getName())
                    {
                      teachers[j].storeAvailability(unitRowIndex, unitColIndex, 0);
                    }
                  }
                }
                // Assign next state of scheduling 
                department.setState(department::State::ScheduledCombinedUnits);
                return true;
              }
            }
          }
        }
      }

      // 2nd schedule in the scheduled but not full units
      for (uint uRow = 0; uRow < (departmentAvailability.size() - 1); uRow++)
      {
        for (uint uCol = 0; uCol < departmentAvailability[uRow].size(); uCol++)
        {
          // Debug: scheduling logic: Supress if not used
          //std::string departmentToDebug = "1TRM";
          //if (departmentName == departmentToDebug)
          //{
          //  std::cout << "Department debug: " << departmentToDebug;
          //}

          // Verify whether actual unit and unit underneath him is not full and has no already
          // scheduled this department
          if ((scheduledTimeplan_[uRow][uCol].isScheduled()) && 
              (scheduledTimeplan_[uRow + 1][uCol].isScheduled()) &&
              !(scheduledTimeplan_[uRow][uCol].isFull(noOfAssignedTeachers)) &&
              !(scheduledTimeplan_[uRow + 1][uCol].isFull(noOfAssignedTeachers)) &&
              !(scheduledTimeplan_[uRow][uCol].hasThisDepartment(departmentName)) &&
              !(scheduledTimeplan_[uRow + 1][uCol].hasThisDepartment(departmentName)))
          {
            unitIsSuitableForTeachers = 0;
            for (uint it = 0; it < noOfAssignedTeachers; it++)
            {
              if ((departmentAvailability[uRow][uCol] == 1) &&
                  (teachersAvailability[it][uRow][uCol] == 1) &&
                  (departmentAvailability[uRow + 1][uCol] == 1) &&
                  (teachersAvailability[it][uRow + 1][uCol] == 1) && 
                  (!scheduledTimeplan_[uRow][uCol].hasThisTeacher(assignedTeachers[it])) &&
                  (!scheduledTimeplan_[uRow + 1][uCol].hasThisTeacher(assignedTeachers[it])) &&
                  (scheduledTimeplan_[uRow][uCol].isSuitableYearDifference(departmentName)) &&
                  (scheduledTimeplan_[uRow + 1][uCol].isSuitableYearDifference(departmentName)))
              {
                unitIsSuitableForTeachers++;
              }
            }
            if (unitIsSuitableForTeachers == noOfAssignedTeachers)
            {
              unitRowIndex = uRow;
              unitColIndex = uCol;

              // Check whether everything has been successfully scheduled
              if ((unitRowIndex != std::numeric_limits<uint>::max()) &&
                  (unitColIndex != std::numeric_limits<uint>::max()))
              {
                department.setScheduledCol(uCol);

                // Sign this teachers as busy then
                for (uint i = 0; i < assignedTeachers.size(); i++)
                {
                  for (uint j = 0; j < teachers.size(); j++)
                  {
                    if (assignedTeachers[i] == teachers[j].getName())
                    {
                      teachers[j].storeAvailability(unitRowIndex, unitColIndex, 0);
                    }
                  }
                }
                // Assign next state of scheduling 
                department.setState(department::State::ScheduledCombinedUnits);
                return true;
              }
            }
          }
        }
      }

      // 3rd schedule in the standard way
      for (uint uRow = 0; uRow < (departmentAvailability.size() - 1); uRow++)
      {
        for (uint uCol = 0; uCol < departmentAvailability[uRow].size(); uCol++)
        {
          // Debug: scheduling logic: Supress if not used
          //std::string departmentToDebug = "1TRM";
          //if (departmentName == departmentToDebug)
          //{
          //  std::cout << "Department debug: " << departmentToDebug;
          //}

          // Verify whether actual unit and unit underneath him is not full and has no already
          // scheduled this department
          if (!(scheduledTimeplan_[uRow][uCol].isFull(noOfAssignedTeachers)) &&
              !(scheduledTimeplan_[uRow + 1][uCol].isFull(noOfAssignedTeachers)) &&
              !(scheduledTimeplan_[uRow][uCol].hasThisDepartment(departmentName)) &&
              !(scheduledTimeplan_[uRow + 1][uCol].hasThisDepartment(departmentName)))
          {
            unitIsSuitableForTeachers = 0;
            for (uint it = 0; it < noOfAssignedTeachers; it++)
            {
              if ((departmentAvailability[uRow][uCol] == 1) &&
                  (teachersAvailability[it][uRow][uCol] == 1) &&
                  (departmentAvailability[uRow + 1][uCol] == 1) &&
                  (teachersAvailability[it][uRow + 1][uCol] == 1) && 
                  (!scheduledTimeplan_[uRow][uCol].hasThisTeacher(assignedTeachers[it])) &&
                  (!scheduledTimeplan_[uRow + 1][uCol].hasThisTeacher(assignedTeachers[it])) &&
                  (scheduledTimeplan_[uRow][uCol].isSuitableYearDifference(departmentName)) &&
                  (scheduledTimeplan_[uRow + 1][uCol].isSuitableYearDifference(departmentName)))
              {
                unitIsSuitableForTeachers++;
              }
            }
            if (unitIsSuitableForTeachers == noOfAssignedTeachers)
            {
              unitRowIndex = uRow;
              unitColIndex = uCol;

              // Check whether everything has been successfully scheduled
              if ((unitRowIndex != std::numeric_limits<uint>::max()) &&
                  (unitColIndex != std::numeric_limits<uint>::max()))
              {
                department.setScheduledCol(uCol);

                // Sign this teachers as busy then
                for (uint i = 0; i < assignedTeachers.size(); i++)
                {
                  for (uint j = 0; j < teachers.size(); j++)
                  {
                    if (assignedTeachers[i] == teachers[j].getName())
                    {
                      teachers[j].storeAvailability(unitRowIndex, unitColIndex, 0);
                    }
                  }
                }
                // Assign next state of scheduling 
                department.setState(department::State::ScheduledCombinedUnits);
                return true;
              }
              else
              {
                department.setState(department::State::SchedulingImpossible);
                return false;
              }
            }
          }
        }
      }
    }

    //Scheduling of single unit
    case department::State::ScheduleSingleUnit:
    {
      uint unitIsSuitableForTeachers = 0;
      std::size_t noOfAssignedTeachers = teachersAvailability.size();

      std::string departmentToDebug = "3BSb";
      if (departmentName == departmentToDebug)
      {
        std::cout << "Department debug: " << departmentToDebug;
      }

      // 1st schedule in the most efficient way from teacher point of view
      for (uint uRow = 0; uRow < departmentAvailability.size(); uRow++)
      {
        for (uint uCol = 0; uCol < departmentAvailability[uRow].size(); uCol++)
        {
          // Verify whether actual unit is not full and not already has this department scheduled
          if ((scheduledTimeplan_[uRow][uCol].isScheduled()) &&
              (scheduledTimeplan_[uRow][uCol].isOptimalTeacherUsage(noOfAssignedTeachers)) &&
              (!scheduledTimeplan_[uRow][uCol].hasThisDepartment(departmentName)) &&
              (department.isNotScheduledAtThisCol(uCol)) &&
              (scheduledTimeplan_[uRow][uCol].isSuitableYearDifference(departmentName)))
          {
            unitIsSuitableForTeachers = 0;
            for (uint it = 0; it < noOfAssignedTeachers; it++)
            {
              if ((departmentAvailability[uRow][uCol] == 1) &&
                  ((teachersAvailability[it][uRow][uCol] == 1)) &&
                  (!scheduledTimeplan_[uRow][uCol].hasThisTeacher(assignedTeachers[it])))
              {
                unitIsSuitableForTeachers++;
              }
            }
            if (unitIsSuitableForTeachers == noOfAssignedTeachers)
            {
              unitRowIndex = uRow;
              unitColIndex = uCol;

              // Check whether everything has been successfully scheduled
              if ((unitRowIndex != std::numeric_limits<uint>::max()) &&
                  (unitColIndex != std::numeric_limits<uint>::max()))
              {
                // Sign this teachers as busy then
                for (uint i = 0; i < assignedTeachers.size(); i++)
                {
                  for (uint j = 0; j < teachers.size(); j++)
                  {
                    if (assignedTeachers[i] == teachers[j].getName())
                    {
                      teachers[j].storeAvailability(unitRowIndex, unitColIndex, 0);
                    }
                  }
                }
                // Assign next state of scheduling 
                department.setState(department::State::ScheduledSingleUnit);
                return true;
              }
            }
          }
        }
      }

      // 2nd schedule in the standard way
      for (uint uRow = 0; uRow < departmentAvailability.size(); uRow++)
      {
        for (uint uCol = 0; uCol < departmentAvailability[uRow].size(); uCol++)
        {
          // Verify whether actual unit is not full and not already has this department scheduled
          if ((scheduledTimeplan_[uRow][uCol].isScheduled()) &&
              (!scheduledTimeplan_[uRow][uCol].isFull(noOfAssignedTeachers)) &&
              (!scheduledTimeplan_[uRow][uCol].hasThisDepartment(departmentName)) &&
              (department.isNotScheduledAtThisCol(uCol)) &&
              (scheduledTimeplan_[uRow][uCol].isSuitableYearDifference(departmentName)))
          {
            unitIsSuitableForTeachers = 0;
            for (uint it = 0; it < noOfAssignedTeachers; it++)
            {
              if ((departmentAvailability[uRow][uCol] == 1) &&
                  ((teachersAvailability[it][uRow][uCol] == 1)) &&
                  (!scheduledTimeplan_[uRow][uCol].hasThisTeacher(assignedTeachers[it])))
              {
                unitIsSuitableForTeachers++;
              }
            }
            if (unitIsSuitableForTeachers == noOfAssignedTeachers)
            {
              unitRowIndex = uRow;
              unitColIndex = uCol;

              // Check whether everything has been successfully scheduled
              if ((unitRowIndex != std::numeric_limits<uint>::max()) &&
                  (unitColIndex != std::numeric_limits<uint>::max()))
              {
                // Sign this teachers as busy then
                for (uint i = 0; i < assignedTeachers.size(); i++)
                {
                  for (uint j = 0; j < teachers.size(); j++)
                  {
                    if (assignedTeachers[i] == teachers[j].getName())
                    {
                      teachers[j].storeAvailability(unitRowIndex, unitColIndex, 0);
                    }
                  }
                }
                // Assign next state of scheduling 
                department.setState(department::State::ScheduledSingleUnit);
                return true;
              }
            }
          }
        }
      }

      // 3rd schedule in the standard way
      for (uint uRow = 0; uRow < departmentAvailability.size(); uRow++)
      {
        for (uint uCol = 0; uCol < departmentAvailability[uRow].size(); uCol++)
        {
          // Verify whether actual unit is not full and not already has this department scheduled
          if ((!scheduledTimeplan_[uRow][uCol].isFull(noOfAssignedTeachers)) &&
              (!scheduledTimeplan_[uRow][uCol].hasThisDepartment(departmentName)) &&
              (department.isNotScheduledAtThisCol(uCol)) &&
              (scheduledTimeplan_[uRow][uCol].isSuitableYearDifference(departmentName)))
          {
            unitIsSuitableForTeachers = 0;
            for (uint it = 0; it < noOfAssignedTeachers; it++)
            {
              if ((departmentAvailability[uRow][uCol] == 1) &&
                  ((teachersAvailability[it][uRow][uCol] == 1)) &&
                  (!scheduledTimeplan_[uRow][uCol].hasThisTeacher(assignedTeachers[it])))
              {
                unitIsSuitableForTeachers++;
              }
            }
            if (unitIsSuitableForTeachers == noOfAssignedTeachers)
            {
              unitRowIndex = uRow;
              unitColIndex = uCol;

              // Check whether everything has been successfully scheduled
              if ((unitRowIndex != std::numeric_limits<uint>::max()) &&
                  (unitColIndex != std::numeric_limits<uint>::max()))
              {
                // Sign this teachers as busy then
                for (uint i = 0; i < assignedTeachers.size(); i++)
                {
                  for (uint j = 0; j < teachers.size(); j++)
                  {
                    if (assignedTeachers[i] == teachers[j].getName())
                    {
                      teachers[j].storeAvailability(unitRowIndex, unitColIndex, 0);
                    }
                  }
                }
                // Assign next state of scheduling 
                department.setState(department::State::ScheduledSingleUnit);
                return true;
              }
              else
              {
                department.setState(department::State::SchedulingImpossible);
                return false;
              }
            }
          }
        }
      }
    }
  }

  return false;
}


uint school::scheduleTimeTable()
{
  // Copy local copies of input data as read on
  std::vector<teacher> teachers = teachers_;
  std::vector<department> departments = departments_;
  std::vector<TeacherAssigner> assignments = assignments_;

  uint scheduledDepartments = 0;
  uint failedScheduled = 0;

  logger_.appendLog(M_INFO,
                    M_LOG_ENABLED,
                    (std::string)"LOG.12: school.cpp school::scheduleTimeTable()");

  // Main algorithm loop
  // For first test run loop will be disabled and only one run will be performed
  while (countNotScheduledDepartments(departments) > 0)
  {
    // 1. Find a department with the lowest availability
    uint indexOfDepartmentToSchedule = findLowestAvailableDepartment(departments,
                                                                     teachers,
                                                                     assignments,
                                                                     scheduledTimeplan_);

    std::stringstream logMessage;
  
    if (indexOfDepartmentToSchedule != std::numeric_limits<uint>::max())
    {
      logMessage << "Step 1: Found department to schedule: " << 
        indexOfDepartmentToSchedule << std::endl;

      // 2. Find a unit suitable for this class and schedule unit
      uint rowOfSuitableUnit = std::numeric_limits<uint>::max();
      uint colOfSuitableUnit = std::numeric_limits<uint>::max();
      uint assignmentIndex = std::numeric_limits<uint>::max();
      if (findSuitableUnit(teachers,
                            departments[indexOfDepartmentToSchedule],
                            assignments,
                            rowOfSuitableUnit,
                            colOfSuitableUnit,
                            assignmentIndex))
      {
        // Assign combined timeplan unit
        if (departments[indexOfDepartmentToSchedule].getState() == 
              department::State::ScheduledCombinedUnits)
        {
          /* Debug: if, comment if not used
          if  (departments[indexOfDepartmentToSchedule].getName() == "3BSc")
          {
            std::cout << "Debug: Scheduling of department: " << 
              departments[indexOfDepartmentToSchedule].getName();
          }
          */

          // Schedule 1st unit
          scheduledTimeplan_[rowOfSuitableUnit][colOfSuitableUnit].scheduleUnit(
            assignments_[assignmentIndex].getAssignment());
          // Schedule 2nd unit
          scheduledTimeplan_[rowOfSuitableUnit + 1][colOfSuitableUnit].scheduleUnit(
            assignments_[assignmentIndex].getAssignment());

          // chenge state to schedule sigle unit
          departments[indexOfDepartmentToSchedule].setState(department::State::ScheduleSingleUnit);

          logMessage << "Step 2: Found suitable unit to schedule combined: " << std::endl <<
            "1st row: " << rowOfSuitableUnit << " 1st col: " << colOfSuitableUnit << std::endl <<
              "1st scheduled unit: " <<
                scheduledTimeplan_[rowOfSuitableUnit][colOfSuitableUnit].getUnit() << std::endl <<
            " 2nd row: " << (rowOfSuitableUnit + 1) << " 2nd col: " << colOfSuitableUnit << std::endl <<
              " 2nd scheduled unit: " <<
                scheduledTimeplan_[rowOfSuitableUnit + 1][colOfSuitableUnit].getUnit() << 
                  std::endl << "Department state: " << 
                    departments[indexOfDepartmentToSchedule].stateToString(
                      departments[indexOfDepartmentToSchedule].getState());
        }
        // Assign single timeplan unit
        else if (departments[indexOfDepartmentToSchedule].getState() == 
              department::State::ScheduledSingleUnit)
        {
          // Schedule unit
          scheduledTimeplan_[rowOfSuitableUnit][colOfSuitableUnit].scheduleUnit(
            assignments_[assignmentIndex].getAssignment());

          // change state to as scheduled
          departments[indexOfDepartmentToSchedule].setState(department::State::Scheduled);

          // Update counters
          scheduledDepartments++;

          logMessage << "Step 2: Found suitable unit to schedule single: " << 
            " row: " << rowOfSuitableUnit << " col: " << colOfSuitableUnit << std::endl <<
              " scheduled unit: " << 
                scheduledTimeplan_[rowOfSuitableUnit][colOfSuitableUnit].getUnit() << 
                  std::endl << "Department state: " << 
                      departments[indexOfDepartmentToSchedule].stateToString(
                        departments[indexOfDepartmentToSchedule].getState());
        }
      }
      else
      {
        logMessage << "Step 2: FAILED: Unit suitable to schedule wasn't found for department: " << 
          departments[indexOfDepartmentToSchedule].getName() << std::endl << "Department state: " << 
            departments[indexOfDepartmentToSchedule].stateToString(
              departments[indexOfDepartmentToSchedule].getState());
        departments[indexOfDepartmentToSchedule].setState(department::State::SchedulingImpossible);
        failedScheduled++;
      }

    }
    else
    {
      logMessage << "Step 1: FAILED: Department wasn't found: " <<
        indexOfDepartmentToSchedule << std::endl;
      departments[indexOfDepartmentToSchedule].setState(department::State::SchedulingImpossible);
      failedScheduled++;
    }

    logger_.appendLog(M_INFO,
                      M_LOG_ENABLED,
                      (std::string)"LOG.12: school.cpp school::scheduleTimeTable()" +
                      logMessage.str());

    // Debug logic to allow 2 try and break loop
    //if (failedScheduled > 0)
    //{
    //  break;
    //}
  }

  scheduledDepartmentsCounter = scheduledDepartments;

  // Log summary report
  std::stringstream logMessage;
  logMessage << std::endl << "------------------ Summary report: ------------------" << std::endl <<
  "Read teachers: " << teachersCounter << std::endl <<
  "Read departments: " << departmentsCounter << std::endl <<
  "Read assignments: " << assignmentsCounter << std::endl <<
  "Scheduled departments: " << scheduledDepartmentsCounter << std::endl <<
  "Failed scheduling: " << failedScheduled;

  logger_.appendLog(M_INFO,
                    M_LOG_ENABLED,
                    (std::string)"LOG.13: school.cpp school::scheduleTimeTable()" +
                    logMessage.str());

  return scheduledDepartments;
}
/*
OpenXLSX::XLStyleIndex school::createHeaderStyle(OpenXLSX::XLDocument& doc)
{
    auto& styles = doc.styles();

    // FONT
    OpenXLSX::XLFont font;
    font.setBold(true);
    auto fontId = styles.fonts().add(font);

    // FILL (zielone tło)
    OpenXLSX::XLFill fill;
    fill.setPatternType(XLFillPatternValues::Solid);
    fill.setFgColor(XLColor(198, 224, 180));
    auto fillId = styles.fills().add(fill);

    // BORDER
    OpenXLSX::XLBorder border;
    border.left().setStyle(XLBorderStyleValues::Thin);
    border.right().setStyle(XLBorderStyleValues::Thin);
    border.top().setStyle(XLBorderStyleValues::Thin);
    border.bottom().setStyle(XLBorderStyleValues::Thin);
    auto borderId = styles.borders().add(border);

    // ALIGNMENT
    XLAlignment alignment;
    alignment.setHorizontal(XLAlignmentHorizontalValues::Center);
    alignment.setVertical(XLAlignmentVerticalValues::Center);
    alignment.setWrapText(true);

    // CELL FORMAT
    XLCellFormat format;
    format.setFontId(fontId);
    format.setFillId(fillId);
    format.setBorderId(borderId);
    format.setAlignment(alignment);

    return styles.cellFormats().add(format);
}
*/
void school::formatScheduleTable(OpenXLSX::XLDocument& doc,
                                 OpenXLSX::XLWorksheet& wks,
                                 const char startLetter,
                                 const int startRow,
                                 std::string headerText)
{

  //auto headerStyle = createHeaderStyle(doc);

  // Columns size formatting
  wks.column(startLetter).setWidth(5);
  wks.column(static_cast<char>(startLetter + 1)).setWidth(22);
  wks.column(static_cast<char>(startLetter + 2)).setWidth(22);
  wks.column(static_cast<char>(startLetter + 3)).setWidth(22);
  wks.column(static_cast<char>(startLetter + 4)).setWidth(22);
  wks.column(static_cast<char>(startLetter + 5)).setWidth(22);

  // Rows size formatting TODO: Eliminating magic numbers
  for (int row = startRow; row <= (startRow + 12); ++row)
  {
    wks.row(row).setHeight(45);
  }

  // Cell style formatting 
 //OpenXLSX::XLStyle cellStyle;
  //cellStyle.alignment().setWrapText(true);
 // cellStyle.alignment().setVertical(XLAlignmentVertical::Center);
 // cellStyle.border().setStyle(XLBorderStyle::Thin);

  // Header cells formatting
  std::string startCell = to_string(startLetter) + to_string(startRow);
  std::string cellsToMerge = startCell + ":" +
    to_string(static_cast<char>(startLetter + 5)) + to_string(startRow);
  wks.mergeCells(cellsToMerge);
  wks.cell(startCell).value() = headerText;
 // wks.cell(startCell).style() = headerStyle;

  // Fill days 
  std::array<string, 5> days = {
      "PONIEDZIAŁEK", "WTOREK", "ŚRODA", "CZWARTEK", "PIĄTEK"
  };

  for (int i = 0; i < days.size(); i++)
  {
      std::string cell = std::string(1, static_cast<char>(startLetter + 1 + i)) + to_string(startRow + 1);
      wks.cell(cell).value() = days[i];
      //wks.cell(cell).style() = headerStyle;
  }

  // Fill Units numbers
  for (int i = 1; i <= programConfig::maxNoOfAvailableUnits; ++i)
  {
      std::string cell = "A" + std::to_string(i + startRow + 2);
      wks.cell(cell).value() = i;
      //wks.cell(cell).style() = headerStyle;
  }

  // Format assignment cells
  for (std::size_t row = 0; row < programConfig::maxNoOfAvailableUnits; row++)
  {
    for (std::size_t col = 0; col < programConfig::maxNoOfAvailableDays; col++)
    {
      char letter = static_cast<char>(startLetter + col);
      int rowIndex = static_cast<int>(startRow + row);
      std::string cell = std::string(1, letter) + std::to_string(rowIndex);
      //wks.cell(cell).style() = cellStyle;
    }
  }

}

int school::writeScheduledTimeplan()
{

  // Write example data to worksheet
  timePlanWks_ = timePlanFile_.workbook().worksheet("Sheet1");
  
  constexpr char startLetter = 'A';
  constexpr int startRow = 1;

  formatScheduleTable(timePlanFile_, timePlanWks_, startLetter, startRow, "Plan lekcji");

  for (std::size_t row = 0; row < scheduledTimeplan_.size(); row++)
  {
    for (std::size_t col = 0; col < scheduledTimeplan_[row].size(); col++)
    {
      char letter = static_cast<char>(startLetter + col);
      int rowIndex = static_cast<int>(startRow + row);
      std::string cell = std::string(1, letter) + std::to_string(rowIndex);
      if (scheduledTimeplan_[row][col].isScheduled())
      {
        timePlanWks_.cell(cell).value() = scheduledTimeplan_[row][col].getUnit();
      }
      else
      {
        timePlanWks_.cell(cell).value() = "";
      }
    }
  }

  // Save file
  timePlanFile_.save();
  return 1;
}

uint school::countNotScheduledDepartments(std::vector<department>& departments)
{
  uint counter = 0;
  for (auto& department : departments)
  {
    if ((department.getState() == department::State::ScheduleCombinedUnits) ||
        (department.getState() == department::State::ScheduleSingleUnit))
    {
      counter++;
    }
  }
  return counter;
}

