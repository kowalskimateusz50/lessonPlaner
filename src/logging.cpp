#include "logging.h"

loggingToTextFile::loggingToTextFile(bool isLogModeOn,
                                     bool isLogToConsoleOn,
                                     bool isLogToFileOn, 
                                     string logFolderPath)
{
	/* Save log foler path into class */
  isLogModeOn_ = isLogModeOn;
  isLogToConsoleOn_ = isLogToConsoleOn;
  isLogToFileOn_ = isLogToFileOn;
	logFolderPath_ = logFolderPath;

	/* Create new local folder if doesn't exist */
    if (fs::create_directories(logFolderPath_))
    {
      cout << "Folder created.\n";
    }
    else
    {
      cout << "Folder already exist.\n";
    }
}
void loggingToTextFile::appendLog(int logType, string logMessage)
{
  stringstream sslogMessage;
  if (isLogModeOn_)
  {
    tTime = time(0);
	  localTime = localtime(&tTime);

    sslogMessage << getMessageType(logType) + "[" + to_string(localTime->tm_mday) + "/" + 
      addLeadingZero(localTime->tm_mon) + "/" + to_string(1900 + localTime->tm_year) + " " + 
        addLeadingZero(localTime->tm_hour) + ":" + addLeadingZero(localTime->tm_min) + ":" + 
          addLeadingZero(localTime->tm_sec) + "]" + ": " + logMessage << endl;

    if (isLogToConsoleOn_)
    {
      cout << sslogMessage.str();
    }

    if (isLogToFileOn_)
    {
      /* Prepare date for filename creation*/
      logFileName = to_string(1900 + localTime->tm_year) + addLeadingZero(localTime->tm_mon) + 
        to_string(localTime->tm_mday);
      logFilePath = (logFolderPath_ + logFileName + ".txt");

      /* File opening */
      logFile.open(logFilePath, std::ios_base::app);

      /* Writing string to file */
      logFile << sslogMessage.str();

      /* File closing */
      logFile.close();
    }
  }
}
string loggingToTextFile::addLeadingZero(int number)
{
	if (number < 10)
	{
		return ("0" + to_string(number));
	}
	else
	{
		return (to_string(number));
	}
}

string loggingToTextFile::getMessageType(int type)
{
	if (type == 1)
	{
		return "[Info]";
	}
	else if (type == 2)
	{
		return "[Warning]";
	}
	else if (type == 3)
	{
		return "[Error]";
	}
	return "";
}