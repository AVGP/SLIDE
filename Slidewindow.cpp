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
        //XSelectInput(d,wndMaximize,ButtonReleaseMask);

        XReparentWindow(d,w,wndDecoration,0,20);
        XMapRaised(d,w);
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

    state = 0;

    recentGeometry.x      = x;
    recentGeometry.y      = y;
    recentGeometry.width  = width;
    recentGeometry.height = height;
}

void SlideWindow::move(int newX, int newY)
{
    recentGeometry.x = x;
    recentGeometry.y = y;

    x = newX;
    y = newY;

    XMoveWindow(disp,wndDecoration,newX,newY);
}

void SlideWindow::resize(int w, int h)
{
    recentGeometry.width  = width;
    recentGeometry.height = height;

    width  = w;
    height = h;

    XResizeWindow(disp,wndDecoration,w,h);
    XResizeWindow(disp,wndWindow,w,h-20);

    XMoveWindow(disp,wndClose,w-18,2);
    XMoveWindow(disp,wndMaximize,w-40,2);
}

void SlideWindow::putOnDesk(unsigned char newDesk)
{}

void SlideWindow::close()
{
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

    XTextProperty wnd_name;
    XGetWMIconName(disp,wndWindow,&wnd_name);
    char **wnd_name_str;
    int n_strs = 0;
    XTextPropertyToStringList(&wnd_name,&wnd_name_str,&n_strs);
    XSetForeground(disp,gc,RGB(80,80,160));
    XDrawString(disp,wndDecoration,gc,5,15,wnd_name_str[0],strlen(wnd_name_str[0]));

    XFree(gc);
    XFreeStringList(wnd_name_str);
}

Window SlideWindow::getWindow(bool subwindow)
{
    if(wndDecoration != None && subwindow) return wndDecoration;
    else return wndWindow;
}

unsigned char SlideWindow::getDesk()
{
    return desk;
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
