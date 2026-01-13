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
      uint32_t minDepartmentLength;
      uint32_t maxDepartmentLength;
      uint32_t minTeacherInitialsLength;
      uint32_t maxTeacherInitialsLength;
      std::string yearColumn;
      std::string departmentColumn;
      std::string assignedTeacherColumn1;
      char asignedTeachersBeginCol;
      char asignedTeachersEndCol;
      uint32_t maxNoAssignedTeachers;
    };

    OpenXLSX::XLWorksheet& wks_;
    Logging& logger_;
    uint32_t& rowPointer_;
    Settings settings_;

  public:

    TeacherAssigner(OpenXLSX::XLWorksheet& wks,
                    uint32_t& rowPointer,
                    Logging& logger);
    bool readAssignment();
    Assignment getAssignment();
    bool isAssignmentValid();
    std::string getAssignedDepartment();
    const std::vector<std::string>& getAssignedTeachers() const;
};