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
    void prepareOutputDataFiles();
    int readTeachersAvailability();
    void showTeachersAvailability();
    int readDepartmentsAvailability();
    void showDepartmentsAvailability();
    int readTeachersAssignment();
    void showTeachersAssignment();
    uint32_t countNotScheduledDepartments(std::vector<department>& departments);
    uint32_t scheduleTimeTable();
    uint32_t findLowestAvailableDepartment(std::vector<department>& departments,
                                       std::vector<teacher>& teachers,
                                       std::vector<TeacherAssigner>& assignments,
                                       std::array<std::array<ScheduledUnit,
                                       programConfig::maxNoOfAvailableDays>,
                                       programConfig::maxNoOfAvailableUnits>& scheduledTimeplan);
    bool findSuitableUnit(std::vector<teacher>& teachers,
                          department& department,
                          std::vector<TeacherAssigner>& assignments,
                          uint32_t& unitRowIndex,
                          uint32_t& unitColIndex,
                          uint32_t& assignmentIndex);
    void formatTimePlan(OpenXLSX::XLDocument& timePlanFile,
                        const char startLetter,
                        const int startRow,
                        std::string headerText);
    int writeScheduledTimePlan();
    int writeScheduledTeacherPlan();

  private:
    ProgramSettings& programSettings_;
    Logging& logger_;
    OpenXLSX::XLDocument inputFile_;
    OpenXLSX::XLWorksheet inputFileWks_;
    OpenXLSX::XLDocument timePlanFile_;
    OpenXLSX::XLWorksheet timePlanWks_;
    OpenXLSX::XLDocument teacherPlanFile_;
    OpenXLSX::XLWorksheet teacherPlanWks_;

    std::vector<teacher> teachers_;
    std::vector<department> departments_;
    std::vector<TeacherAssigner> assignments_;

    std::array<std::array<ScheduledUnit, programConfig::maxNoOfAvailableDays>, 
      programConfig::maxNoOfAvailableUnits> scheduledTimeplan_;

    uint32_t teachersCounter;
    uint32_t departmentsCounter;
    uint32_t assignmentsCounter;
    uint32_t scheduledDepartmentsCounter;
};

