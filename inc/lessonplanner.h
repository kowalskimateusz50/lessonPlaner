#include <iostream>
#include "programsettings.h"
#include "school.h"
#include "logging.h"

/**
 * @brief Class responsible for controlling the program execution flow
 *
 */
class LessonPlanner
{
  public:

    /**
    * @brief Represents the result of the runtime process.
    *
    * This enumeration defines possible outcomes of the LessonPlanner::run()
    * operation. It allows distinguishing between different failure stages.
    */
    enum class RuntimeErrors
    {
      Success = 0,                  ///< Program executed successfully.
      ProgramSettingsLoad,          ///< Failed to load program settings
      TeachersAvailabilityLoad,     ///< Failed to load teachers availability data.
      DepartmentsAvailabilityLoad,  ///< Failed to load departments availability data.
      TeachersAssignmentLoad,       ///< Failed to load teachers assignment data.
      ScheduleTimePlanProcess       ///< Failed to schedule time plan.
    };
  
    /**
    * @brief Constructs a LessonPlanner instance.
    *
    * Initializes the planner with a reference to an existing School object.
    * The School instance must outlive the LessonPlanner.
    *
    * @param[in] schoolInstance Reference to the School object used by the planner.
    */
    LessonPlanner(School& schoolInstance) 
      : schoolInstance_(schoolInstance) {}

    /**
     * @brief Run program sequence
     *
     * @return RuntimeErrors program execution result
     */
    RuntimeErrors run();

  private:

    School& schoolInstance_; ///< School instance object
    /**
     * @brief Load data required for timeplan scheduling process
     *
     * @return RuntimeErrors result of data load
     */
    RuntimeErrors load();

    /**
     * @brief Show data required for timeplan scheduling process
     *
     * @return RuntimeErrors result of data show
     */
    RuntimeErrors show();

    /**
     * @brief Process data and schedule timeplan
     *
     * @return RuntimeErrors result of processing data
     */
    RuntimeErrors process();

    /**
     * @brief Save processed data
     *
     * @return RuntimeErrors result of saving data
     */
    RuntimeErrors save();
};