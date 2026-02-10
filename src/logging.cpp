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
  fs::path filePathObj(logFolderPath_);
  fs::path directory = filePathObj.parent_path();
  // Create folder if doesn't exists
  if (!directory.empty() && !fs::exists(directory))
  {
    try
    {
        fs::create_directories(directory);
        std::cout << "Created directory: "<< directory << '\n';
    }
    catch (const fs::filesystem_error& e)
    {
        std::cerr << "Directory creation failed: "
                  << e.what() << '\n';
        return;
    }
  }

  /* Initialize time */
  std::time_t now = std::time(nullptr);
  localtime_r(&now, &localTime_);

  /* Prepare date for filename creation */
  logFileName_ =
    std::to_string(1900 + localTime_.tm_year) +
      addLeadingZero(localTime_.tm_mon + 1) +
        addLeadingZero(localTime_.tm_mday);

  logFilePath_ = logFolderPath_ + "/" + logFileName_+ ".txt";

  logFile_.open(logFilePath_, std::ios::app);
  if (!logFile_)
  {
      throw std::runtime_error("Failed to open log file");
  }
}

Logging::~Logging()
{
  /* File closing */
  logFile_.close();
}

void Logging::appendLog(LogLevel logLevel, LogMode logMode, std::string logMessage)
{
  if (isLogModeOn_ && (logMode == LogMode::Enabled))
  {
    std::string message = getLogLevelType(logLevel) + "[" + std::to_string(localTime_.tm_mday) + 
      "/" + addLeadingZero(localTime_.tm_mon) + "/" + std::to_string(1900 + localTime_.tm_year) + 
        " " + addLeadingZero(localTime_.tm_hour) + ":" + addLeadingZero(localTime_.tm_min) + ":" +
          addLeadingZero(localTime_.tm_sec) + "]" + ": " + logMessage + "\n";

    std::lock_guard<std::mutex> lock(mLock_);

    if (isLogToConsoleOn_)
    {
      std::cout << message;
    }
    if (isLogToFileOn_)
    {
      /* Writing string to file */
      logFile_ << message;
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