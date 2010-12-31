#include "Slidewindowmanager.h"

SlideWindowManager::SlideWindowManager(bool debug)
{
    Logger::getInstance()->log("WM starting...");
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
    XSetErrorHandler((int (*)(Display *, XErrorEvent *))&SlideWindowManager::errorHandler);

    XSelectInput(disp, DefaultRootWindow(disp), SubstructureNotifyMask | ExposureMask | KeyPressMask );
    XGrabKey(disp,0x41,ControlMask,DefaultRootWindow(disp),True,GrabModeAsync,GrabModeAsync); //CTRL+Space
    XGrabKey(disp,0x17,AnyModifier,DefaultRootWindow(disp),True,GrabModeAsync,GrabModeAsync); //CTRL+TAB & ALT+TAB
    XGrabKey(disp,0x64,ControlMask,DefaultRootWindow(disp),True,GrabModeAsync,GrabModeAsync); //CTRL+Left
    XGrabKey(disp,0x66,ControlMask,DefaultRootWindow(disp),True,GrabModeAsync,GrabModeAsync); //CTRL+Right
    ctrl = new SlideConnection((char *)"/tmp/Slide_wm.sock");
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
            //Debug only
        Atom atom,retType;
        int formatReturned;
        unsigned long bytesReturned = 0,itemsReturned = 0;
        unsigned char *values;
        //May be removed any time!
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
                        case 0x41: //CTRL+Space
                            tileWorkspaces();
                            XUngrabKey(disp,0x41,ControlMask,DefaultRootWindow(disp));
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
                case ButtonPress: //Usually thrown on Desktop, when using Tile-Workspaces & then select one
                    //Restore all desks
                    sprintf(msg,"Click was on #%i",event.xbutton.window);
                    Logger::getInstance()->log(msg);
                    for(unsigned int i=0;i<numWorkspaces;i++)
                    {
                        XResizeWindow(disp,desktop[i],screenWidth,screenHeight-40);
                        XMoveWindow(disp,desktop[i],0,40);
                        if(desktop[i] == event.xbutton.window)
                        {
                            sprintf(msg,"Showing Desk #%i",i);
                            Logger::getInstance()->log(msg);
                            currentWorkspace = i;
                            XRaiseWindow(disp,desktop[i]);
                        }
                        XUngrabButton(disp,1,AnyModifier,desktop[i]);
                        XGrabKey(disp,0x41,ControlMask,DefaultRootWindow(disp),True,GrabModeAsync,GrabModeAsync);
                    }

                    for(unsigned int i=0;i<windows.size();i++)
                    {
                        if(windows[i]->state & SlideWindow::STATE_SHOWN)
                        {
                            windows[i]->restoreGeometry();
                        }
                    }
                    break;
                case ButtonRelease:
                    if(event.xbutton.subwindow != None)
                    {
                        XFetchName(disp,event.xbutton.subwindow,&wnd_name);
                        sprintf(msg,"[Press] Window-Title: %s",wnd_name);
                        Logger::getInstance()->log(msg);

                        if(strncmp(wnd_name,"__SLIDE__Close",10) == 0)
                        {
                            closeWindow(&event);
                        }
                        else if(strncmp(wnd_name,"__SLIDE__Maxi",9) == 0)
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
                        Logger::getInstance()->log("Focus event.");
                        focusWindow(&event);
                    }
                    break;
                case MapNotify:
                    //if(event.xmap.window == None) break;
                    Logger::getInstance()->log((std::string)"MapNotify");
                    /*
                    atom = XInternAtom(disp,"WM_CLASS",False);
                    XGetWindowProperty(disp,event.xmap.window,atom,0,500,False,AnyPropertyType,&retType,&formatReturned,&itemsReturned,&bytesReturned,&values);
                    if(values == NULL) break;
                    sprintf(msg,"Window-Title: %s",values);
                    Logger::getInstance()->log(msg);
                    */
                    XFetchName(disp,event.xmap.window,&wnd_name);
                    sprintf(msg,"Window-Title: %s",wnd_name);
                    Logger::getInstance()->log(msg);
                    createWindow(&event);
//                    focusWindow(&event);
                    break;
                case DestroyNotify:
                    closeWindow(&event);
                    break;
                case MotionNotify:
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
                msg.type = GEOMETRYREPLY;
                msg.len = sizeof(int)*2;
                memcpy(msg.msg,&screenWidth,sizeof(int));
                memcpy(msg.msg+sizeof(int),&screenHeight,sizeof(int));
                Logger::getInstance()->log("Sending MSG to Core.");
                ctrl->sendMessage(&msg,msg.addr.sun_path);
            }
            else if(msg.type == WINDOWLISTINTEREST)
            {
                windowChangeListeners.push_back(std::string(msg.addr.sun_path));
                Logger::getInstance()->log("WMINOTIFICATION Listener setup!");
            }
            else
            {
                usleep(1000);
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
                CTRLMSG msg;
                msg.type = WINDOWLISTDESTROYWND;
                msg.len = sizeof(SlideWindow);
                memcpy(msg.msg,(*iter),sizeof(SlideWindow));

                for(unsigned int i=0;i<windowChangeListeners.size();i++)
                {
                    ctrl->putMessage(&msg,(char *)windowChangeListeners[i].c_str());
                }

                (*iter)->close();
                if(focusedWindow != NULL && e->xbutton.window == focusedWindow->getWindow(true)) focusedWindow = NULL;
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
                CTRLMSG msg;
                msg.type = WINDOWLISTDESTROYWND;
                msg.len = sizeof(SlideWindow);
                memcpy(msg.msg,(*iter),sizeof(SlideWindow));

                for(unsigned int i=0;i<windowChangeListeners.size();i++)
                {
                    ctrl->putMessage(&msg,(char *)windowChangeListeners[i].c_str());
                }

                (*iter)->close(false);
                if(focusedWindow != NULL && e->xdestroywindow.event == focusedWindow->getWindow()) focusedWindow = NULL;
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
        
        XWMHints *wmhints = XGetWMHints(disp,e->xmap.window);
        
        if(wmhints != NULL)
        {
            Logger::getInstance()->log("WMHints specified:");
            char msg[500];
            sprintf(msg,"Initial State: %i",wmhints->initial_state);
            Logger::getInstance()->log(msg);
            sprintf(msg,"Input: %s",(wmhints->input == True ? "yes" : "no"));
            Logger::getInstance()->log(msg);
        }
        
        if(wmhints == NULL || (wmhints->initial_state == NormalState && wmhints->input == True))
        {
            w->state |= SlideWindow::STATE_FOCUSED;

            if(focusedWindow != NULL)
            {
                focusedWindow->state ^= SlideWindow::STATE_FOCUSED;
            }
            focusedWindow = w;

            XSetInputFocus(disp,w->getWindow(),RevertToNone,CurrentTime);
        }
        else if(wmhints != NULL && wmhints->initial_state != NormalState)
        {
          w->state = SlideWindow::STATE_HIDDEN;
        }
        windows.push_back(w);
        
        if(wmhints != NULL) XFree(wmhints);
        
        CTRLMSG msg;
        msg.type = WINDOWLISTCREATEWND;
        msg.len = sizeof(SlideWindow);
        memcpy(msg.msg,w,sizeof(SlideWindow));

        for(unsigned int i=0;i<windowChangeListeners.size();i++)
        {
            Logger::getInstance()->log("Sending WML-Create-Notify");
            ctrl->putMessage(&msg,(char *)windowChangeListeners[i].c_str());
        }

    }
    else if(strncmp(wndName,"__SLIDE__Desktop",16) == 0)
    {
        Logger::getInstance()->log("Creating desktop");
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
        if(windows[i]->getWindow(true) == evtWnd || windows[i]->getWindow() == evtWnd)
        {
            XEvent e;
            e.type = Expose;
            if(focusedWindow != NULL)
            {
                focusedWindow->state ^= SlideWindow::STATE_FOCUSED;
                e.xexpose.window = focusedWindow->getWindow(true);
                XSendEvent(disp,focusedWindow->getWindow(true),True,ExposureMask,&e);
            }

            e.xexpose.window = windows[i]->getWindow(true);
            windows[i]->state |= SlideWindow::STATE_FOCUSED;
            XSendEvent(disp,windows[i]->getWindow(true),True,ExposureMask,&e);

            focusedWindow = windows[i];
            break;
        }
    }
    XMapRaised(disp,evtWnd);
    XSetInputFocus(disp,evtWnd,RevertToNone,CurrentTime);
}

