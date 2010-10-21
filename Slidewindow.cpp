#include "Slidewindow.h"

SlideWindow::SlideWindow(Display *d, Window w, Window parent, int group, bool sticky, unsigned char desk)
{
    XWindowAttributes attr;
    char *t_name;

    XGetWindowAttributes(d,w,&attr);
    XFetchName(d,w,&t_name);
//Debug
    char msg[255];
    sprintf(msg,"Mapping Request for %s",t_name);
    Logger::getInstance()->log(msg);
//
    this->wndWindow     = w;
    if(strncmp(t_name,"__SLIDE__",9) != 0)
    {
        this->wndDecoration = XCreateSimpleWindow(d,
                                                DefaultRootWindow(d),
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
        XSelectInput(d,wndDecoration, ButtonPressMask | Button1MotionMask | ButtonReleaseMask);
        XSelectInput(d,w,ButtonReleaseMask);

        wndClose = XCreateSimpleWindow(d,wndDecoration,attr.width-16,2,14,14,1,RGB(200,0,0),RGB(255,100,100));
        XSetStandardProperties(d,wndClose,"SlideClose","SlideClose",None,NULL,0,NULL);
        XSelectInput(d,wndClose,ButtonReleaseMask);

        wndMaximize = XCreateSimpleWindow(d,wndDecoration,attr.width-36,2,14,14,1,RGB(0,200,0),RGB(100,255,100));
        XSetStandardProperties(d,wndMaximize,"SlideMaxi","SlideMaxi",None,NULL,0,NULL);
        XSelectInput(d,wndMaximize,ButtonReleaseMask);

        XReparentWindow(d,w,wndDecoration,0,20);
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

    XMapRaised(d,w);

}

void SlideWindow::move(int newX, int newY)
{
    XMoveWindow(disp,wndDecoration,newX,newY);
}

void SlideWindow::resize(int w, int h)
{
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
