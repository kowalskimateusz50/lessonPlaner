#include "programsettings.h"

ProgramSettings::ProgramSettings()
{
  //Initialize input file path
  isLogModeOn_ = true;
  isLogToConsoleOn_ = true;
  isLogToFileOn_ = true;
  inputFilePath_ = "";
  timePlanFilePath_ = "";
  teacherPlanFilePath_ = "";
  logFilePath_ = "";
}

int ProgramSettings::readProgramSettings()
{
  YAML::Node config = YAML::LoadFile("ustawienia.yaml");

  inputFilePath_ = config["ustawienia"]["dane_wejsciowe"].as<std::string>();
  timePlanFilePath_ = config["ustawienia"]["plan_lekcji"].as<std::string>();
  teacherPlanFilePath_ = config["ustawienia"]["plan_nauczycieli"].as<std::string>();
  isLogModeOn_ = config["ustawienia_diagnostyczne"]["tryb_diagnostyczny"].as<bool>();
  logFilePath_ = config["ustawienia_diagnostyczne"]["plik_diagnostyczny"].as<std::string>();

  if (inputFilePath_ != "" && timePlanFilePath_ != "" && teacherPlanFilePath_ != "" &&
      logFilePath_ != "")
  {
    return 0;
  }
  return 1;
}

bool ProgramSettings::isLogModeOn()
{
  return isLogModeOn_;
}

bool ProgramSettings::isLogToConsoleOn()
{
  return isLogToConsoleOn_;
}

bool ProgramSettings::isLogToFileOn()
{
  return isLogToFileOn_;
}

std::string ProgramSettings::getInputFilePath()
{
    return inputFilePath_;
}

std::string ProgramSettings::getTimePlanFilePath()
{
    return timePlanFilePath_;
}

std::string ProgramSettings::getTeacherPlanFilePath()
{
    return teacherPlanFilePath_;
}

std::string ProgramSettings::getLogFilePath()
{
    return logFilePath_;
}