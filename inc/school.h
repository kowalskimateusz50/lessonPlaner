#pragma once

#include <iostream>
#include <vector>
#include <array>
#include "department.h"
#include "teacher.h"
#include <OpenXLSX.hpp>
#include "logging.h"
#include "teacherassigner.h"
#include "scheduledunit.h"

class school
{

  public:

    school(OpenXLSX::XLWorksheet& wks, Logging& logger);
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

    Logging& logger_;
    OpenXLSX::XLWorksheet& wks_;

    std::vector<teacher> teachers_;
    std::vector<department> departments_;
    std::vector<TeacherAssigner> assignments_;

    std::array<std::array<ScheduledUnit, programConfig::maxNoOfAvailableDays>, 
      programConfig::maxNoOfAvailableUnits> scheduledTimeplan_;

    uint teachersCounter;
    uint departmentsCounter;
    uint assignmentsCounter;
};

