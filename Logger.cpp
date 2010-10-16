#include "Logger.h"

Logger::Logger()
{}

Logger *Logger::instance = NULL;

Logger *Logger::getInstance()
{
    if(Logger::instance == NULL) instance = new Logger();
    return instance;
}

void Logger::log(char *msg)
{
    std::ofstream logFile(LOG_PATH,std::ios_base::app);
    if(!logFile)
    {
        logFile << msg << std::endl;
    }
    logFile.close();
}
