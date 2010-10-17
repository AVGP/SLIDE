#include "Slide.h"

Slide *Slide::instance = NULL;

Slide::Slide()
{}

Slide *Slide::getInstance()
{
    if(instance == NULL)
    {
        instance = new Slide();
        return instance;
    }
    else
    {
        return instance;
    }
}

bool Slide::startUp(bool debug)
{
    Logger::getInstance()->log("Starting up.");
    //Reading the config:
    //config = new SlideConfig();

    //Starting up the components:
    //First we start the CommServer
    componentPIDs[0] = fork();
    if(componentPIDs[0] == 0)
    {
        SlideCommServer *commServer = new SlideCommServer("/tmp/Slide_commserver.sock");
        if(!commServer->run() && debug)
        {
            Logger::getInstance()->log("FAULT: CommServer failed!");
            exit(EXIT_FAILURE);
        }
    }
    Logger::getInstance()->log("STATUS: CommServer running.");
    //Now the WM

    componentPIDs[1] = fork();
    if(componentPIDs[1] == 0)
    {
        SlideWindowManager *wm = new SlideWindowManager(true);
        if(!wm->run() && debug)
        {
            Logger::getInstance()->log("FAULT: Windowmanager failed!");
            exit(EXIT_FAILURE);
        }
    }

    //Now the Client-Components
    if(fork() == 0) execl("/usr/bin/SlideStarter","SlideComponent",(char *)0);


    Logger::getInstance()->log("STATUS: AWESOME STARTUP.");

    return true;
}

bool Slide::shutDown()
{

}


void Slide::run()
{
    while(1)
    {
        sleep(1);
    }
}
