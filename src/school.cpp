#include "school.h"

namespace fs = std::filesystem;

School::School(ProgramSettings& programSettings, Logging& logger)
  : programSettings_(programSettings), logger_(logger) // initialization list
{
  // Open input data file
  prepareInputDataFile();

  // Open output data file
  prepareOutputDataFiles();
}

School::~School()
{
  /// Close input file
  inputFile_.close();

  /// Close time plan file
  timePlanFile_.close();

  /// Close teacher plan file
  teacherPlanFile_.close();
}

void School::prepareInputDataFile()
{
  /// Preparing input file
  inputFile_.open(programSettings_.getInputFilePath());
  /// Open input file worksheet
  inputFileWks_ = inputFile_.workbook().worksheet(1);
}

void School::prepareOutputDataFiles()
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

  // Create and open time plan file
  try
  {
    timePlanFile_.create(timePlanFilePath, OpenXLSX::XLForceOverwrite);   // Create new time plan file
    timePlanFile_.open(timePlanFilePath);     // Open new time plan file path
  }
  catch (const std::exception& e)
  {
      std::cerr << "Failed to create/open XLSX file: " << e.what() << '\n';
      return;
  }

  // Create and open teacher plan file
  try
  {
    teacherPlanFile_.create(teacherPlanFilePath, OpenXLSX::XLForceOverwrite);   // Create new teacher plan file
    teacherPlanFile_.open(teacherPlanFilePath);     // Open new teacher plan file path
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
 * @return amount of read teachers
 */
int School::readTeachersAvailability()
{
  uint32_t rowPointer = 2;
  std::stringstream logMessage;

  while (true)
  {
    teacher readTeacher(inputFileWks_, rowPointer, logger_);

    if (!readTeacher.readAvailability())
    {
      break;
    }

    counters_.teachers++;
    teachers_.push_back(readTeacher);
    logMessage << "\nFound teacher No.: " << counters_.teachers << endl;
  }

  logMessage << "\nStopped searching teachers, found: " << counters_.teachers << " teachers" << endl;

  logger_.appendLog(Logging::LogLevel::Info,
                    Logging::LogMode::Enabled, 
                    (std::string)"LOG.2: School.cpp School::readTeachersAvailability()" +
                    logMessage.str());

  return counters_.teachers;
}
/**
 * @brief funtion to display teachers availability
 * 
 */
void School::showTeachersAvailability()
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
int School::readDepartmentsAvailability()
{
  uint32_t rowPointer = 2;
  std::stringstream logMessage;

  while (true)
  {
    department readDepartment(inputFileWks_, rowPointer, logger_);

    if (!readDepartment.readAvailability())
    {
      break;
    }

    counters_.departments++;
    readDepartment.setState(department::State::ScheduleCombinedUnits);
    departments_.push_back(readDepartment);
    logMessage << "\nFound department No.: " << counters_.departments << endl;
  }
  logMessage << "\nStopped searching departments: " << counters_.departments << endl;

  logger_.appendLog(Logging::LogLevel::Info,
                    Logging::LogMode::Enabled,
                    (std::string)"LOG.8: School.cpp School::readDepartmentsAvailability()" +
                    logMessage.str());

  return counters_.departments;
}

/**
 * @brief funtion to display departments availability
 * 
 */
void School::showDepartmentsAvailability()
{
  for (auto& department : departments_)
  {
    department.showAvailability();
  }
}

int School::readTeachersAssignment()
{
  uint32_t rowPointer = 3;
  std::stringstream logMessage;

  while (true)
  {
    TeacherAssigner readTeacherAssignment(inputFileWks_, rowPointer, logger_);

    if (!readTeacherAssignment.readAssignment())
    {
      break;
    }

    counters_.assignments++;

    //Clear log stringstream
    logMessage.str("");
    logMessage.clear();

    logMessage << "Found new assignment, no.: " << counters_.assignments << endl;
    logMessage << "Department: " << readTeacherAssignment.getAssignedDepartment() << endl;
 
    for (const auto& teacher : readTeacherAssignment.getAssignedTeachers())
    {
      logMessage << "Assigned teacher: " << teacher << endl;
    }

    assignments_.push_back(readTeacherAssignment);

    logger_.appendLog(Logging::LogLevel::Info,
                      Logging::LogMode::Enabled,
                      (string)"LOG.9: School.cpp School::readTeachersAssignment()" +
                      logMessage.str());
  }

  logger_.appendLog(Logging::LogLevel::Info,
                    Logging::LogMode::Enabled,
                    (string)"LOG.10: School.cpp School::readTeachersAssignment()" +
                    (string)"Teacher assignment wasn't, stop finding");

  return counters_.assignments;
}

void School::showTeachersAssignment()
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
  logger_.appendLog(Logging::LogLevel::Info,
                    Logging::LogMode::Enabled,
                    (string)"LOG.11: School.cpp School::showTeachersAssignment()" +
                    logMessage.str());
}

