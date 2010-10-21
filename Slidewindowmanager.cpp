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

    XSelectInput(disp, DefaultRootWindow(disp), SubstructureNotifyMask | KeyPressMask );
    XGrabKey(disp,0x17,ControlMask,DefaultRootWindow(disp),True,GrabModeAsync,GrabModeAsync); //CTRL+TAB
//    XGrabButton(disp,1,AnyModifier,DefaultRootWindow(disp),True,ButtonPressMask,GrabModeAsync,GrabModeAsync,None,None);
}

bool SlideWindowManager::run()
{
    while(1)
    {
        XEvent event;
        if(XPending(disp) > 0)
        {
            XNextEvent(disp,&event);
            char msg[255],*wnd_name;
            switch(event.type)
            {
                case KeyPress:
                    switch(event.xkey.keycode)
                    {
                        case 0x17: //TAB
                            tileWindows();
                            break;
                    }
                    break;
                case ButtonRelease:
                    focusWindow(&event);
                    break;
                case MapNotify:

                    Logger::getInstance()->log((std::string)"MapNotify");
                    XFetchName(disp,event.xmap.window,&wnd_name);
                    sprintf(msg,"Window-Title: %s",wnd_name);
                    Logger::getInstance()->log(msg);

                    if(strncmp(wnd_name,"SlideCmp",8) != 0)
                    {
                        createWindow(&event);
                    }
                    break;
                case DestroyNotify:
                    if(event.xdestroywindow.event != None) XDestroyWindow(disp,event.xdestroywindow.event);
                    break;
                case MotionNotify:
                    Logger::getInstance()->log("Motion!");
                    moveWindow(&event);
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
    char *wndName;
    XFetchName(disp,e->xmap.window,&wndName);
    SlideWindow *w = new SlideWindow(disp,e->xmap.window,desktop);
    if(strncmp(t_name,"__SLIDE__",9) != 0)
    {
        windows.push_back(w);
    }
}

void SlideWindowManager::focusWindow(XEvent *e)
{
    XMapRaised(disp,e->xbutton.window);
    XSetInputFocus(disp,e->xbutton.window,RevertToNone,CurrentTime);
}

void SlideWindowManager::moveWindow(XEvent *e)
{
    char *wndName;
    if(e->xmotion.window != None)
    {
        XFetchName(disp,e->xmotion.window,&wndName);
        if(strncmp("SlideDeco",wndName,9) == 0)
        {
            XWindowAttributes attr;
            XGetWindowAttributes(disp,e->xmotion.window,&attr);
            XGrabButton(disp,1,AnyModifier,DefaultRootWindow(disp),False,ButtonPressMask,GrabModeSync,GrabModeAsync,None,None);
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
            XUngrabButton(disp,1,AnyModifier,DefaultRootWindow(disp));
        }
    }

}

void SlideWindowManager::tileWindows()
{

}

void SlideWindowManager::resizeWindow(XEvent *e)
{}
