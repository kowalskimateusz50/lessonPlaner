#pragma once

#include <iostream>
#include <vector>
#include <array>
#include <OpenXLSX.hpp>
#include "programsettings.h"
#include "department.h"
#include "teacher.h"
#include "logging.h"
#include "teacherassigner.h"
#include "scheduledunit.h"

/**
 * @brief Main class responsible for scheduling the school timetable.
 *
 * School orchestrates the entire scheduling process, from reading input data
 * to generating formatted XLSX output files.
 *
 * Responsibilities:
 * - Reading XLSX files and preparing input data
 * - Scheduling the timetable
 * - Creating and formatting XLSX output files
 * - Writing scheduled timetables and teacher plans
 */
class School
{

  public:
    /**
     * @brief Construct a new School object
     *
     * @param[in] programSettings global program settings
     * @param[in, out] logger object for logging
     */
    School(ProgramSettings& programSettings, Logging& logger);

    /**
     * @brief Destroy the School object
     * 
     */
    ~School();

    /**
     * @brief Open xlsx input data file and read wks
     *
     */
    void prepareInputDataFile();

    /**
     * @brief Create and open output data files
     *
     */
    void prepareOutputDataFiles();

    /**
     * @brief read availability of teachers from xlsx input file
     *
     * @return int number of read teachers
     */
    int readTeachersAvailability();

    /**
     * @brief show teachers availability for logging
     *
     */
    void showTeachersAvailability();

    /**
     * @brief read department availability from xlsx input file
     * @return int number of read departments
     */
    int readDepartmentsAvailability();

    /**
     * @brief show departments availability
     * 
     */
    void showDepartmentsAvailability();

    /**
     * @brief read assignments between departments and teachers from xlsx input file
     * @return int number of read assignments
     */
    int readTeachersAssignment();

    /**
     * @brief show read assignments for logging
     */
    void showTeachersAssignment();

    /**
     * @brief count number of not scheduled departments, based on their states
     * @param[in] departments vector of departments
     * @return uint32_t number of not scheduled departments
     */
    uint32_t countNotScheduledDepartments(std::vector<department>& departments);

    /**
     * @brief Main timetable scheduling sequence
     * @return uint32_t number of scheduled departments
     */
    uint32_t scheduleTimeTable();

    /**
     * @brief Find index of least available department
     * 
     * @param departments Departments considered during selection
     * @param teachers Teachers used when calculating availability.
     * @param assignments Teachers - department assignment
     * @param scheduledTimeplan Current state of scheduled timeplan
     * @return uint32_t found index of department with lowest availability
     */
    uint32_t findLowestAvailableDepartment(std::vector<department>& departments,
                                           std::vector<teacher>& teachers,
                                           std::vector<TeacherAssigner>& assignments,
                                           std::array<std::array<ScheduledUnit,
                                           programConfig::maxNoOfAvailableDays>,
                                           programConfig::maxNoOfAvailableUnits>& scheduledTimeplan);

    /**
     * @brief Find suitable unit to schedule department with the lowest availablitiy
     * 
     * @param[in] teachers Teachers availability using to schedule suitable unit
     * @param[in] department Availability of selected to scheduling department
     * @param[in] assignments Teacher - department assignments
     * @param[out] unitRowIndex Found row index of selected unit in matrix
     * @param unitColIndex Found column index of selected unit in matrix
     * @param assignmentIndex Index of assignment related to selected deaprtment
     * @return true if found suitable unit to schedule department, false otherwise
     */
    bool findSuitableUnit(std::vector<teacher>& teachers,
                          department& department,
                          std::vector<TeacherAssigner>& assignments,
                          uint32_t& unitRowIndex,
                          uint32_t& unitColIndex,
                          uint32_t& assignmentIndex);
    /**
     * @brief Prepare formatted xlsx document for saving scheduled timeplan
     * 
     * @param[in] timePlanFile Scheduled timeplan begin columnswdwadwdard wddaw 
     * @param[in] startLetter Start column of timeplan table
     * @param[in] startRow Start row number of timeplan table
     * @param[in] headerText Header of timeplan table
     */
    void formatTimePlan(OpenXLSX::XLDocument& timePlanFile,
                        const char startLetter,
                        const int startRow,
                        std::string headerText);
    /**
     * @brief Write scheduled timeplan to xlsx file
     * 
     * @return int 1 if timeplan was save, 0 otherwise
     */
    int writeScheduledTimePlan();

    /**
     * @brief Write informations about scheduling process to xlsx file
     * 
     * @param[in] timePlanFile xlsx document
     * @return int 1 if informations was write, 0 otherwise
     */
    int writeSchedulingInfo(OpenXLSX::XLDocument& timePlanFile);

    /**
     * @brief Write scheduled timeplans for teachers
     *
     * @return int 1 if scheduled teachers plan was write successfully, 0 otherwise
     */
    int writeScheduledTeacherPlan();

  private:

    /**
    * @brief Aggregates scheduling statistics collected during timetable generation.
    *
    * This structure stores counters used to track progress,
    * successful assignments, and scheduling failures.
    */
    struct Counters
    {
      uint32_t teachers = 0;              ///< Total number of teachers
      uint32_t departments = 0;           ///< Total number of departments
      uint32_t assignments = 0;           ///< Total number of teacher–department assignments
      uint32_t scheduledDepartments = 0;  ///< Number of successfully scheduled departments
      uint32_t schedulingFails = 0;       ///< Number of scheduling failures

      void reset() { *this = {}; }
    };

    ProgramSettings& programSettings_; ///< Program settings
    Logging& logger_; ///< Logging object
    OpenXLSX::XLDocument inputFile_; ///< Input xlsx file
    OpenXLSX::XLWorksheet inputFileWks_; ///< Input file wks
    OpenXLSX::XLDocument timePlanFile_; ///< Timeplan xlsx output file
    OpenXLSX::XLWorksheet timePlanWks_; ///< Timeplan output wks
    OpenXLSX::XLDocument teacherPlanFile_; ///< Teachers timeplan xlsx output file
    OpenXLSX::XLWorksheet teacherPlanWks_; ///< Teachers output wks

    std::vector<teacher> teachers_; ///< Read teachers
    std::vector<department> departments_; ///< Read departments
    std::vector<TeacherAssigner> assignments_; ///< Read assignments

    std::array<std::array<ScheduledUnit, programConfig::maxNoOfAvailableDays>,
      programConfig::maxNoOfAvailableUnits> scheduledTimeplan_; ///< Scheduled timeplan

    Counters counters_; ///< Aggregated scheduling statistics used for raporting

    std::stringstream schedulingFaults; ///< Faults appeard during scheduling

};