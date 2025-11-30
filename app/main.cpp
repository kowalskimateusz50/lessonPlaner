
#include <iostream>
#include "programsettings.h"
#include "school.h"
#include "logging.h"

using namespace std;

int main() 
{
  //Getting program settings
  ProgramSettings settings;
  settings.readProgramSettings();

  //Creating logging class instance and log settings
  Logging logger(settings.isLogModeOn(),
                           settings.isLogToConsoleOn(),
                           settings.isLogToFileOn(),
                           settings.getLogFilePath());

  logger.appendLog(M_INFO,
                   M_LOG_ENABLED,
                   (string)"LOG.1: main.cpp settings.readProgramSettings()" +
                   " logFilePath=" + settings.getLogFilePath() +
                   " isLogModeOn=" + to_string(settings.isLogModeOn()) +
                   " logFilePath=" + settings.getLogFilePath());

  //Open xlsx document
  OpenXLSX::XLDocument doc;
  doc.open(settings.getInputFilePath());
  //Open worksheet
  OpenXLSX::XLWorksheet wks = doc.workbook().worksheet(1);

  //school instance test
  school schoolInstance(wks, logger);
  //read teachers
  schoolInstance.readTeachersAvailability();
  schoolInstance.showTeachersAvailability();
  schoolInstance.readDepartmentsAvailability();
  schoolInstance.showDepartmentsAvailability();
  schoolInstance.readTeachersAssignment();
  schoolInstance.showTeachersAssignment(); 

  doc.close();

  return 0;
}