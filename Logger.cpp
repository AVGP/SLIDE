#include "Logger.h"

Logger::Logger()
{
	logPath = getenv("HOME");
	logPath += "/.Slide_debug.log";
}

Logger *Logger::instance = NULL;

Logger *Logger::getInstance()
{
    if(Logger::instance == NULL) instance = new Logger();
    return instance;
}

void Logger::log(char *msg)
{
    std::ofstream logFile(LOG_PATH,std::ios_base::app);
    if(logFile.is_open())
    {
        logFile << msg << "\n";
    }
    logFile.flush();
    logFile.close();
}

void Logger::log(std::string msg)
{
    std::ofstream logFile(LOG_PATH,std::ios_base::app);
    if(logFile.is_open())
    {
        logFile << msg << "\n";
    }
    logFile.flush();
    logFile.close();
}
