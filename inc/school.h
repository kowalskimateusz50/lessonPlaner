#pragma once

#include <iostream>
#include <vector>
#include "department.h"
#include "teacher.h"
#include "logging.h"

using namespace OpenXLSX;
using namespace std;

class school
{

  public:

    school();
    int readTeachersAvailability(loggingToTextFile& logger, OpenXLSX::XLWorksheet& wks);
    void showTeachersAvailability(loggingToTextFile& logger);
    int readDepartmentsAvailability(loggingToTextFile& logger, OpenXLSX::XLWorksheet& wks);
    void showDepartmentsAvailability(loggingToTextFile& logger);

  private:
    avaiabilitySettings settings;
    vector<teacher>teachers;
    vector<department>departments;

};

