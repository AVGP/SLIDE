#include "Slidewindowmanager.h"

SlideWindowManager::SlideWindowManager(bool debug)
{
    ctrl = new SlideConnection((char *)"/tmp/Slide_wm.sock",COMP_WM);
    disp         = XOpenDisplay(NULL);
    screen       = DefaultScreenOfDisplay(disp);
    screenWidth  = WidthOfScreen(screen);
    screenHeight = HeightOfScreen(screen);
    desktop      = DefaultRootWindow(disp);

//Just as long as we're debugging!
    if(debug)
    {
        XSynchronize(disp,True);
    }

    XSelectInput(disp, DefaultRootWindow(disp), SubstructureNotifyMask | KeyPressMask );
    XGrabKey(disp,0x17,AnyModifier,DefaultRootWindow(disp),True,GrabModeAsync,GrabModeAsync); //CTRL+TAB & ALT+TAB
//    XGrabKey(disp,AnyKey,Mod1Mask,DefaultRootWindow(disp),True,GrabModeAsync,GrabModeAsync);
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
                            if(event.xkey.state & ControlMask)
                            {
                                tileWindows();
                            }
                            else if(event.xkey.state & Mod1Mask)
                            {
                                XCirculateSubwindows(disp,desktop,RaiseLowest);
                            }
                            break;
                        default:
                            sprintf(msg,"Keycode: %i",event.xkey.keycode);
                            Logger::getInstance()->log(msg);
                            break;
                    }
                    break;
                case ButtonRelease:
                    if(event.xbutton.subwindow != None)
                    {
                        XFetchName(disp,event.xbutton.subwindow,&wnd_name);
                        sprintf(msg,"[Press] Window-Title: %s",wnd_name);
                        Logger::getInstance()->log(msg);

                        if(strncmp(wnd_name,"SlideClose",10) == 0)
                        {
                            closeWindow(&event);
                        }
                        else if(strncmp(wnd_name,"SlideMaxi",9) == 0)
                        {
                            maximizeWindow(&event);
                        }
                        else
                        {
                            focusWindow(&event);
                        }
                    }
                    else
                    {
                        focusWindow(&event);
                    }
                    break;
                case MapNotify:
                    //if(event.xmap.event != None) break;
                    Logger::getInstance()->log((std::string)"MapNotify");
                    XFetchName(disp,event.xmap.window,&wnd_name);
                    sprintf(msg,"Window-Title: %s",wnd_name);
                    Logger::getInstance()->log(msg);

                    if(strncmp(wnd_name,"Slide",5) != 0)
                    {
                        createWindow(&event);
                    }
                    break;
                case DestroyNotify:
                    Logger::getInstance()->log("Destroy me");

//                    XFetchName(disp,event.xdestroywindow.event,&wnd_name);
                    sprintf(msg,"[Destroy] Window-ID: %i %i",event.xdestroywindow.window,event.xdestroywindow.event);
                    Logger::getInstance()->log(msg);

/*                    if(event.xdestroywindow.window != None)
                    {
                        if(strncmp(wnd_name,"Slide",5) != 0)
                        {
                            Window root,parent,**child;
                            XQueryTree(disp,event.xdestroywindow.window,&root,&parent,child,0);
                            XDestroyWindow(disp,parent);
                        }
                        XDestroyWindow(disp,event.xdestroywindow.window);
                    }
                    else //the application already destroyed the window >:(
                    {*/
                    closeWindow(&event);
//                    }
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
{
    if(e->type == ButtonRelease || e->type == ButtonPress)
    {
        std::vector<SlideWindow *>::iterator iter = windows.begin();
        while(iter != windows.end())
        {
            if((*iter)->getWindow(true) == e->xbutton.window)
            {
                (*iter)->close();
                windows.erase(iter);
                break;
            }
            iter++;
        }
    }
    else
    {
                        std::vector<SlideWindow *>::iterator iter = windows.begin();
                        while(iter != windows.end())
                        {
                            if((*iter)->getWindow() == e->xdestroywindow.event)
                            {
                                (*iter)->close();
                                windows.erase(iter);
                                break;
                            }
                            iter++;
                        }
    }
}

void SlideWindowManager::createWindow(XEvent *e)
{
    char *wndName;
    XFetchName(disp,e->xmap.window,&wndName);
    if(strncmp(wndName,"__SLIDE__",9) != 0)
    {
        SlideWindow *w = new SlideWindow(disp,e->xmap.window,desktop);
        windows.push_back(w);
    }
    else if(strncmp(wndName,"__SLIDE__Desktop",16) == 0)
    {
        SlideWindow *w = new SlideWindow(disp,e->xmap.window,desktop);
        desktop = w->getWindow();
    }
    else
    {
//        SlideWindow *w = new SlideWindow(disp,e->xmap.window,DefaultRootWindow(disp));
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

void SlideWindowManager::maximizeWindow(XEvent *e)
{
    for(unsigned int i=0;i<windows.size();i++)
    {
        if(e->xbutton.window == windows[i]->getWindow())
        {
            windows[i]->move(0,0);
            windows[i]->resize(screenWidth,screenHeight-40);
            focusWindow(e);
        }
    }
}

void SlideWindowManager::tileWindows()
{
    int x=0,y=0;
    int widthPerWindow  = screenWidth/(windows.size() < 4 ? windows.size() : 4);
    int screenpart = ceil(windows.size()/4);
    int heightPerWindow = (screenHeight-40)/(screenpart == 0 ? 1 : screenpart);
    for(unsigned int i=0;i<windows.size();i++)
    {
        windows[i]->resize(widthPerWindow,heightPerWindow);
        windows[i]->move(x,y);
        x += widthPerWindow;
        if(x >= screenWidth)
        {
            x  = 0;
            y += heightPerWindow;
        }
    }
}

void SlideWindowManager::resizeWindow(XEvent *e)
{}
