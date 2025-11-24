#pragma once
#include <iostream>
#include <string>
#include <logging.h>
#include <OpenXLSX.hpp>

using namespace std;
using namespace OpenXLSX;

class TeacherAssigner
{
  private
    uint year;
    string department;
    vector<string> assignedTeachers;

    OpenXLSX::XLWorksheet& wks_;
    Logging& logger_;
    uint& initialsRowPointer_;

  public:
    int readAssignment();
    void showAssignment();

}