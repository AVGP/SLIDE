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

    int numDesks = atoi((char *)config->getConfigValue((char *)"VirtualDesks",&len));
//    componentPIDs = (pid_t*)malloc((4*sizeof(pid_t)));//2+numDesks)*sizeof(pid_t));
    char dbg[255];
    sprintf(dbg,"# workspaces: %i",numDesks);
    Logger::getInstance()->log(dbg);

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
    struct sockaddr_un addr;
    CTRLMSG msg;
    msg.type = GEOMETRYREQUEST;
    msg.len  = 0;
    ctrlConnection->sendMessage(&msg,(char *)"/tmp/Slide_wm.sock");
    msg = ctrlConnection->getMessage(&addr);

    int sx,sy;
    memcpy(&sx,msg.msg,sizeof(int));
    memcpy(&sy,msg.msg+sizeof(int),sizeof(int));

    //Now the Client-Components
    char sw[5],sh[5];
    sprintf(sw,"%d",sx);
    sprintf(sh,"%d",sy);

    for(int i=0;i<numDesks;i++)
    {
        sprintf(dbg,"Starting Desk #%i",i);
        Logger::getInstance()->log(dbg);
//        componentPIDs[2+i] = fork();
        if(fork() == 0)//componentPIDs[2+i] == 0)
        {
            execl((char *)config->getConfigValue((char *)"DesktopApp",&len),(char *)"Desk",sw,sh,(char *)config->getConfigValue((char *)"DesktopWallpaper",&len),NULL);
        }
    }

    componentPIDs[1] = fork();
    if(componentPIDs[1] == 0)
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
