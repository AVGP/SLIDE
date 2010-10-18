/**
* @file Slide.h
* @brief Contains the declarations for the Slide-Core&Watchdog
*/
#ifndef _Slide_H_
#define _Slide_H_

#include "Global.h"
#include "Slideconfig.h"
#include "Slidewindowmanager.h"
#include "Logger.h"

/**
* @class Slide
* This class is the core of Slide. It provides an interface to the
* core of the environment. Its responsible for starting and stopping
* everything and watches over the other processes (or threads) and
* does the regeneration if neccessary.
*/
class Slide
{
  public:
    /**
    * As Slide is a singleton-class, you need to call this
    * to obtain the instance. A new instance is created internally
    * if there hasn't been one before.
    */
    static Slide *getInstance();

    /**
    * Is called from main to start the Environment. Blocks.
    * @ToDo Load Configuration, start WM.
    * @param Set this to true, to have debug output. Default is false.
    * @return Returns true, if start up was successful.
    */
    bool startUp(bool debug = false);

    /**
    * Shuts down the instance of Slide. Usually this is triggered from the internal event-loop
    * on behalf of a SHUTDOWNREQUEST-message received via the connection to the CommServer.
    * @return Returns true, if the shutdown was successfully completed.
    */
    bool shutDown();

    /**
    * Runs the actual event loop. This method returns, when either the event loop died somehow
    * Or.... never.
    */
    void run();
  private:
    Slide();
    ~Slide();
    static Slide *instance;
    SlideConnection *ctrlConnection;
    SlideConfig *config;
    pid_t componentPIDs[3]; // Indices: 0 = WM, 1 = Desktop, 2 = Tray
};

#endif
