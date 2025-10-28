#pragma once

#include <iostream>
#include <vector>
#include "department.h"
#include "teacher.h"

using namespace std;

class school
{

public:

  school();

  //Methods
  int readTeachersAvailability(OpenXLSX::XLWorksheet& wks);
  void showTeachersAvailability();
  //int readDepartmentAvailability(string inputFilePath);
  //void showDepartmentsAvailability();

private:
  avaiabilitySettings settings;
  vector<teacher>teachers;
  vector<department>departments;

};

