#include "Slidewindowmanager.h"

SlideWindowManager::SlideWindowManager(bool debug)
{
    disp             = XOpenDisplay(NULL);
    screen           = DefaultScreenOfDisplay(disp);
    screenWidth      = WidthOfScreen(screen);
    screenHeight     = HeightOfScreen(screen);
    numWorkspaces    = 0;
    currentWorkspace = 0;
    focusedWindow    = NULL;

//Just as long as we're debugging!
    if(debug)
    {
        XSynchronize(disp,True);
    }

    XSelectInput(disp, DefaultRootWindow(disp), SubstructureNotifyMask | ExposureMask | KeyPressMask );
    XGrabKey(disp,0x17,AnyModifier,DefaultRootWindow(disp),True,GrabModeAsync,GrabModeAsync); //CTRL+TAB & ALT+TAB
    XGrabKey(disp,0x64,ControlMask,DefaultRootWindow(disp),True,GrabModeAsync,GrabModeAsync); //CTRL+Left
    XGrabKey(disp,0x66,ControlMask,DefaultRootWindow(disp),True,GrabModeAsync,GrabModeAsync); //CTRL+Right
//    XGrabButton(disp,1,AnyModifier,DefaultRootWindow(disp),True,ButtonPressMask,GrabModeAsync,GrabModeAsync,None,None);
    ctrl = new SlideConnection((char *)"/tmp/Slide_wm.sock",COMP_WM);
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
                                if(event.xkey.state & Mod1Mask)
                                {
                                    untileWindows();
                                }
                                else
                                {
                                    tileWindows();
                                }
                            }
                            else if(event.xkey.state & Mod1Mask)
                            {
                                XCirculateSubwindows(disp,desktop[currentWorkspace],RaiseLowest);
                            }
                            break;
                        case 0x64: //Left-Arrow
                            if(currentWorkspace == 0) currentWorkspace = numWorkspaces-1;
                            else currentWorkspace--;
                            XRaiseWindow(disp,desktop[currentWorkspace]);
                            break;
                        case 0x66: //Right-Arrow
                            if(currentWorkspace == numWorkspaces-1) currentWorkspace = 0;
                            else currentWorkspace++;
                            XRaiseWindow(disp,desktop[currentWorkspace]);
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
                        focusWindow(&event);
                    }
                    break;
                case DestroyNotify:
                    closeWindow(&event);
                    break;
                case MotionNotify:
                    focusWindow(&event);
                    moveWindow(&event);
                    break;
                case Expose:
                    for(unsigned int i=0;i<windows.size();i++)
                    {
                        if(event.xexpose.window == windows[i]->getWindow(true))
                        {
                            if(windows[i]->state & SlideWindow::STATE_FOCUSED) windows[i]->drawDecoration(true);
                            else windows[i]->drawDecoration(false);
                            break;
                        }
                    }

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
            else if(msg.type == WINDOWLISTINTEREST)
            {
                char dbg[100];
                sprintf(dbg,"[WLI] Got msg from %s",msg.addr.sun_path);
                Logger::getInstance()->log(dbg);
                windowChangeListeners.push_back(msg.addr);
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
                (*iter)->close(false);
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
        SlideWindow *w = new SlideWindow(disp,e->xmap.window,desktop[currentWorkspace],currentWorkspace);
        w->state |= SlideWindow::STATE_FOCUSED;

        if(focusedWindow != NULL)
        {
            focusedWindow->state ^= SlideWindow::STATE_FOCUSED;
        }
        focusedWindow = w;

        XSetInputFocus(disp,focusedWindow->getWindow(),RevertToNone,CurrentTime);

        windows.push_back(w);
        CTRLMSG msg;
        msg.type = WINDOWLISTCREATEWND;
        msg.len = sizeof(SlideWindow);
        memcpy(msg.msg,w,sizeof(SlideWindow));

        for(unsigned int i=0;i<windowChangeListeners.size();i++)
        {
            Logger::getInstance()->log("Sending WML-Create-Notify");
            ctrl->sendMessage(&msg,windowChangeListeners[i].sun_path);
        }
    }
    else if(strncmp(wndName,"__SLIDE__Desktop",16) == 0)
    {
        numWorkspaces++;
        desktop = (Window *)realloc((void *)desktop,numWorkspaces*sizeof(Window));
        SlideWindow *w = new SlideWindow(disp,e->xmap.window,DefaultRootWindow(disp));
        desktop[numWorkspaces-1] = w->getWindow();
    }
}

