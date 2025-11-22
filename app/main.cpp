
#include <iostream>
#include "programSettings.h"
#include "school.h"
#include "logging.h"

using namespace std;

int main() 
{
  //Getting program settings 
  ProgramSettings settings;
  settings.readProgramSettings();

  //Creating logging class instance and log settings
  loggingToTextFile logger(settings.isLogModeOn(),
                           settings.isLogToConsoleOn(),
                           settings.isLogToFileOn(),
                           settings.getLogFilePath());
  
  logger.appendLog(M_INFO, (string)"LOG.1: main.cpp settings.readProgramSettings()"
                   " logFilePath=" + settings.getLogFilePath() + 
                   " isLogModeOn=" + to_string(settings.isLogModeOn()) + 
                   " logFilePath=" + settings.getLogFilePath());

  //Open xlsx document
  OpenXLSX::XLDocument doc;
  doc.open(settings.getInputFilePath());
  //Open worksheet
  OpenXLSX::XLWorksheet wks = doc.workbook().worksheet(1);

  //school instance test
  school schoolInstance;
  //read teachers
  schoolInstance.readTeachersAvailability(logger, wks);
  schoolInstance.showTeachersAvailability(logger);
  schoolInstance.readDepartmentsAvailability(logger, wks);
  schoolInstance.showDepartmentsAvailability(logger);

  return 0;
}