#include "Slidewindowmanager.h"

SlideWindowManager::SlideWindowManager(bool debug)
{
    ctrl = new SlideConnection((char *)"/tmp/Slide_wm.sock",COMP_WM);
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
                    Logger::getInstance()->log((std::string)"MapNotify");
                    XFetchName(disp,event.xmaprequest.window,&dbg_name);
                    sprintf(msg,"Window-Title: %s",dbg_name);
                    Logger::getInstance()->log(msg);
                    createWindow(&event);
                    break;
                case MotionNotify:

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
{
    char *wndName;
    if(e->xmotion.window != None)
    {
        //Hier unbedingt (!!) den ClassHint benutzen!
        XFetchName(disp,e->xmotion.window,&wndName);
        if(strncmp("TWM__Decoration",wndName,15) == 0)
        {
            XWindowAttributes attr;
            XGetWindowAttributes(disp,e->xmotion.window,&attr);
            XGrabPointer(disp, e->xmotion.window, False, PointerMotionMask|ButtonReleaseMask, GrabModeAsync, GrabModeAsync, None, None, CurrentTime);
            int start_x = e->xmotion.x_root;
            int start_y = e->xmotion.y_root;
            do
            {
                XNextEvent(disp,e);
                int diff_x = e->xmotion.x_root - start_x;
                int diff_y = e->xmotion.y_root - start_y;
                XMoveWindow(disp,e->xmotion.window,attr.x+diff_x,attr.y+diff_y);
            }
            while(e->type == MotionNotify);
            XUngrabPointer(disp, CurrentTime);
        }
    }

}

void SlideWindowManager::resizeWindow(XEvent *e)
{}
