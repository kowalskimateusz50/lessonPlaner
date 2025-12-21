#pragma once

#include <iostream>
#include <vector>
#include <array>
#include <OpenXLSX.hpp>
#include "programsettings.h"
#include "department.h"
#include "teacher.h"
#include "logging.h"
#include "teacherassigner.h"
#include "scheduledunit.h"

class school
{

  public:

    school(ProgramSettings& settings, Logging& logger);
    ~school();
    prepareInputDataFile();
    prepareOutputDataFile(); 
    int readTeachersAvailability();
    void showTeachersAvailability();
    int readDepartmentsAvailability();
    void showDepartmentsAvailability();
    int readTeachersAssignment();
    void showTeachersAssignment();
    bool scheduleTimeTable();
    int findLowestAvailableDepartment(std::vector<department>& departments);
    template <typename T, std::size_t N>
    bool findSuitableUnit(std::vector<teacher>& teachers,
                          department department,
                          std::vector<TeacherAssigner>& assignments,
                          std::array<T, N>& scheduledTimeplan,
                          int& unitRowIndex,
                          int& unitColIndex,
                          int& assignmentIndex);
  private:
    ProgramSettings& progamSettings_;
    Logging& logger_;
    OpenXLSX::XLDocument inputFile;
    OpenXLSX::XLWorksheet inputFileWks_;
    OpenXLSX::XLDocument outputFile_;    
    OpenXLSX::XLWorksheet outputFileWks_;

    std::vector<teacher> teachers_;
    std::vector<department> departments_;
    std::vector<TeacherAssigner> assignments_;

    std::array<std::array<ScheduledUnit, programConfig::maxNoOfAvailableDays>, 
      programConfig::maxNoOfAvailableUnits> scheduledTimeplan_;

    uint teachersCounter;
    uint departmentsCounter;
    uint assignmentsCounter;
};

