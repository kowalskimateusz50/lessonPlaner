#pragma once

#include <iostream>
#include <vector>
#include "department.h"
#include "teacher.h"

using namespace OpenXLSX;
using namespace std;

class school
{

  public:

    school();
    int readTeachersAvailability(OpenXLSX::XLWorksheet& wks);
    void showTeachersAvailability();
    int readDepartmentsAvailability(OpenXLSX::XLWorksheet& wks);
    void showDepartmentsAvailability();

  private:
    avaiabilitySettings settings;
    vector<teacher>teachers;
    vector<department>departments;

};