uint32_t School::countNotScheduledDepartments(std::vector<department>& departments)
{
  uint32_t counter = 0;
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

uint32_t School::findLowestAvailableDepartment(std::vector<department>& departments,
                                               std::vector<teacher>& teachers,
                                               std::vector<TeacherAssigner>& assignments,
                                               std::array<std::array<ScheduledUnit, 
                                               programConfig::maxNoOfAvailableDays>, 
                                               programConfig::maxNoOfAvailableUnits>& scheduledTimeplan)
{
  // Set lowest value as max possible to schedule + 1
  uint32_t lowestAvailabilityUnits = static_cast<uint32_t>(programConfig::maxNoOfAvailableUnits *
    programConfig::maxNoOfAvailableDays + 1);
  uint32_t lowestAvailabilityIndex = std::numeric_limits<uint32_t>::max();
  uint32_t counterOfAvailableUnits = std::numeric_limits<uint32_t>::max();

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
      for (uint32_t ite = 0; ite < assignedTeachers.size(); ite++)
      {
        for (uint32_t j = 0; j < teachers.size(); j++)
        {
          if (assignedTeachers[ite] == teachers[j].getName())
          {
            teachersAvailability.emplace_back(teachers[j].getAvailabilityVector());
          }
        }
      }

      std::size_t noOfAssignedTeachers = teachersAvailability.size();
      // 3. Count availability
      for (uint32_t uRow = 0; uRow < departmentAvailability.size(); uRow++)
      {
        for (uint32_t uCol = 0; uCol < departmentAvailability[uRow].size(); uCol++)
        {
          uint32_t unitIsSuitableForTeachers = 0;
          for (uint32_t it = 0; it < noOfAssignedTeachers; it++)
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
  if (lowestAvailabilityIndex != std::numeric_limits<uint32_t>::max())
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
      for (uint32_t ite = 0; ite < assignedTeachers.size(); ite++)
      {
        for (uint32_t j = 0; j < teachers.size(); j++)
        {
          if (assignedTeachers[ite] == teachers[j].getName())
          {
            teachersAvailability.emplace_back(teachers[j].getAvailabilityVector());
          }
        }
      }

      std::size_t noOfAssignedTeachers = teachersAvailability.size();
      // 3. Count availability
      for (uint32_t uRow = 0; uRow < departmentAvailability.size(); uRow++)
      {
        for (uint32_t uCol = 0; uCol < departmentAvailability[uRow].size(); uCol++)
        {
          uint32_t unitIsSuitableForTeachers = 0;
          for (uint32_t it = 0; it < noOfAssignedTeachers; it++)
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

bool School::findSuitableUnit(std::vector<teacher>& teachers,
              department& department,
              std::vector<TeacherAssigner>& assignments,
              uint32_t& unitRowIndex,
              uint32_t& unitColIndex,
              uint32_t& assignmentIndex)
{
  // 1. Get indexes of teachers related to departments
  std::vector<std::string> assignedTeachers;
  std::string departmentName = department.getName();

  for (uint32_t i = 0; i < assignments.size(); i++)
  {
    if (departmentName == assignments[i].getAssignedDepartment())
    {
      assignedTeachers = assignments[i].getAssignedTeachers();
      assignmentIndex = i;
      break;
    }
  }

  if (assignmentIndex == std::numeric_limits<uint32_t>::max())
  {
    department.setState(department::State::NotProperlyAssigned);
    department.setError(department::Error::NotProperlyAssigned);
    return false;
  }

  // 2. Prepare department availability and teachers availability
  std::vector<std::vector<int>> departmentAvailability = department.getAvailabilityVector();
  std::vector<std::vector<std::vector<int>>> teachersAvailability;
  std::vector<std::string> teachersNames;  

  for (uint32_t i = 0; i < assignedTeachers.size(); i++)
  {
    for (uint32_t j = 0; j < teachers.size(); j++)
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
      uint32_t unitIsSuitableForTeachers = 0;
      std::size_t noOfAssignedTeachers = teachersAvailability.size();

      // 1st schedule in the most efficient way
      for (uint32_t uRow = 0; uRow < (departmentAvailability.size() - 1); uRow++)
      {
        for (uint32_t uCol = 0; uCol < departmentAvailability[uRow].size(); uCol++)
        {
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
            for (uint32_t it = 0; it < noOfAssignedTeachers; it++)
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
              if ((unitRowIndex != std::numeric_limits<uint32_t>::max()) &&
                  (unitColIndex != std::numeric_limits<uint32_t>::max()))
              {
                department.setScheduledCol(uCol);

                // Sign this teachers as busy then
                for (uint32_t i = 0; i < assignedTeachers.size(); i++)
                {
                  for (uint32_t j = 0; j < teachers.size(); j++)
                  {
                    if (assignedTeachers[i] == teachers[j].getName())
                    {
                      teachers[j].storeAvailability(unitRowIndex, unitColIndex, 0);
                    }
                  }
                }
                // Assign next state of scheduling 
                department.setState(department::State::ScheduledCombinedUnits);
                department.setError(department::Error::None);
                return true;
              }
            }
          }
        }
      }

      // 2nd schedule in the scheduled but not full units
      for (uint32_t uRow = 0; uRow < (departmentAvailability.size() - 1); uRow++)
      {
        for (uint32_t uCol = 0; uCol < departmentAvailability[uRow].size(); uCol++)
        {

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
            for (uint32_t it = 0; it < noOfAssignedTeachers; it++)
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
              if ((unitRowIndex != std::numeric_limits<uint32_t>::max()) &&
                  (unitColIndex != std::numeric_limits<uint32_t>::max()))
              {
                department.setScheduledCol(uCol);

                // Sign this teachers as busy then
                for (uint32_t i = 0; i < assignedTeachers.size(); i++)
                {
                  for (uint32_t j = 0; j < teachers.size(); j++)
                  {
                    if (assignedTeachers[i] == teachers[j].getName())
                    {
                      teachers[j].storeAvailability(unitRowIndex, unitColIndex, 0);
                    }
                  }
                }
                // Assign next state of scheduling
                department.setState(department::State::ScheduledCombinedUnits);
                department.setError(department::Error::None);
                return true;
              }
            }
          }
        }
      }

      // 3rd schedule in the standard way
      for (uint32_t uRow = 0; uRow < (departmentAvailability.size() - 1); uRow++)
      {
        for (uint32_t uCol = 0; uCol < departmentAvailability[uRow].size(); uCol++)
        {
          // Verify whether actual unit and unit underneath him is not full and has no already
          // scheduled this department
          if (!(scheduledTimeplan_[uRow][uCol].isFull(noOfAssignedTeachers)) &&
              !(scheduledTimeplan_[uRow + 1][uCol].isFull(noOfAssignedTeachers)) &&
              !(scheduledTimeplan_[uRow][uCol].hasThisDepartment(departmentName)) &&
              !(scheduledTimeplan_[uRow + 1][uCol].hasThisDepartment(departmentName)))
          {
            unitIsSuitableForTeachers = 0;
            for (uint32_t it = 0; it < noOfAssignedTeachers; it++)
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
              if ((unitRowIndex != std::numeric_limits<uint32_t>::max()) &&
                  (unitColIndex != std::numeric_limits<uint32_t>::max()))
              {
                department.setScheduledCol(uCol);

                // Sign this teachers as busy then
                for (uint32_t i = 0; i < assignedTeachers.size(); i++)
                {
                  for (uint32_t j = 0; j < teachers.size(); j++)
                  {
                    if (assignedTeachers[i] == teachers[j].getName())
                    {
                      teachers[j].storeAvailability(unitRowIndex, unitColIndex, 0);
                    }
                  }
                }
                // Assign next state of scheduling
                department.setState(department::State::ScheduledCombinedUnits);
                department.setError(department::Error::None);
                return true;
              }
            }
          }
        }
      }
      department.setState(department::State::SchedulingImpossible);
      department.setError(department::Error::ScheduleCombinedUnits);
      return false;
    }

    //Scheduling of single unit
    case department::State::ScheduleSingleUnit:
    {
      uint32_t unitIsSuitableForTeachers = 0;
      std::size_t noOfAssignedTeachers = teachersAvailability.size();

      // 1st schedule in the most efficient way from teacher point of view
      for (uint32_t uRow = 0; uRow < departmentAvailability.size(); uRow++)
      {
        for (uint32_t uCol = 0; uCol < departmentAvailability[uRow].size(); uCol++)
        {
          // Verify whether actual unit is not full and not already has this department scheduled
          if ((scheduledTimeplan_[uRow][uCol].isScheduled()) &&
              (scheduledTimeplan_[uRow][uCol].isOptimalTeacherUsage(noOfAssignedTeachers)) &&
              (!scheduledTimeplan_[uRow][uCol].hasThisDepartment(departmentName)) &&
              (department.isNotScheduledAtThisCol(uCol)) &&
              (scheduledTimeplan_[uRow][uCol].isSuitableYearDifference(departmentName)))
          {
            unitIsSuitableForTeachers = 0;
            for (uint32_t it = 0; it < noOfAssignedTeachers; it++)
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
              if ((unitRowIndex != std::numeric_limits<uint32_t>::max()) &&
                  (unitColIndex != std::numeric_limits<uint32_t>::max()))
              {
                // Sign this teachers as busy then
                for (uint32_t i = 0; i < assignedTeachers.size(); i++)
                {
                  for (uint32_t j = 0; j < teachers.size(); j++)
                  {
                    if (assignedTeachers[i] == teachers[j].getName())
                    {
                      teachers[j].storeAvailability(unitRowIndex, unitColIndex, 0);
                    }
                  }
                }
                // Assign next state of scheduling 
                department.setState(department::State::ScheduledSingleUnit);
                department.setError(department::Error::None);
                return true;
              }
            }
          }
        }
      }

      // 2nd schedule in the standard way
      for (uint32_t uRow = 0; uRow < departmentAvailability.size(); uRow++)
      {
        for (uint32_t uCol = 0; uCol < departmentAvailability[uRow].size(); uCol++)
        {
          // Verify whether actual unit is not full and not already has this department scheduled
          if ((scheduledTimeplan_[uRow][uCol].isScheduled()) &&
              (!scheduledTimeplan_[uRow][uCol].isFull(noOfAssignedTeachers)) &&
              (!scheduledTimeplan_[uRow][uCol].hasThisDepartment(departmentName)) &&
              (department.isNotScheduledAtThisCol(uCol)) &&
              (scheduledTimeplan_[uRow][uCol].isSuitableYearDifference(departmentName)))
          {
            unitIsSuitableForTeachers = 0;
            for (uint32_t it = 0; it < noOfAssignedTeachers; it++)
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
              if ((unitRowIndex != std::numeric_limits<uint32_t>::max()) &&
                  (unitColIndex != std::numeric_limits<uint32_t>::max()))
              {
                // Sign this teachers as busy then
                for (uint32_t i = 0; i < assignedTeachers.size(); i++)
                {
                  for (uint32_t j = 0; j < teachers.size(); j++)
                  {
                    if (assignedTeachers[i] == teachers[j].getName())
                    {
                      teachers[j].storeAvailability(unitRowIndex, unitColIndex, 0);
                    }
                  }
                }
                // Assign next state of scheduling
                department.setState(department::State::ScheduledSingleUnit);
                department.setError(department::Error::None);
                return true;
              }
            }
          }
        }
      }

      // 3rd schedule in the standard way
      for (uint32_t uRow = 0; uRow < departmentAvailability.size(); uRow++)
      {
        for (uint32_t uCol = 0; uCol < departmentAvailability[uRow].size(); uCol++)
        {
          // Verify whether actual unit is not full and not already has this department scheduled
          if ((!scheduledTimeplan_[uRow][uCol].isFull(noOfAssignedTeachers)) &&
              (!scheduledTimeplan_[uRow][uCol].hasThisDepartment(departmentName)) &&
              (department.isNotScheduledAtThisCol(uCol)) &&
              (scheduledTimeplan_[uRow][uCol].isSuitableYearDifference(departmentName)))
          {
            unitIsSuitableForTeachers = 0;
            for (uint32_t it = 0; it < noOfAssignedTeachers; it++)
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
              if ((unitRowIndex != std::numeric_limits<uint32_t>::max()) &&
                  (unitColIndex != std::numeric_limits<uint32_t>::max()))
              {
                // Sign this teachers as busy then
                for (uint32_t i = 0; i < assignedTeachers.size(); i++)
                {
                  for (uint32_t j = 0; j < teachers.size(); j++)
                  {
                    if (assignedTeachers[i] == teachers[j].getName())
                    {
                      teachers[j].storeAvailability(unitRowIndex, unitColIndex, 0);
                    }
                  }
                }
                // Assign next state of scheduling 
                department.setState(department::State::ScheduledSingleUnit);
                department.setError(department::Error::None);
                return true;
              }
            }
          }
        }
      }
      department.setState(department::State::SchedulingImpossible);
      department.setError(department::Error::ScheduleSingleUnit);
      return false;
    }
  }

  return false;
}

