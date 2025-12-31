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

  private:
    bool isScheduled_;
    uint noOfAssignedTeachers_;
    std::vector<Assignment> assignments_;
};