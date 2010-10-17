#include "Slidewindowmanager.h"

SlideWindowManager::SlideWindowManager(bool debug)
{
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
