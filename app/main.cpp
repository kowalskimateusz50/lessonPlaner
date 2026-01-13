
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

  //school instance creation
  school schoolInstance(progamSettings, logger);

  // Sequence TODO: More professional sequence in state machine
  schoolInstance.readTeachersAvailability();

  schoolInstance.showTeachersAvailability();

  schoolInstance.readDepartmentsAvailability();

  schoolInstance.showDepartmentsAvailability();

  schoolInstance.readTeachersAssignment();

  schoolInstance.showTeachersAssignment();
  
  schoolInstance.scheduleTimeTable();

  schoolInstance.writeScheduledTimePlan();

  schoolInstance.writeScheduledTeacherPlan();

  return 0;
}