uint32_t School::scheduleTimeTable()
{
  // Copy local copies of input data as read on
  std::vector<teacher> teachers = teachers_;
  std::vector<department> departments = departments_;
  std::vector<TeacherAssigner> assignments = assignments_;

  uint32_t scheduledDepartments = 0;
  uint32_t failedScheduled = 0;

  logger_.appendLog(Logging::LogLevel::Info,
                    Logging::LogMode::Enabled,
                    (std::string)"LOG.12: School.cpp School::scheduleTimeTable()");

  // Main algorithm loop
  // For first test run loop will be disabled and only one run will be performed
  while (countNotScheduledDepartments(departments) > 0)
  {
    // 1. Find a department with the lowest availability
    uint32_t indexOfDepartmentToSchedule = findLowestAvailableDepartment(departments,
                                                                     teachers,
                                                                     assignments,
                                                                     scheduledTimeplan_);

    std::stringstream logMessage;
  
    if (indexOfDepartmentToSchedule != std::numeric_limits<uint32_t>::max())
    {
      logMessage << "Step 1: Found department to schedule: " << 
        indexOfDepartmentToSchedule << std::endl;

      // 2. Find a unit suitable for this class and schedule unit
      uint32_t rowOfSuitableUnit = std::numeric_limits<uint32_t>::max();
      uint32_t colOfSuitableUnit = std::numeric_limits<uint32_t>::max();
      uint32_t assignmentIndex = std::numeric_limits<uint32_t>::max();
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
        // UI error logging
        schedulingFaults << "Błąd planowania klasy: " << 
          departments[indexOfDepartmentToSchedule].getName() << " : " <<
            departments[indexOfDepartmentToSchedule].errorToString(
              departments[indexOfDepartmentToSchedule].getError()) << std::endl;
      }

    }
    else
    {
      // Debug logging
      logMessage << "Step 1: FAILED: Department wasn't found: " <<
        indexOfDepartmentToSchedule << std::endl;
      departments[indexOfDepartmentToSchedule].setState(department::State::SchedulingImpossible);
      failedScheduled++;

      // UI logging
      schedulingFaults << "Nie zostala znaleziona klasa do rozplanowania: " << std::endl;
    }

    logger_.appendLog(Logging::LogLevel::Info,
                      Logging::LogMode::Enabled,
                      (std::string)"LOG.12: School.cpp School::scheduleTimeTable()" +
                      logMessage.str());

  }

  counters_.scheduledDepartments = scheduledDepartments;
  counters_.schedulingFails = failedScheduled;

  // Log summary report
  std::stringstream logMessage;
  logMessage << std::endl << "------------------ Summary report: ------------------" << std::endl <<
    "Read teachers: " << counters_.teachers << std::endl <<
      "Read departments: " << counters_.departments << std::endl <<
        "Read assignments: " << counters_.assignments << std::endl <<
          "Scheduled departments: " << counters_.scheduledDepartments << std::endl <<
            "Failed scheduling: " << counters_.schedulingFails;

  logger_.appendLog(Logging::LogLevel::Info,
                    Logging::LogMode::Enabled,
                    (std::string)"LOG.13: School.cpp School::scheduleTimeTable()" +
                    logMessage.str());

  return counters_.scheduledDepartments;
}

