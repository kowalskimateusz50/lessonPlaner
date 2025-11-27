#pragma once

#include <iostream>
#include <vector>
#include "department.h"
#include "teacher.h"
#include <OpenXLSX.hpp>
#include "logging.h"
#include "teacherassigner.h"

using namespace OpenXLSX;
using namespace std;

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

  private:

    Logging& logger_;
    OpenXLSX::XLWorksheet& wks_;

    vector<teacher> teachers_;
    vector<department> departments_;
    vector<TeacherAssigner> assignments_;

    uint teachersCounter;
    uint departmentsCounter;
    uint assignmentsCounter;
};