void SlideWindowManager::focusWindow(XEvent *e)
{
    Window evtWnd = None;
    if(e->type == ButtonPress || ButtonRelease)
    {
        evtWnd = e->xbutton.window;
    }
    else if(e->type == MapNotify)
    {
        evtWnd = e->xmap.window;
    }
    else if(e->type == MotionNotify)
    {
        evtWnd = e->xmotion.window;
    }


    for(unsigned int i=0;i<windows.size();i++)
    {
        if(windows[i]->getWindow(true) == evtWnd)
        {
            XEvent e;
            e.type = Expose;
            e.xexpose.window = windows[i]->getWindow(true);

            focusedWindow->state ^= SlideWindow::STATE_FOCUSED;
            windows[i]->state |= SlideWindow::STATE_FOCUSED;

            XSendEvent(disp,windows[i]->getWindow(true),True,ExposureMask,&e);
            e.xexpose.window = focusedWindow->getWindow(true);
            XSendEvent(disp,focusedWindow->getWindow(true),True,ExposureMask,&e);

            focusedWindow = windows[i];
        }
    }
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
            SlideWindow *w;
            for(unsigned int i=0;i<windows.size();i++)
            {
                if(e->xmotion.window == windows[i]->getWindow(true))
                {
                    w = windows[i];
                    focusedWindow->state ^= SlideWindow::STATE_FOCUSED;
                    w->state |= SlideWindow::STATE_FOCUSED;
                    focusedWindow = w;
                    break;
                }
            }
            do
            {
                XNextEvent(disp,e);
                if(e->type == MotionNotify)
                {
                    int diff_x = e->xmotion.x_root - start_x;
                    int diff_y = e->xmotion.y_root - start_y;
                    if(attr.x+diff_x < -100)
                    {
                        if(currentWorkspace == 0) currentWorkspace = numWorkspaces-1;
                        else currentWorkspace--;
                        w->putOnDesk(currentWorkspace,desktop[currentWorkspace]);
                        w->move(screenWidth-attr.width,attr.y+diff_y,true);
                        XWarpPointer(disp,None,desktop[currentWorkspace],0,0,0,0,screenWidth-e->xmotion.x_root,e->xmotion.y_root-40);
                        XRaiseWindow(disp,desktop[currentWorkspace]);
                    }
                    else if(attr.x+attr.width+diff_x > screenWidth+100)
                    {
                        if(currentWorkspace == numWorkspaces-1) currentWorkspace = 0;
                        else currentWorkspace++;
                        w->putOnDesk(currentWorkspace,desktop[currentWorkspace]);
                        w->move(0,attr.y+diff_y,true);
                        XWarpPointer(disp,None,desktop[currentWorkspace],0,0,0,0,screenWidth-e->xmotion.x_root,e->xmotion.y_root-40);
                        XRaiseWindow(disp,desktop[currentWorkspace]);
                    }
                    else
                    {
                        w->move(attr.x+diff_x,attr.y+diff_y,true);
                    }
                    //XMoveWindow(disp,e->xmotion.window,attr.x+diff_x,attr.y+diff_y);
                }
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
        if(e->xbutton.window == windows[i]->getWindow(true))
        {
           if(windows[i]->state & SlideWindow::STATE_MAXIMIZED)
            {
                windows[i]->restoreGeometry();
                windows[i]->state ^= SlideWindow::STATE_MAXIMIZED;
                focusWindow(e);
            }
            else
            {
                windows[i]->move(0,0);
                windows[i]->resize(screenWidth,screenHeight-40);
                focusWindow(e);
                windows[i]->state |= SlideWindow::STATE_MAXIMIZED;
            }
        }
    }
}

void SlideWindowManager::tileWindows()
{
    unsigned int x=0,y=0,numWindows = 0;
    for(unsigned int i=0;i<windows.size();i++)
    {
        if(windows[i]->getDesk() == currentWorkspace && (windows[i]->state & SlideWindow::STATE_SHOWN)) numWindows++;
    }
    int widthPerWindow  = screenWidth/(numWindows < 4 ? numWindows : 4);
    int screenpart = ceil(numWindows/4);
    int heightPerWindow = (screenHeight-40)/(screenpart == 0 ? 1 : screenpart);
    for(unsigned int i=0;i<windows.size();i++)
    {
        if(windows[i]->getDesk() == currentWorkspace)
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
}

void SlideWindowManager::untileWindows()
{
    for(unsigned int i=0;i<windows.size();i++)
    {
        if(windows[i]->getDesk() == currentWorkspace)
        {
            windows[i]->restoreGeometry();
            if(windows[i]->state & SlideWindow::STATE_MAXIMIZED)
            {
                windows[i]->move(0,0);
                windows[i]->resize(screenWidth,screenHeight-40);
            }
        }
    }
}

void SlideWindowManager::resizeWindow(XEvent *e)
{}
