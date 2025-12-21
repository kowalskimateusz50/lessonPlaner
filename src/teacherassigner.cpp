#include "teacherassigner.h"

TeacherAssigner::TeacherAssigner(OpenXLSX::XLWorksheet& wks,
                                     uint& rowPointer,
                                     Logging& logger)
  : wks_(wks), rowPointer_(rowPointer), logger_(logger) // initialization list
{
  // Write settings
  settings_.minDepartmentLength = 2;
  settings_.maxDepartmentLength = 3;
  settings_.minTeacherInitialsLength = 2;
  settings_.maxTeacherInitialsLength = 2;
  settings_.yearColumn = "P";
  settings_.departmentColumn = "Q";
  settings_.asignedTeachersBeginCol = 'R';
  settings_.asignedTeachersEndCol = 'T';
  settings_.maxNoAssignedTeachers = 3;
}

bool TeacherAssigner::readAssignment()
{
  //Clear values
  assignment_.year = 0;
  assignment_.department = "";
  assignment_.assignedTeachers.clear();

  OpenXLSX::XLCellAssignable cell = wks_.findCell(settings_.yearColumn + std::to_string(rowPointer_));
  //Check whether there is no end of assignment table
  if (cell.empty())
  {
    return false;
  }

  assignment_.year = wks_.cell(settings_.yearColumn + std::to_string(rowPointer_)).value().get<uint>();
  assignment_.department = wks_.cell(settings_.departmentColumn + std::to_string(rowPointer_)).value().get<std::string>();
  //Read teachers in the loop
  for (char i = settings_.asignedTeachersBeginCol; i <= settings_.asignedTeachersEndCol; i++)
  {
    std::string rowLetter(1, i);
    std::string tempTeacherInitials = wks_.cell(
      rowLetter + std::to_string(rowPointer_)).value().get<std::string>();
    if ((tempTeacherInitials != "x"))
    {
      assignment_.assignedTeachers.push_back(tempTeacherInitials);
    }
  }
  rowPointer_++;
  return isAssignmentValid();
}
bool TeacherAssigner::isAssignmentValid()
{
  //Check department year and name
  if (!((assignment_.year > 0) && 
       (assignment_.department.size() >= settings_.minDepartmentLength) &&
       (assignment_.department.size() <= settings_.maxDepartmentLength) &&
       (assignment_.assignedTeachers.size() > 0) &&
       (assignment_.assignedTeachers.size() <= settings_.maxNoAssignedTeachers)))
  {
    return false;
  }

  return true;
}

Assignment TeacherAssigner::getAssignment()
{
  return assignment_;
}

uint TeacherAssigner::getAssignedYear()
{
  return assignment_.year;
}

std::string TeacherAssigner::getAssignedDepartment()
{
  return assignment_.department;
}

const std::vector<std::string>& TeacherAssigner::getAssignedTeachers() const
{
  return assignment_.assignedTeachers;
}
