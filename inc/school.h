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

    school(ProgramSettings& programSettings, Logging& logger);
    ~school();
    void prepareInputDataFile();
    void prepareOutputDataFile(); 
    int readTeachersAvailability();
    void showTeachersAvailability();
    int readDepartmentsAvailability();
    void showDepartmentsAvailability();
    int readTeachersAssignment();
    void showTeachersAssignment();
    bool scheduleTimeTable();
    int findLowestAvailableDepartment(std::vector<department>& departments);
    bool findSuitableUnit(std::vector<teacher>& teachers,
                          department department,
                          std::vector<TeacherAssigner>& assignments,
                          int& unitRowIndex,
                          int& unitColIndex,
                          int& assignmentIndex);
    int writeScheduledTimeplan();

  private:
    ProgramSettings& programSettings_;
    Logging& logger_;
    OpenXLSX::XLDocument inputFile_;
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