void School::formatTimePlan(OpenXLSX::XLDocument& timePlanFile,
                            const char startLetter,
                            const int startRow,
                            std::string headerText)
{
  OpenXLSX::XLWorksheet wks = timePlanFile.workbook().worksheet(1);

  auto& fonts   = timePlanFile.styles().fonts();
  auto& fills   = timePlanFile.styles().fills();
  auto& borders = timePlanFile.styles().borders();
  auto& formats = timePlanFile.styles().cellFormats();

  // Colors setting
  OpenXLSX::XLColor greenDark("FF7FBF5F");   // title bar
  OpenXLSX::XLColor greenLight("FFA9D18E");  // header
  OpenXLSX::XLColor black("FF000000");

  // Font for title
  OpenXLSX::XLStyleIndex fontTitle = fonts.create();
  fonts[fontTitle].setBold();
  fonts[fontTitle].setFontSize(20); 

  // Font settings
  OpenXLSX::XLStyleIndex fontBold = fonts.create();
  fonts[fontBold].setBold();

  // Borders setting
  OpenXLSX::XLStyleIndex borderThin = borders.create();
  borders[borderThin].setOutline(true);
  borders[borderThin].setLeft  (OpenXLSX::XLLineStyleThin, black);
  borders[borderThin].setRight (OpenXLSX::XLLineStyleThin, black);
  borders[borderThin].setTop   (OpenXLSX::XLLineStyleThin, black);
  borders[borderThin].setBottom(OpenXLSX::XLLineStyleThin, black);

  // Format title
  OpenXLSX::XLStyleIndex titleFill = fills.create();
  fills[titleFill].setPatternType(OpenXLSX::XLPatternSolid);
  fills[titleFill].setColor(greenDark);

  OpenXLSX::XLStyleIndex titleFormat = formats.create();
  formats[titleFormat].setFontIndex(fontTitle);
  formats[titleFormat].setFillIndex(titleFill);
  formats[titleFormat].setBorderIndex(borderThin);
  formats[titleFormat].alignment(OpenXLSX::XLCreateIfMissing).setHorizontal(OpenXLSX::XLAlignCenter);
  formats[titleFormat].alignment(OpenXLSX::XLCreateIfMissing).setVertical(OpenXLSX::XLAlignCenter);

  // Format header
  OpenXLSX::XLStyleIndex headerFill = fills.create();
  fills[headerFill].setPatternType(OpenXLSX::XLPatternSolid);
  fills[headerFill].setColor(greenLight);

  OpenXLSX::XLStyleIndex headerFormat = formats.create();
  formats[headerFormat].setFontIndex(fontBold);
  formats[headerFormat].setFillIndex(headerFill);
  formats[headerFormat].setBorderIndex(borderThin);
  formats[headerFormat].alignment(OpenXLSX::XLCreateIfMissing).setHorizontal(OpenXLSX::XLAlignCenter);
  formats[headerFormat].alignment(OpenXLSX::XLCreateIfMissing).setVertical(OpenXLSX::XLAlignCenter);
  formats[headerFormat].alignment(OpenXLSX::XLCreateIfMissing).setWrapText(true);

  // Format cells
  OpenXLSX::XLStyleIndex cellFormat = formats.create();
  formats[cellFormat].setBorderIndex(borderThin);
  formats[cellFormat].alignment(OpenXLSX::XLCreateIfMissing).setWrapText(true);
  formats[cellFormat].alignment(OpenXLSX::XLCreateIfMissing).setVertical(OpenXLSX::XLAlignCenter);

  // Title
  std::string startCell = startLetter + std::to_string(startRow);
  std::string cellsToMerge = startCell + ":" +
    static_cast<char>(startLetter + programConfig::maxNoOfAvailableDays) + std::to_string(startRow);
  wks.cell(startCell) = headerText;
  wks.mergeCells(cellsToMerge);
  wks.cell(startCell).setCellFormat(titleFormat);
  wks.row(startRow).setHeight(35);

  // Days headers
  std::array<std::string, programConfig::maxNoOfAvailableDays + 1> headers = {
      "", "Poniedziałek", "Wtorek", "Środa", "Czwartek", "Piątek"
  };

  for (int col = 1; col <= headers.size(); col++) {
      wks.cell(startRow + 1, col) = headers[col - 1];
      wks.cell(startRow + 1, col).setCellFormat(headerFormat);
  }
  wks.row(startRow + 1).setHeight(30);

  // Lessons numbers
  int startRowOffset = startRow + 1;
  for (int i = 1; i <= programConfig::maxNoOfAvailableUnits; i++) {
      wks.cell(startRowOffset + i, 1) = i;
      wks.cell(startRowOffset + i, 1).setCellFormat(headerFormat);
      wks.row(startRowOffset + i).setHeight(50);
  }

  // Format timetable area
  std::string planRangeCells = static_cast<char>(startLetter + 1) +
    std::to_string(startRow + 2) + ":" + static_cast<char>(startLetter  +
      programConfig::maxNoOfAvailableDays) + std::to_string(startRow + 1 + 
        programConfig::maxNoOfAvailableUnits);

  OpenXLSX::XLCellRange planRange = wks.range(planRangeCells);
  planRange.setFormat(cellFormat);

  // Columns size formatting
  wks.column(std::string(1, startLetter)).setWidth(5);
  wks.column(std::string(1, startLetter + 1)).setWidth(18);
  wks.column(std::string(1, startLetter + 2)).setWidth(18);
  wks.column(std::string(1, startLetter + 3)).setWidth(18);
  wks.column(std::string(1, startLetter + 4)).setWidth(18);
  wks.column(std::string(1, startLetter + 5)).setWidth(18);

}

