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

/**
 * @brief Main class responsible for scheduling the school timetable.
 *
 * School orchestrates the entire scheduling process, from reading input data
 * to generating formatted XLSX output files.
 *
 * Responsibilities:
 * - Reading XLSX files and preparing input data
 * - Scheduling the timetable
 * - Creating and formatting XLSX output files
 * - Writing scheduled timetables and teacher plans
 */
class School
{

  public:

    School(ProgramSettings& programSettings, Logging& logger);
    ~School();
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
    int writeSchedulingInfo(OpenXLSX::XLDocument& timePlanFile);
    int writeScheduledTeacherPlan();

  private:
    struct Counters
    {
      uint32_t teachers = 0;
      uint32_t departments = 0;
      uint32_t assignments = 0;
      uint32_t scheduledDepartments = 0;
      uint32_t schedulingFails = 0;

      void reset() { *this = {}; }
    };

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

    Counters counters_;

    std::stringstream schedulingFaults;

};

