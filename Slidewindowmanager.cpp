#include "Slidewindowmanager.h"

SlideWindowManager::SlideWindowManager(bool debug)
{
    ctrl = new SlideConnection("/tmp/Slide_wm.sock",COMP_WM);
    disp         = XOpenDisplay(NULL);
    screen       = DefaultScreenOfDisplay(disp);
    screenWidth  = WidthOfScreen(screen);
    screenHeight = HeightOfScreen(screen);

//Just as long as we're debugging!
    if(debug)
    {
        XSynchronize(disp,True);
    }


    XSelectInput(disp, DefaultRootWindow(disp), SubstructureNotifyMask | SubstructureRedirectMask | PropertyChangeMask);
    XGrabButton(disp,1,AnyModifier,DefaultRootWindow(disp),True,ButtonPressMask,GrabModeAsync,GrabModeAsync,None,None);
}

bool SlideWindowManager::run()
{
    while(1)
    {
        XEvent event;
        if(XPending(disp) > 0)
        {
            XNextEvent(disp,&event);
            char msg[255],*dbg_name;
            switch(event.type)
            {
                case MapRequest:
                    Logger::getInstance()->log("MapNotify");
                    XFetchName(disp,event.xmaprequest.window,&dbg_name);
                    sprintf(msg,"Window-Title: %s",dbg_name);
                    Logger::getInstance()->log(msg);
                    createWindow(&event);
                    break;
                default:
                    sprintf(msg,"Event %i ocurred.",event.type);
                    Logger::getInstance()->log(msg);
            }
        }
        else
        {
            struct sockaddr_un addr;
            CTRLMSG msg = ctrl->peekMessage(&addr);
            if(msg.type == GEOMETRYREQUEST)
            {
                char dbg[100];
                sprintf(dbg,"Got msg from %s",msg.addr.sun_path);
                Logger::getInstance()->log(dbg);
                msg.type = GEOMETRYREPLY;
                msg.len = sizeof(int)*2;
                memcpy(msg.msg,&screenWidth,sizeof(int));
                memcpy(msg.msg+sizeof(int),&screenHeight,sizeof(int));
                ctrl->sendMessage(&msg,msg.addr.sun_path);
            }
        }
    }
    return true;
}

void SlideWindowManager::closeWindow(XEvent *e)
{}

void SlideWindowManager::createWindow(XEvent *e)
{

    SlideWindow *w = new SlideWindow(disp,e->xmap.window,desktop);
    windows.push_back(w);
}

void SlideWindowManager::focusWindow(XEvent *e)
{}

void SlideWindowManager::moveWindow(XEvent *e)
{}

void SlideWindowManager::resizeWindow(XEvent *e)
{}
