#include "logging.h"

namespace fs = std::filesystem;

Logging::Logging(bool isLogModeOn,
                 bool isLogToConsoleOn,
                 bool isLogToFileOn, 
                 std::string logFolderPath)
{
	/* Save logging settings */
  isLogModeOn_ = isLogModeOn;
  isLogToConsoleOn_ = isLogToConsoleOn;
  isLogToFileOn_ = isLogToFileOn;
	logFolderPath_ = logFolderPath;

	/* Create new local folder if doesn't exist */
  if (fs::create_directories(logFolderPath_))
  {
    std::cout << "Folder for log files created." << std::endl;
  }
  else
  {
    std::cout << "Log file folder already exist." << std::endl;
  }

  tTime = time(0);
	localTime = localtime(&tTime);

  /* Prepare date for filename creation */
  logFileName = std::to_string(1900 + localTime->tm_year) + addLeadingZero(localTime->tm_mon) + 
  std::to_string(localTime->tm_mday);
  logFilePath = (logFolderPath_ + logFileName + ".txt");

  /* File opening */
  logFile.open(logFilePath, std::ios_base::app);

  std::cout << "Log file opened." << std::endl;
}

Logging::~Logging()
{
  /* File closing */
  logFile.close();
}

void Logging::appendLog(LogLevel logLevel, LogMode logMode, std::string logMessage)
{
  if (isLogModeOn_ && (logMode == LogMode::Enabled))
  {
    std::string message = getLogLevelType(logLevel) + "[" + std::to_string(localTime->tm_mday) + 
      "/" + addLeadingZero(localTime->tm_mon) + "/" + std::to_string(1900 + localTime->tm_year) + 
        " " + addLeadingZero(localTime->tm_hour) + ":" + addLeadingZero(localTime->tm_min) + ":" +
          addLeadingZero(localTime->tm_sec) + "]" + ": " + logMessage + "\n";

    std::lock_guard<std::mutex> lock(mLock_);

    if (isLogToConsoleOn_)
    {
      std::cout << message;
    }
    if (isLogToFileOn_)
    {
      /* Writing string to file */
      logFile << message;
    }
  }
}

std::string Logging::addLeadingZero(int number)
{
	if (number < 10)
	{
		return ("0" + std::to_string(number));
	}
	else
	{
		return (std::to_string(number));
	}
}

std::string Logging::getLogLevelType(LogLevel logLevel)
{
  switch (logLevel)
  {
    case LogLevel::Idle: return "[Idle]";
    case LogLevel::Info: return "[Info]";
    case LogLevel::Warning: return "[Warning]";
    case LogLevel::Error: return "[Error]";
  }
	return "Unknown";
}