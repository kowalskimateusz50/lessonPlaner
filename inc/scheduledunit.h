#include <iostream>
#include <vector>
#include <OpenXLSX.hpp>
#include "logging.h"
#include "teacherassigner.h"
#include "programsettings.h"

/**
 * @brief Class responsible for storing and handling data of single scheduled unit
 *
 * This class is used in scheduling process and providing methods to control state of unit.
 *
 */
class ScheduledUnit
{
  public:

    /**
     * @brief Construct a new Scheduled Unit object
     * 
     */
    ScheduledUnit();

    /**
     * @brief Check whether at least one assignment has been assigned to this unit.
     * 
     * @return true if at least one assignment has been assigned, false if no connections
     *         is assigned to this unit.
     */
    bool isScheduled();

    /**
     * @brief Check whether unit has space to schedule teachers.
     * 
     * @param[in] noOfAssignedTeachers number of teachers that needs to be scheduled
     *
     * @return true if unit has space to schedule declared number of teachers, false otherwise
     */
    bool isFull(std::size_t noOfAssignedTeachers);

    /**
     * @brief Check whether unit already has assigned declared teacher
     * 
     * @param[in] teacherName name of teacher to check
     *
     * @return true if unit already has a teacher assigned with given name, false otherwise
     */
    bool hasThisTeacher(std::string teacherName);

    /**
     * @brief Check whether unit already has assigned unit with this department
     *
     * @param[in] departmentName name of department to check
     *
     * @return true if unit already has a department assigned with given name, false otherwise
     */
    bool hasThisDepartment(std::string departmentName);

    /**
     * @brief Schedule assignment to this unit
     *
     * @param[in] assignment to schedule at this unit
     */
    void scheduleUnit(Assignment assignment);

    /**
     * @brief Get the whole scheduled unit object as a string
     * 
     * @return std::string contained whole scheduled unit
     */
    std::string getUnit();

    /**
     * @brief Get only the department with Assigned Teacher
     * 
     * @param[in] teacher to which department need to be assigned
     *
     * @return std::string name of assigned department to this teacher
     */
    std::string getDepartmentWithAssignedTeacher(std::string teacher);

    /**
     * @brief Get the year from department name 
     *
     * @param[in] name name of department
     *
     * @return uint32_t department year
     */
    uint32_t getYearFromDepartmentName(std::string name);

    /**
     * @brief Check whether is suitable year difference between departmend already shcheduled and 
     *        department to schedule
     * 
     * @param[in] departmentNameToSchedule name of department to schedule
     *
     * @return true if difference is less or equal than declared in program config, false otherwise
     */
    bool isSuitableYearDifference(std::string departmentNameToSchedule);

    /**
     * @brief Check whether unit has maximum allowed number of teachers
     * 
     * @param[in] noOfAssignedTeachers number of teachers to schedule
     *
     * @return true if unit will have maximum allowed with scheduled departmentNameToSchedule
     */
    bool isOptimalTeacherUsage(std::size_t noOfAssignedTeachers);

  private:

    bool isScheduled_; ///< flag indicates that at least one assignment has been assigned
    uint32_t noOfAssignedTeachers_; ///< counter for number of assigned teachers in unit
    std::vector<Assignment> assignments_; ///< scheduled assignments
};