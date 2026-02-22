#include "lessonplanner.h"

int main() 
{
  // Get program settings
  ProgramSettings programSettings;
  if (programSettings.readProgramSettings() != 0)
  {
    return static_cast<int>(LessonPlanner::RuntimeErrors::ProgramSettingsLoad);
  }

  // Create logging class instance and log settings
  Logging logger(programSettings.isLogModeOn(),
                 programSettings.isLogToConsoleOn(),
                 programSettings.isLogToFileOn(),
                 programSettings.getLogFilePath());

  logger.appendLog(Logging::LogLevel::Info,
                   Logging::LogMode::Enabled,
                   (std::string)"LOG.1: lessonplanner.cpp programSettings.readProgramSettings()" +
                   " logFilePath=" + programSettings.getLogFilePath() +
                   " isLogModeOn=" + to_string(programSettings.isLogModeOn()) +
                   " logFilePath=" + programSettings.getLogFilePath());

  // School instance creation
  School schoolInstance(programSettings, logger);

  // Create lessonPlanner with dependency injection
  LessonPlanner lessonPlanner(schoolInstance);

  // Run program process
  auto result = lessonPlanner.run();

  return static_cast<int>(result);
}