int School::writeScheduledTimePlan()
{

  // Get worksheet from doc
  timePlanWks_ = timePlanFile_.workbook().worksheet("Sheet1");

  formatTimePlan(timePlanFile_, 'A', 1, "Plan lekcji");

  writeSchedulingInfo(timePlanFile_);

  constexpr char startLetter = 'B';
  constexpr int startRow = 3;

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

int School::writeSchedulingInfo(OpenXLSX::XLDocument& timePlanFile)
{
  OpenXLSX::XLWorksheet wks = timePlanFile.workbook().worksheet(1);

  auto& fonts   = timePlanFile.styles().fonts();
  auto& fills   = timePlanFile.styles().fills();
  auto& borders = timePlanFile.styles().borders();
  auto& formats = timePlanFile.styles().cellFormats();

  // Colors setting
  OpenXLSX::XLColor greenDark("FF7FBF5F");   // title bar
  OpenXLSX::XLColor greenLight("FFA9D18E");  // header
  OpenXLSX::XLColor black("FF000000");

  // Font for title
  OpenXLSX::XLStyleIndex fontTitle = fonts.create();
  fonts[fontTitle].setBold();
  fonts[fontTitle].setFontSize(20); 

  // Font settings
  OpenXLSX::XLStyleIndex fontBold = fonts.create();
  fonts[fontBold].setBold();

  // Borders setting
  OpenXLSX::XLStyleIndex borderThin = borders.create();
  borders[borderThin].setOutline(true);
  borders[borderThin].setLeft  (OpenXLSX::XLLineStyleThin, black);
  borders[borderThin].setRight (OpenXLSX::XLLineStyleThin, black);
  borders[borderThin].setTop   (OpenXLSX::XLLineStyleThin, black);
  borders[borderThin].setBottom(OpenXLSX::XLLineStyleThin, black);

  // Format title
  OpenXLSX::XLStyleIndex titleFill = fills.create();
  fills[titleFill].setPatternType(OpenXLSX::XLPatternSolid);
  fills[titleFill].setColor(greenDark);

  OpenXLSX::XLStyleIndex titleFormat = formats.create();
  formats[titleFormat].setFontIndex(fontTitle);
  formats[titleFormat].setFillIndex(titleFill);
  formats[titleFormat].setBorderIndex(borderThin);
  formats[titleFormat].alignment(OpenXLSX::XLCreateIfMissing).setHorizontal(OpenXLSX::XLAlignCenter);
  formats[titleFormat].alignment(OpenXLSX::XLCreateIfMissing).setVertical(OpenXLSX::XLAlignCenter);

  // Format header
  OpenXLSX::XLStyleIndex headerFill = fills.create();
  fills[headerFill].setPatternType(OpenXLSX::XLPatternSolid);
  fills[headerFill].setColor(greenLight);

  OpenXLSX::XLStyleIndex headerFormat = formats.create();
  formats[headerFormat].setFontIndex(fontBold);
  formats[headerFormat].setFillIndex(headerFill);
  formats[headerFormat].setBorderIndex(borderThin);
  formats[headerFormat].alignment(OpenXLSX::XLCreateIfMissing).setHorizontal(OpenXLSX::XLAlignCenter);
  formats[headerFormat].alignment(OpenXLSX::XLCreateIfMissing).setVertical(OpenXLSX::XLAlignCenter);
  formats[headerFormat].alignment(OpenXLSX::XLCreateIfMissing).setWrapText(true);

  // Format cells
  OpenXLSX::XLStyleIndex cellFormat = formats.create();
  formats[cellFormat].setBorderIndex(borderThin);
  formats[cellFormat].alignment(OpenXLSX::XLCreateIfMissing).setWrapText(true);
  formats[cellFormat].alignment(OpenXLSX::XLCreateIfMissing).setVertical(OpenXLSX::XLAlignCenter);

  // Title
  std::string startCell = "H2";
  wks.cell(startCell) = "Podsumowanie planowania";
  wks.cell(startCell).setCellFormat(titleFormat);
  wks.column("H").setWidth(50);

  // Message cell 1
  std::string messageCell1Addr = "H3";
  std::stringstream messageCell1;
  messageCell1 << "Odczytana z pliku ilość nauczycieli: " << counters_.teachers << std::endl <<
    "Odczytana z pliku ilość klas: " << counters_.departments << std::endl;
  wks.cell(messageCell1Addr) = messageCell1.str();

  // Message cell 2
  std::string messageCell2Addr = "H4";
  std::stringstream messageCell2;
  messageCell2 << "Odczytana z pliku ilość przypisań klas do nauczycieli: " << counters_.assignments << std::endl <<
      "Ilość rozplanowanych klas: " << counters_.scheduledDepartments << std::endl;
  wks.cell(messageCell2Addr) = messageCell2.str();

  // Fault cell 1
  std::string faultsCell1Addr = "H5";
  if (counters_.schedulingFails > 0)
  {
    wks.cell(faultsCell1Addr) = schedulingFaults.str();
  }

  return 1;
}

int School::writeScheduledTeacherPlan()
{
  constexpr int teacherPlanOffset = 14;

  // Get worksheet from doc
  teacherPlanWks_ = teacherPlanFile_.workbook().worksheet("Sheet1");

  char startLetter = 'A';
  int startRow = 1;

  for (int i = 0; i < teachers_.size(); i++)
  {
    std::string teacherName = teachers_[i].getName();
    // Prepare table for data
    formatTimePlan(teacherPlanFile_, startLetter, startRow, teacherName);

    for (std::size_t row = 0; row < scheduledTimeplan_.size(); row++)
    {
      for (std::size_t col = 0; col < scheduledTimeplan_[row].size(); col++)
      {
        char letter = static_cast<char>(startLetter + 1 + col);
        int rowIndex = static_cast<int>(startRow + 2 + row);
        std::string cell = std::string(1, letter) + std::to_string(rowIndex);
        if (scheduledTimeplan_[row][col].hasThisTeacher(teacherName))
        {
          teacherPlanWks_.cell(cell).value() = 
            scheduledTimeplan_[row][col].getDepartmentWithAssignedTeacher(teacherName);
        }
        else
        {
          teacherPlanWks_.cell(cell).value() = "";
        }
      }
    }

    startRow += teacherPlanOffset;
  }

  
  // Save file
  teacherPlanFile_.save();
  return 1;
}

