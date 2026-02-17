#pragma once

#include <iostream>
#include <string>
#include <OpenXLSX.hpp>
#include "availability.h"
#include "programsettings.h"

/**
 * @brief Class responsible for handling department data
 *
 * This class extends the Availability base class by adding department-specific data processing 
 * and providing access to the availability data of the department object.
 *
 * Responsibilities:
 * - Orchestrates the department availability data
 * - Handling of department scheduling states
 * - Handling of department scheduling errors
 * - Provide data needed for scheduling process
 */
class department : public Availability
{
  public:

    /**
     * @brief Enum class to represent department scheduling states
     * 
     */
    enum class State
    {
      Idle, ///< State assigned always after object creation.
      ScheduleCombinedUnits, ///< Department has prepared data needed for 1st stage of scheduling
      ScheduledCombinedUnits, ///< 1st stage of scheduling done, combined unit was sheduled
      ScheduleSingleUnit, ///< Department is prepared for 2nd stage of scheduling
      ScheduledSingleUnit, ///< 2nd stage of scheduling done, single unit was scheduled
      Scheduled, ///< Scheduling process was successfully finished
      NotProperlyAssigned, ///< Scheduling error, department assignment is not available
      SchedulingImpossible ///< Scheduling error, not possible to schedule department
    };

    /**
     * @brief Enum class to represent errors displayed in user raport
     * 
     */
    enum class Error
    {
      None, ///< Error not present
      ScheduleCombinedUnits, ///< Not able to schedule combined units
      ScheduleSingleUnit, ///< Not able to schedule single units
      NotProperlyAssigned, ///< Not able to find department assignment
    };

    /**
     * @brief Construct a new department object
     *
     * @param[in] wks Worksheet providing access to availability data stored in an XLSX file.
     * @param[in,out] initialsRowPointer Current row index used during matrix parsing.
     * @param[in,out] logger Logger instance for reporting processing details.
     */
    department(OpenXLSX::XLWorksheet& wks,
               uint32_t& initialsRowPointer,
               Logging& logger);
    /**
     * @brief Set the State object
     * 
     * @param state
     */
    void setState(State state);
    /**
     * @brief Get the State object
     * 
     * @return State
     */
    State getState();
    /**
     * @brief Set the Error object
     * 
     * @param error 
     */
    void setError(Error error);
    /**
     * @brief Get the Error object
     * 
     * @return Error 
     */
    Error getError();
    /**
     * @brief Translate state to readable string 
     * 
     * @param[in] state
     * @return std::string containted exaplained current state needed for user raport
     */
    std::string stateToString(State state);
    /**
     * @brief Translate error to readable string
     * 
     * @param[in] error 
     * @return std::string contained explained error message needed for user raport
     */
    std::string errorToString(Error error);
    /**
     * @brief Set the Scheduled Col object
     *
     * @param col
     */
    void setScheduledCol(uint32_t col);
    /**
     * @brief Check whether department is already scheduled in column
     * 
     * @param[in] col column to be verified
     * @return true if department is already scheduled at this column, false if not
     */
    bool isNotScheduledAtThisCol(uint32_t col);

  private:

    State state_; ///< Current state of department scheduling process
    Error error_; ///< Current error of department scheduling process
    uint32_t scheduledCol_; ///< Column in which department has scheduled combined unit
};

