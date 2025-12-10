#pragma once

#include <iostream>
#include <vector>
#include "department.h"
#include "teacher.h"
#include <OpenXLSX.hpp>
#include "logging.h"
#include "teacherassigner.h"

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

  private:

    Logging& logger_;
    OpenXLSX::XLWorksheet& wks_;

    std::vector<teacher> teachers_;
    std::vector<department> departments_;
    std::vector<TeacherAssigner> assignments_;

    std::vector<std::vector<Assignment>> scheduledPlan;

    uint teachersCounter;
    uint departmentsCounter;
    uint assignmentsCounter;
};

