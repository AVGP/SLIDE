#include "Slide.h"

Slide *Slide::instance = NULL;

Slide::Slide()
{}

const Slide *Slide::getInstance()
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
    //Reading the config:
    config = new SlideConfig();

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
    //Now the WM
    /*
    componentPIDs[1] = fork();
    if(componentPIDs[1] == 0)
    {
        SlideWindowManager wm = SlideWindowManager::getInstance();
        if(!wm->run() && debug)
        {
            Logger::getInstance()->log("FAULT: Windowmanager failed!");
            exit(EXIT_FAILURE);
        }
    }
*/
    return true;
}

bool Slide::shutDown()
{

}
