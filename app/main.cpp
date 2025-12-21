
#include <iostream>
#include "programsettings.h"
#include "school.h"
#include "logging.h"

int main() 
{
  //Getting program settings
  ProgramSettings progamSettings;
  settings.readProgramSettings();

  //Creating logging class instance and log settings
  Logging logger(settings.isLogModeOn(),
                 settings.isLogToConsoleOn(),
                 settings.isLogToFileOn(),
                 settings.getLogFilePath());

  logger.appendLog(M_INFO,
                   M_LOG_ENABLED,
                   (std::string)"LOG.1: main.cpp settings.readProgramSettings()" +
                   " logFilePath=" + settings.getLogFilePath() +
                   " isLogModeOn=" + to_string(settings.isLogModeOn()) +
                   " logFilePath=" + settings.getLogFilePath());

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

  return 0;
}