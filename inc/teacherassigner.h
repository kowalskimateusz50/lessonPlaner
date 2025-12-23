#pragma once
#include <iostream>
#include <string>
#include <logging.h>
#include <OpenXLSX.hpp>

struct Assignment
{
  std::string department;
  std::vector<std::string> assignedTeachers;
};

class TeacherAssigner
{
  private:
    Assignment assignment_;

    struct Settings 
    {
      uint minDepartmentLength;
      uint maxDepartmentLength;
      uint minTeacherInitialsLength;
      uint maxTeacherInitialsLength;
      std::string yearColumn;
      std::string departmentColumn;
      std::string assignedTeacherColumn1;
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
    Assignment getAssignment();
    bool isAssignmentValid();
    std::string getAssignedDepartment();
    const std::vector<std::string>& getAssignedTeachers() const;
};