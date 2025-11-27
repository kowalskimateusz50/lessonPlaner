#pragma once
#include <iostream>
#include <string>
#include <logging.h>
#include <OpenXLSX.hpp>

using namespace std;
using namespace OpenXLSX;

class TeacherAssigner
{
  private:

    uint year_;
    string department_;
    vector<string> assignedTeachers_;

    struct Settings 
    {
      uint minDepartmentLength;
      uint maxDepartmentLength;
      uint minTeacherInitialsLength;
      uint maxTeacherInitialsLength;
      string yearColumn;
      string departmentColumn;
      string assignedTeacherColumn1;
      char asignedTeachersBeginCol;
      char asignedTeachersEndCol;
      uint maxNoAssignedTeachers;
    };

    OpenXLSX::XLWorksheet& wks_;
    Logging& logger_;
    uint& rowPointer_;
    Settings settings_; 

  public:
   
    TeacherAssigner(OpenXLSX::XLWorksheet& wks,
                                     uint& rowPointer,
                                     Logging& logger);
    bool readAssignment();
    void showAssignment();
    bool isAssignmentValid();
    uint getAssignedYear();
    string getAssignedDepartment();
    const vector<string>& getAssignedTeachers() const;
};