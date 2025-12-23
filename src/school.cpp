#include "school.h"

namespace fs = std::filesystem;

school::school(ProgramSettings& programSettings, Logging& logger)
  : programSettings_(programSettings), logger_(logger) // initialization list
{
  // Open input data file
  prepareInputDataFile();

  // Open output data file
  prepareOutputDataFile();

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
  outputFile_.close();

}

void school::prepareInputDataFile()
{
  //Preparing input file
  inputFile_.open(programSettings_.getInputFilePath());
  //Open input file worksheet
  inputFileWks_ = inputFile_.workbook().worksheet(1);
}

void school::prepareOutputDataFile()
{
  std::string filePath = programSettings_.getOutputFilePath();

  fs::path filePathObj(filePath);

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
        outputFile_.create(filePath, true);   // Create new wks
        outputFile_.open(filePath);     // Open new wks
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

  while (true)
  {
    auto departmentPtr =
        std::make_unique<department>(inputFileWks_, rowPointer, logger_);

      if (!departmentPtr->readAvailability())
          break;

      departments_.push_back(std::move(departmentPtr));

    departmentsCounter++;
    logMessage << "\nFound department No.: " << departmentsCounter;
  }

  logMessage << "\nStopped searching departments: " << departmentsCounter;

  logger_.appendLog(
    M_INFO,
    M_LOG_ENABLED,
    "LOG.8: school.cpp school::readDepartmentsAvailability()" + logMessage.str()
);

return departmentsCounter;
}

/**
 * @brief funtion to display departments availability
 * 
 */
void school::showDepartmentsAvailability()
{
  for (std::size_t i = 0; i < departments_.size(); i++)
  {
    departments_[i]->showAvailability();
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

int school::findLowestAvailableDepartment(
  const std::vector<std::unique_ptr<department>>& departments)
{
  int lowestAvailabilityUnits = departments[0]->countAvailabilityUnits();
  int lowestAvailabityIndex = 0;

  for (int i = 1; i < departments.size(); i++)
  {
    int countAvailabilityUnits = departments[i]->countAvailabilityUnits();
    if (countAvailabilityUnits < lowestAvailabilityUnits)
    {
      lowestAvailabilityUnits = departments[i]->countAvailabilityUnits();
      lowestAvailabityIndex = i;
    }
  }
  return lowestAvailabityIndex;
}
bool school::findSuitableUnit(std::vector<teacher>& teachers,
              department& department,
              std::vector<TeacherAssigner>& assignments,
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
    if (departmentName == assignments[i].getAssignedDepartment())
    {
      assignedTeachers = assignments[i].getAssignedTeachers();
      assignmentIndex = i;
      break;
    }
  }

  // 2. Prepare department availability and teachers availability
  std::vector<std::vector<int>> departmentAvailability = department.getAvailabilityVector();
  std::vector<std::vector<std::vector<int>>> teachersAvailability;
  std::vector<std::string> teachersNames;  

  for (int i = 0; i < assignedTeachers.size(); i++)
  {
      for (int j = 0; j < teachers.size(); j++)
      {
        if (assignedTeachers[i] == teachers[j].getName())
        {
          teachersAvailability.emplace_back(teachers[j].getAvailabilityVector());
          teachersNames.emplace_back(teachers[j].getName());
        }
      }
  }

  uint unitIsSuitableForTeachers = 0;

  // 3. Select 1st unit suitable for department and for teachers
  for (uint uRow = 0; uRow < departmentAvailability.size(); uRow++)
  {
    for (uint uCol = 0; uCol < departmentAvailability[uRow].size(); uCol++)
    {
      if (!scheduledTimeplan_[uRow][uCol].isFull())
      {
        unitIsSuitableForTeachers = 0;
        for (int it = 0; it < teachersAvailability.size(); it++)
        {
          if ((departmentAvailability[uRow][uCol] == 1) && 
              ((teachersAvailability[it][uRow][uCol] == 1) || 
               (scheduledTimeplan_[uRow][uCol].hasThisTeacher(teachersNames[it]))))
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
  //std::vector<std::unique_ptr<department>> departments = departments_;
  std::vector<TeacherAssigner> assignments = assignments_;

  bool foundPossibleTimeTable = false;

  logger_.appendLog(M_INFO,
                    M_LOG_ENABLED,
                    (std::string)"LOG.12: school.cpp school::scheduleTimeTable()");

  // Main algorithm loop
  // For first test run loop will be disabled and only one run will be performed
  while (departments_.size() > 0)
  {
    // 1. Find a department with the lowest availability
    int indexOfDepartmentToSchedule = findLowestAvailableDepartment(departments_);

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
                           *departments_[indexOfDepartmentToSchedule],
                           assignments,
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
      logMessage << "Step 1: FAILED: Department to schedule wasn't found: ";
    }

    logger_.appendLog(M_INFO,
                      M_LOG_ENABLED,
                      (std::string)"LOG.12: school.cpp school::scheduleTimeTable()" +
                      logMessage.str());

    departments_.erase(departments_.begin() + static_cast<std::size_t>(indexOfDepartmentToSchedule));

  }

  return foundPossibleTimeTable;
}

int school::writeScheduledTimeplan()
{

  // Write example data to worksheet
  outputFileWks_ = outputFile_.workbook().worksheet("Sheet1");
  
  constexpr char startLetter = 'A';
  constexpr int startRow = 1;

  for (std::size_t row = 0; row < scheduledTimeplan_.size(); row++)
  {
    for (std::size_t col = 0; col < scheduledTimeplan_[row].size(); col++)
    {
      char letter = static_cast<char>(startLetter + col);
      int rowIndex = static_cast<int>(startRow + row);
      std::string cell = std::string(1, letter) + std::to_string(rowIndex);
      if (scheduledTimeplan_[row][col].isScheduled())
      {
        outputFileWks_.cell(cell).value() = scheduledTimeplan_[row][col].getUnit();
      }
      else
      {
        outputFileWks_.cell(cell).value() = "";
      }
    }
  }

  // Save file
  outputFile_.save();
  return 1;
}