void SlideWindowManager::moveWindow(XEvent *e)
{
    char *wndName;
    if(e->xmotion.window != None)
    {
        XFetchName(disp,e->xmotion.window,&wndName);
        if(strncmp("__SLIDE__Deco",wndName,9) == 0)
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
                    if(focusedWindow != NULL)
                    {
                        focusedWindow->state ^= SlideWindow::STATE_FOCUSED;
                    }
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

    char msg[500];
    sprintf(msg,"We have %i windows to tile!",numWindows);
    Logger::getInstance()->log(msg);

    int widthPerWindow  = screenWidth/(numWindows < 4 ? numWindows : 4);
    int rows = ceil((double)numWindows/4.0);
    int heightPerWindow = (screenHeight-40)/(rows == 0 ? 1 : rows);

    sprintf(msg,"We have %i rows!",rows);
    Logger::getInstance()->log(msg);

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

int *SlideWindowManager::errorHandler(Display *d,XErrorEvent *e)
{
    Logger::getInstance()->log("XError.");
    return 0;
}

void SlideWindowManager::tileWorkspaces()
{
    int x=0,y=40;
    int widthPerWindow  = screenWidth/(numWorkspaces < 4 ? numWorkspaces : 4);
    int rows = ceil((double)numWorkspaces/4.0);
    int heightPerWindow = (screenHeight-40)/(rows == 0 ? 1 : rows);
    for(unsigned int i=0;i<numWorkspaces;i++)
    {
        XGrabButton(disp,1,AnyModifier,desktop[i],False,ButtonPressMask,GrabModeAsync,GrabModeAsync,None,None);
//        XSelectInput(disp,desktop[i],ButtonPressMask | ButtonReleaseMask | OwnerGrabButtonMask);
        XResizeWindow(disp,desktop[i],widthPerWindow-10,heightPerWindow-10);
        XMoveWindow(disp,desktop[i],x+5,y+5);
        x += widthPerWindow;
        if(x >= screenWidth)
        {
            x  = 0;
            y += heightPerWindow;
        }
    }

    //Resize the windows accordingly (only shown windows, anyway)
    double fx = (double)(widthPerWindow-10)/(double)screenWidth,fy = (double)heightPerWindow / (double)(screenHeight-50);
    char msg[500];
    sprintf(msg,"Factors: %f %f",fx,fy);
    Logger::getInstance()->log(msg);

    for(unsigned int i=0;i<windows.size();i++)
    {
        if(windows[i]->state & SlideWindow::STATE_SHOWN)
        {
            windows[i]->resizeBy(fx,fy);
            windows[i]->move((int)((double)windows[i]->getX()*fx),(int)((double)windows[i]->getY()*fy));
        }
    }
}
