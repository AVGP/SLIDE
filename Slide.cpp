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
    Logger::getInstance()->log((std::string)"Starting up.");
    //Reading the config:
    config = new SlideConfig();

    unsigned int len;
    //Starting up the components:
    //Start the WM
    componentPIDs[0] = fork();
    if(componentPIDs[0] == 0)
    {
        SlideWindowManager *wm = new SlideWindowManager(debug);
        if(!wm->run() && debug)
        {
            Logger::getInstance()->log((std::string)"FAULT: Windowmanager failed!");
            exit(EXIT_FAILURE);
        }

    }
    //Setup the local socket:
    ctrlConnection = new SlideConnection((char *)"/tmp/Slide_core.sock",COMP_CORE);

    //Get resolution from the WM:
    CTRLMSG msg;
    msg.type = GEOMETRYREQUEST;
    msg.len  = 0;
    ctrlConnection->sendMessage(&msg,(char *)"/tmp/Slide_wm.sock");
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

    componentPIDs[1] = fork();
    if(componentPIDs[1] == 0)
    {
        execl((char *)config->getConfigValue((char *)"DesktopApp",&len),(char *)"SlideComponent",sw,sh,(char *)config->getConfigValue((char *)"DesktopWallpaper",&len),(char *)0);
    }

    componentPIDs[2] = fork();
    if(componentPIDs[2] == 0)
    {
        execl((char *)config->getConfigValue((char *)"TrayApp",&len),(char *)"SlideComponent",sw,sh,(char *)0);
    }

    Logger::getInstance()->log((std::string)"STATUS: AWESOME STARTUP.");

    return true;
}

bool Slide::shutDown()
{
    return true;
}


void Slide::run()
{
    while(1)
    {
        sleep(1);
    }
}
