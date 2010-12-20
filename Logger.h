/**
* @file Logger.h
* @brief Contains declarations for logging debug messages.
*/
#ifndef LOGGER_H_INCLUDED
#define LOGGER_H_INCLUDED

#include <fstream>
#include <string>
#include <cstdlib>

/**
* @class Logger
* This class provides you with the possibility to log messages
* to a log file. Yes, it is THAT easy.
* You should be aware, that Logger is a singleton!
*/
class Logger
{
  public:
    /**
    * Returns an instance of Logger. Either it creates one or
    * it returns the existing instance.
    * @return An instance of Logger.
    */
    static Logger *getInstance();

    /**
    * Writes the message into the logfile.
    * @param The Message to write out to the log file.
    */
    void log(char *msg);

    /**
    * Writes a C++ std::string into the logfile.
    * @param The message to write out to the log file.
    */
    void log(std::string msg);

  private:
    Logger();
    ~Logger();
    static Logger *instance;
	std::string logPath;
};

#endif // LOGGER_H_INCLUDED
