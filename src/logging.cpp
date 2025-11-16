#include "logging.h"

loggingToTextFile::loggingToTextFile(string iLogFolderPath)
{
	/* Save log foler path into class */
	logFolderPath = iLogFolderPath;

	/* Create new local folder if doesn't exist */
    if (fs::create_directories(logFolderPath))
    {
      cout << "Folder created.\n";
    }
    else
    {
      cout << "Folder already exist.\n";
    }
}
int loggingToTextFile::appendLog(string iLogMessage, int iLogType)
{
	tTime = time(0);
	localTime = localtime(&tTime);

	/* Prepare date for filename creation*/
	logFileName = to_string(1900 + local_time->tm_year) + AddLeadingZero(local_time->tm_mon) + to_string(local_time->tm_mday);
	logFilePath = logFolderPath + LogFileName + ".txt";

	/* File opening */
	logFile.open(LogFilePath, std::ios_base::app);

	/* Writing string to file */
	logFile << GetMessageType(LogType) + "[" + to_string(local_time->tm_mday) + "/" + 
    AddLeadingZero(local_time->tm_mon) + "/" + to_string(1900 + local_time->tm_year) + " " + 
      AddLeadingZero(local_time->tm_hour) + ":" + AddLeadingZero(local_time->tm_min) + ":" + 
        AddLeadingZero(local_time->tm_sec) + "]" + ": " + logMessage << endl;
	
	/* File closing */
	logFile.close();

	return 0;
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