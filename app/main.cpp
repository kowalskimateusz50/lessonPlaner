
#include <iostream>
#include "programsettings.h"
#include "school.h"
#include "logging.h"

int main() 
{
  //Getting program settings
  ProgramSettings progamSettings;
  progamSettings.readProgramSettings();

  //Creating logging class instance and log settings
  Logging logger(progamSettings.isLogModeOn(),
                 progamSettings.isLogToConsoleOn(),
                 progamSettings.isLogToFileOn(),
                 progamSettings.getLogFilePath());

  logger.appendLog(M_INFO,
                   M_LOG_ENABLED,
                   (std::string)"LOG.1: main.cpp progamSettings.readProgramSettings()" +
                   " logFilePath=" + progamSettings.getLogFilePath() +
                   " isLogModeOn=" + to_string(progamSettings.isLogModeOn()) +
                   " logFilePath=" + progamSettings.getLogFilePath());

  //School instance creation
  School SchoolInstance(progamSettings, logger);

  // Sequence TODO: More professional sequence in state machine
  SchoolInstance.readTeachersAvailability();

  SchoolInstance.showTeachersAvailability();

  SchoolInstance.readDepartmentsAvailability();

  SchoolInstance.showDepartmentsAvailability();

  SchoolInstance.readTeachersAssignment();

  SchoolInstance.showTeachersAssignment();
  
  SchoolInstance.scheduleTimeTable();

  SchoolInstance.writeScheduledTimePlan();

  SchoolInstance.writeScheduledTeacherPlan();

  return 0;
}