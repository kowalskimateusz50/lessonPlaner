#pragma once

#include <iostream>
#include <vector>
#include "department.h"
#include "teacher.h"
#include <OpenXLSX.hpp>
#include "logging.h"

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

  private:

    Logging& logger_;
    OpenXLSX::XLWorksheet& wks_;

    vector<teacher>teachers_;
    vector<department>departments_;

};

