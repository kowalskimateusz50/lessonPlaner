#pragma once

#include <iostream>
#include <vector>
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
    int findSuitableUnit(std::vector<teacher>& teachers,
                         department department,
                         std::vector<TeacherAssigner>& assignments,
                         int unitRowIndex,
                         int unitColIndex);

  private:

    Logging& logger_;
    OpenXLSX::XLWorksheet& wks_;

    std::vector<teacher> teachers_;
    std::vector<department> departments_;
    std::vector<TeacherAssigner> assignments_;

    std::vector<std::vector<ScheduledUnit>> scheduledTimePlan_;

    uint teachersCounter;
    uint departmentsCounter;
    uint assignmentsCounter;
};

