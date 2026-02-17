#pragma once
#include <iostream>
#include <string>
#include <logging.h>
#include <OpenXLSX.hpp>

/**
 * @brief Represents assignment between department and teachers
 * 
 */
struct Assignment
{
  std::string department; ///< name of department
  std::vector<std::string> assignedTeachers; ///< name of teachers assigned to the department
};

/**
 * @brief Class responsible for handling department - teachers assignment data
 *
 */
class TeacherAssigner
{
  private:

    Assignment assignment_; ///< department - teachers assignment

    /**
     * @brief Configuration used to read and verify assignments
     * 
     */
    struct Settings
    {
      uint32_t minDepartmentLength; ///< Minimal department length of initials
      uint32_t maxDepartmentLength; ///< Maximal department length of initials
      uint32_t minTeacherInitialsLength; ///< Minimal teacher length of initials
      uint32_t maxTeacherInitialsLength; ///< Maximal teacher length of initials
      std::string departmentColumn; ///< Column position of department name
      char asignedTeachersBeginCol; ///< Assigned teachers begin column
      char asignedTeachersEndCol; ///< Assigned teachers end column
      uint32_t maxNoAssignedTeachers; ///< Maximum number of assigned teachers
    };

    OpenXLSX::XLWorksheet& wks_; ///< Worksheet of xlsx file
    Logging& logger_; ///< Logging object
    uint32_t& rowPointer_; ///< Pointer to the currently processed row
    Settings settings_; ///< Configuration used to read and verify assignment

  public:

    /**
     * @brief Construct a new Teacher Assigner object
     * 
     * @param[in] wks Worksheet providing access to availability data stored in an XLSX file.
     * @param[in,out] initialsRowPointer Current row index used during matrix parsing.
     * @param[in,out] logger Logger instance for reporting processing details.
     */
    TeacherAssigner(OpenXLSX::XLWorksheet& wks,
                    uint32_t& rowPointer,
                    Logging& logger);

    /**
     * @brief Read assignment
     * 
     * @return true if the assignment was read successfully, false otherwise
     */
    bool readAssignment();

    /**
     * @brief Get the Assignment object
     * 
     * @return Assignment
     */
    Assignment getAssignment();

    /**
     * @brief Verify whether assignment is valid
     * 
     * @return true if assignment is valid, false otherwise
     */
    bool isAssignmentValid();

    /**
     * @brief Get the Assigned Department object
     * 
     * @return std::string assignment department name
     */
    std::string getAssignedDepartment();

    /**
     * @brief Get the Assigned Teachers object
     * 
     * @return const std::vector<std::string>& assigned teachers
     */
    const std::vector<std::string>& getAssignedTeachers() const;
};