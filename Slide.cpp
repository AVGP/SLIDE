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
    config = new SlideConfig();

    unsigned int len;
    //Starting up the components:
    //First we start the CommServer
    /*
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
    */
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

    //Setup the local socket:
    ctrlConnection = new SlideConnection("/tmp/Slide_core.sock",COMP_CORE);
    CTRLMSG msg;
    msg.type = GEOMETRYREQUEST;
    msg.len  = 0;
    ctrlConnection->sendMessage(&msg,"/tmp/Slide_wm.sock");
    struct sockaddr_un addr;
    msg = ctrlConnection->getMessage(&addr);
    char dbg_out[100];
    int sx,sy;
    memcpy(&sx,msg.msg,sizeof(int));
    memcpy(&sy,msg.msg+sizeof(int),sizeof(int));
    sprintf(dbg_out,"Geometry: %i x %i",sx,sy);//(int *)(msg.msg)+1);
    Logger::getInstance()->log(dbg_out);
    //Now the Client-Components

    char sw[5],sh[5];
    sprintf(sw,"%d",sx);
    sprintf(sh,"%d",sy);
    if(fork() == 0) execl((char *)config->getConfigValue("DesktopApp",&len),"SlideComponent",sw,sh,(char *)config->getConfigValue("DesktopWallpaper",&len),(char *)0);

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
