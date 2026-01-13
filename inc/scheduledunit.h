#include <iostream>
#include <vector>
#include <OpenXLSX.hpp>
#include "logging.h"
#include "teacherassigner.h"
#include "programsettings.h"

class ScheduledUnit
{
  public:
    ScheduledUnit();
    bool isScheduled();
    bool isFull(std::size_t noOfAssignedTeachers);
    bool hasThisTeacher(std::string teacherName);
    bool hasThisDepartment(std::string departmentName);    
    void scheduleUnit(Assignment assignment);
    std::string getUnit();
    std::string getUnitWithAssignedTeacher(std::string teacher);
    uint32_t getYearFromDepartmentName(std::string name);
    bool isSuitableYearDifference(std::string departmentNameToSchedule);
    bool isOptimalTeacherUsage(std::size_t noOfAssignedTeachers);

  private:
    bool isScheduled_;
    uint32_t noOfAssignedTeachers_;
    std::vector<Assignment> assignments_;
};