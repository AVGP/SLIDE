#include "Slidewindow.h"

const unsigned char SlideWindow::STATE_SHOWN     = 1;
const unsigned char SlideWindow::STATE_MAXIMIZED = 1 << 1;
const unsigned char SlideWindow::STATE_FOCUSED   = 1 << 2;


SlideWindow::SlideWindow(Display *d, Window w, Window parent, unsigned char desk, int group, bool sticky)
{
    XWindowAttributes attr;
    char *t_name;

    XGetWindowAttributes(d,w,&attr);
    XFetchName(d,w,&t_name);
    this->wndWindow = w;
    if(strncmp(t_name,"__SLIDE__",9) != 0)
    {
        this->wndDecoration = XCreateSimpleWindow(d,
                                                parent,
                                                attr.x,
                                                attr.y,
                                                attr.width+2,
                                                attr.height+22,
                                                1,
                                                RGB(180,180,180),
                                                RGB(240,240,240));
        this->width         = attr.width+2;
        this->height        = attr.height+22;

        XSetStandardProperties(d,wndDecoration,"SlideDeco","SlideDeco",None,NULL,0,NULL);
        XSelectInput(d,wndDecoration, ExposureMask | ButtonPressMask | Button1MotionMask | ButtonReleaseMask);
        XSelectInput(d,w, ButtonReleaseMask | SubstructureNotifyMask);

        wndClose = XCreateSimpleWindow(d,wndDecoration,attr.width-16,2,14,14,1,RGB(200,0,0),RGB(255,100,100));
        XSetStandardProperties(d,wndClose,"SlideClose","SlideClose",None,NULL,0,NULL);
        XSelectInput(d,wndClose,ButtonReleaseMask);

        wndMaximize = XCreateSimpleWindow(d,wndDecoration,attr.width-36,2,14,14,1,RGB(0,200,0),RGB(100,255,100));
        XSetStandardProperties(d,wndMaximize,"SlideMaxi","SlideMaxi",None,NULL,0,NULL);

        XReparentWindow(d,w,wndDecoration,0,20);
//        XMapRaised(d,w);
        XMapRaised(d,wndDecoration);
        XMapRaised(d,wndClose);
        XMapRaised(d,wndMaximize);
        XSetInputFocus(d,w,RevertToNone,CurrentTime);
    }
    else
    {
        this->wndDecoration = None;
        this->width         = attr.width;
        this->height        = attr.height;
        XLowerWindow(d,w);
    }

    this->groupID       = group;
    this->sticky        = sticky;
    this->desk          = desk;
    this->disp          = d;
    this->x             = attr.x;
    this->y             = attr.y;

    char **wnd_name_str;
    int n_strs = 0;
    XTextProperty wnd_name;
    XGetWMIconName(disp,wndWindow,&wnd_name);
    XTextPropertyToStringList(&wnd_name,&wnd_name_str,&n_strs);
    if(n_strs > 0) snprintf(title,255,"%s",wnd_name_str[0]);
    XFreeStringList(wnd_name_str);

    state = SlideWindow::STATE_SHOWN;

    recentGeometry.x      = x;
    recentGeometry.y      = y;
    recentGeometry.width  = width;
    recentGeometry.height = height;
}

void SlideWindow::move(int newX, int newY, bool updateGeometry)
{
    if(updateGeometry)
    {
        recentGeometry.x = x;
        recentGeometry.y = y;
    }

    x = newX;
    y = newY;

    XMoveWindow(disp,wndDecoration,newX,newY);
}

void SlideWindow::resize(int w, int h, bool updateGeometry)
{
    if(updateGeometry)
    {
        recentGeometry.width  = width;
        recentGeometry.height = height;
    }

    width  = w;
    height = h;

    XResizeWindow(disp,wndDecoration,w,h);
    XResizeWindow(disp,wndWindow,w,h-20);

    XMoveWindow(disp,wndClose,w-18,2);
    XMoveWindow(disp,wndMaximize,w-40,2);
}

void SlideWindow::putOnDesk(unsigned char newDesk,Window newDesktop)
{
    desk = newDesk;
    XReparentWindow(disp,wndDecoration,newDesktop,x,y);
}

void SlideWindow::close()
{

    Atom atom = XInternAtom(disp,"_NET_WM_PID",False);

    pid_t pid;
    Atom retType;
    int formatReturned;
    unsigned long bytesReturned,itemsReturned;
    unsigned char *values;

    XGetWindowProperty(disp,wndWindow,atom,0,(long)BUFSIZ,False,AnyPropertyType,&retType,&formatReturned,&itemsReturned,&bytesReturned,&values);

    if(itemsReturned > 0)
    {
        memcpy(&pid,values,sizeof(pid_t));
        kill(pid,SIGTERM);
    }
    XDestroyWindow(disp,wndDecoration);
}

void SlideWindow::hide()
{}

void SlideWindow::makeSticky()
{}

void SlideWindow::makeUnsticky()
{}

void SlideWindow::setGroup(int groupID)
{}

void SlideWindow::show(bool maximized)
{}

void SlideWindow::drawDecoration(bool focus)
{

    GC gc = XCreateGC(disp,wndDecoration,0,0);
    int y=0;
    int baseColor;
    if(focus) baseColor = 205;
    else baseColor = 105;

    for(y=0;y<20;y++)
    {
        XSetForeground(disp,gc,RGB((baseColor+y*2),(baseColor+y*2),(baseColor+y*2)));
        XDrawLine(disp,wndDecoration,gc,0,y,width,y);
    }

    XSetForeground(disp,gc,RGB(80,80,160));
    XDrawString(disp,wndDecoration,gc,5,15,title,strlen(title));

    XFree(gc);
}

Window SlideWindow::getWindow(bool decoWindow)
{
    if(wndDecoration != None && decoWindow) return wndDecoration;
    else return wndWindow;
}

unsigned char SlideWindow::getDesk()
{
    return desk;
}

char *SlideWindow::getTitle()
{
    return title;
}

void SlideWindow::restoreGeometry()
{
    x       = recentGeometry.x;
    y       = recentGeometry.y;
    width   = recentGeometry.width;
    height  = recentGeometry.height;
    move(x,y);
    resize(width,height);
}
